cmake_minimum_required(VERSION 3.7)
include(CMakeDependentOption)
include(CMakePrintHelpers)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 3)
set(PROJECT_VERSION_PATCH 5)

set(PROJECT_DESCRIPTION "Quality of life when working with the Lua C API")

project("slw"
  VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
  LANGUAGES CXX
)

cmake_dependent_option(SLW_BUILD_TESTS "Generate installation information" ON
    "NOT SLW_ALLOW_INSTALLATION" OFF)

option(SLW_BUILD_TESTS "Build test suite for libslw" OFF)
option(SLW_ALLOW_INSTALLATION "Generate installation information" ON)

set(CMAKE_CXX_STANDARD 14)
list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Lua 5.3 REQUIRED)

configure_file("configure.h.in" "include/${PROJECT_NAME}/configure.h")

set(PUBLIC_HEADERS
  include/slw/get_field.hpp
  include/slw/state.hpp
  include/slw/types.hpp
  include/slw/utility.hpp
  include/slw/reference.hpp
  include/slw/value.hpp
  include/slw/functional.hpp
  include/slw/exception.hpp
  include/slw/range/type_range.hpp
  include/slw/range/apply.hpp
  include/slw/variant/variant.hpp
  include/slw/variant/bool.hpp
  include/slw/variant/int.hpp
  include/slw/variant/number.hpp
  include/slw/variant/string.hpp
  include/slw/variant/table.hpp
  include/slw/variant/uint.hpp
  include/slw/variant/factory.hpp
)

set(PRIVATE_HEADERS
    src/variant/variant_ctors.hpp
    src/variant/variant_numeric_dereference.hpp
    src/variant/variant_op_assign.hpp
    src/variant/variant_op_eq.hpp
)

set(SOURCES
    src/get_field.cpp
    src/state.cpp
    src/types.cpp
    src/utility.cpp
    src/reference.cpp
    src/value.cpp
    src/functional.cpp
    src/exception.cpp
    src/variant/variant.cpp
    src/variant/bool.cpp
    src/variant/int.cpp
    src/variant/number.cpp
    src/variant/string.cpp
    src/variant/table.cpp
    src/variant/uint.cpp
    src/variant/factory.cpp

    ${PRIVATE_HEADERS}
    ${PUBLIC_HEADERS}
)

add_library(${PROJECT_NAME} SHARED
  ${SOURCES}
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

add_executable(${PROJECT_NAME}-test
    src/main.cpp
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}-test
PUBLIC
  Threads::Threads
  Lua::Lua
)

target_include_directories(${PROJECT_NAME}-test
PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(${PROJECT_NAME}-test PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Werror>)

target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Werror>)

target_link_libraries(${PROJECT_NAME}
PUBLIC
  Threads::Threads
  Lua::Lua
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

set_target_properties(${PROJECT_NAME} PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
  INTERFACE_LINK_LIBRARIES "Lua::Lua"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (SLW_BUILD_TESTS)
    if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)
        add_subdirectory(tests)
    elseif(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/share/tests)
        add_subdirectory(share/tests)
    endif()
endif (SLW_BUILD_TESTS)

if (SLW_ALLOW_INSTALLATION)
  include(GNUInstallDirs)

  set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
  set(PROJECT_NAME_TARGETS "${PROJECT_NAME}-targets")

  install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME_TARGETS}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

  install(DIRECTORY include/slw DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp")

  install(EXPORT ${PROJECT_NAME_TARGETS}
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${INSTALL_CONFIGDIR}
  )

  # pkg-config
  configure_file(pc.in ${PROJECT_NAME}.pc @ONLY)

  install(FILES
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
  )

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion # TODO: Investigate, and FIX
  )

  configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
  )

  # Install the configuration, version, and custom find modules
  install(FILES
    ${CMAKE_CURRENT_LIST_DIR}/cmake/FindLua.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
  )

  # Export from the build tree
  configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/FindLua.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FindLua.cmake
    COPYONLY
  )

  export(EXPORT
    ${PROJECT_NAME_TARGETS}
    FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
  )
endif (SLW_ALLOW_INSTALLATION)

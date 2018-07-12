cmake_minimum_required(VERSION 3.7)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 3)
set(PROJECT_VERSION_PATCH 3)

set(PROJECT_DESCRIPTION "Quality of life when working with the Lua C API")

project("slw"
  VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    LANGUAGES CXX
)

option(SLW_ALLOW_INSTALLATION "Generate installation information" ON)
option(SLW_BUILD_TESTS "Build test suite for libslw" OFF)

set(CMAKE_CXX_STANDARD 11)
list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Lua 5.1 REQUIRED)

configure_file("configure.h.in" "include/${PROJECT_NAME}/configure.h")

set(PUBLIC_HEADERS
  include/slw/call.hpp
  include/slw/field.hpp
  include/slw/get_field.hpp
  include/slw/state.hpp
  include/slw/stdout.hpp
  include/slw/types.hpp
  include/slw/utility.hpp
)

add_library(${PROJECT_NAME} SHARED
  src/call.cpp
  src/field.cpp
  src/get_field.cpp
  src/state.cpp
  src/types.cpp
  src/utility.cpp
  src/helpers/tostring.cpp
  src/state/peek.cpp
  src/state/pop.cpp
  src/state/push.cpp
  src/state/remove.cpp

  src/helpers/peek.hpp
  src/helpers/pop.hpp
  src/helpers/push.hpp
  src/helpers/remove.hpp
  src/helpers/tostring.hpp

  ${PUBLIC_HEADERS}
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Werror>)

target_link_libraries(${PROJECT_NAME}
PUBLIC
  Threads::Threads
  Lua::Lua
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

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
  add_subdirectory(tests)
endif (SLW_BUILD_TESTS)

if (SLW_ALLOW_INSTALLATION)
	include(GNUInstallDirs)

  set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
  set(PROJECT_NAME_TARGETS "${PROJECT_NAME}-targets")

  install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME_TARGETS}
	  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/slw
	)

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

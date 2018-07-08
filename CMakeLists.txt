cmake_minimum_required(VERSION 3.7)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 3)
set(PROJECT_VERSION_PATCH 1)

set(PROJECT_DESCRIPTION "Quality of life when working with the Lua C API")

project("slw"
  VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    LANGUAGES CXX
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules/")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(LuaJIT 2.0.2 REQUIRED)
find_package(OpenGL REQUIRED)

configure_file("configure.h.in" "include/${PROJECT_NAME}/configure.h")

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
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Threads::Threads
  ${LUA_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER
  include/slw/call.hpp
  include/slw/field.hpp
  include/slw/get_field.hpp
  include/slw/state.hpp
  include/slw/stdout.hpp
  include/slw/types.hpp
  include/slw/utility.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  include
  src
  ${LUA_INCLUDE_DIR}
)

option(SLW_ALLOW_INSTALLATION "Generate installation information" ON)
option(SLW_BUILD_TESTS "Build test suite for libslw" OFF)

if (SLW_BUILD_TESTS)
	add_executable(${PROJECT_NAME}-ci
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
	  src/main.cpp
	)

	target_include_directories(${PROJECT_NAME}-ci PRIVATE 
	  include
	  src
	  ${LUA_INCLUDE_DIR}
	)

	target_link_libraries(${PROJECT_NAME}-ci PRIVATE
		${PROJECT_NAME}
	  Threads::Threads
	  ${LUA_LIBRARIES}
	)
endif (SLW_BUILD_TESTS)

if (SLW_ALLOW_INSTALLATION)
	include(GNUInstallDirs)

	install(TARGETS ${PROJECT_NAME}
	  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	# pkg-config
	configure_file(pc.in ${PROJECT_NAME}.pc @ONLY)

	install(FILES
	  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
	  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
	)
endif (SLW_ALLOW_INSTALLATION)

cmake_minimum_required(VERSION 3.7)

set(CMAKE_BUILD_TYPE "Debug")

if (NOT PROJECT_NAME)
    set(PROJECT_NAME "slw")
endif()

set(LIB_NAME ${PROJECT_NAME})

project(${PROJECT_NAME}-ci
    LANGUAGES CXX
)

set(MODULE_DIRECTORY "${PROJECT_SOURCE_DIR}/../cmake")

if (NOT IS_DIRECTORY ${MODULE_DIRECTORY})
    set(MODULE_DIRECTORY "${PROJECT_SOURCE_DIR}/cmake")
endif()

list(INSERT CMAKE_MODULE_PATH 0 ${MODULE_DIRECTORY})
set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)

if (NOT THREADS_FOUND)
  find_package(Threads REQUIRED)
endif()

if (NOT LUA_FOUND)
  find_package(Lua 5.3 REQUIRED)
endif()

add_executable(${PROJECT_NAME}
  src/catch.hpp
  src/main.cpp
  src/state.cpp
  src/reference.cpp
  src/variant.cpp
  src/functional.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  Lua::Lua
  ${LIB_NAME}
  Threads::Threads
)


cmake_minimum_required(VERSION 3.7)

if (NOT THREADS_FOUND)
  find_package(Threads REQUIRED)
endif()

if (NOT LUA_FOUND)
  find_package(Lua 5.1 REQUIRED)
endif()
	
add_executable(${PROJECT_NAME}-ci
  src/main.cpp
  src/state/copy.cpp
  src/state/load.cpp
  src/state/peek.cpp
  src/state/pop.cpp
  src/state/push.cpp
)

target_link_libraries(${PROJECT_NAME}-ci 
PRIVATE
  Lua::Lua
  ${PROJECT_NAME}
  Threads::Threads
)


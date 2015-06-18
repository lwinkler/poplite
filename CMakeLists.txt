project(poplite)
cmake_minimum_required(VERSION 2.6)

add_definitions(-std=c++11)
find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

set(CMAKE_CXX_FLAGS "-Wextra -Wno-unused-parameter -Wno-deprecated-register")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3")
set(CMAKE_CXX_FLAGS_DEBUG          "-O2 -g")


add_subdirectory(examples)

project(poplite_getset_example)
cmake_minimum_required(VERSION 2.6)

# Options to be set with ccmake or cmake-gui
set(CMAKE_BUILD_TYPE       "Debug" CACHE BOOL "Mode of compilation")
set(POPLITE_DIR            ".."    CACHE PATH "Directory of poplite")

set(CMAKE_CXX_FLAGS "-Wextra -Wno-unused-parameter -Wno-deprecated-register")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -g")
set(CMAKE_CXX_FLAGS_DEBUG          "-O2 -g")

add_definitions(-std=c++11)
find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
include_directories(. ${POPLITE_DIR} generated)

# add main
add_executable(main main.cpp)
target_link_libraries(main ${Boost_LIBRARIES} )

# add object main
add_executable(main.TestClass generated/main.TestClass.cpp)
target_link_libraries(main.TestClass ${Boost_LIBRARIES} )

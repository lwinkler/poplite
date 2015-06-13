project(poplite_world_list_game)
cmake_minimum_required(VERSION 2.6)

# Options to be set with ccmake or cmake-gui
set(CMAKE_BUILD_TYPE       "Debug" CACHE BOOL "Mode of compilation")
set(POPLITE_DIR            "../.." CACHE PATH "Directory of poplite")

set(CMAKE_CXX_FLAGS "-Wextra -Wno-unused-parameter -Wno-deprecated-register")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3")
set(CMAKE_CXX_FLAGS_DEBUG          "-O2 -g")

add_definitions(-std=c++11)
find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
include_directories(. ${POPLITE_DIR} generated)

# Add custom target for source generation
add_custom_target(generate 
	${POPLITE_DIR}/parser/pop_gen_src client client.hpp && 
	${POPLITE_DIR}/parser/pop_gen_src server server.hpp)
add_custom_command(OUTPUT generated/main.client.cpp COMMAND ${POPLITE_DIR}/parser/pop_gen_src client client.hpp)
add_custom_command(OUTPUT generated/main.server.cpp COMMAND ${POPLITE_DIR}/parser/pop_gen_src server server.hpp)

# add main
add_executable(word_list_game word_list_game.cpp)
target_link_libraries(word_list_game ${Boost_LIBRARIES})

# add object client
add_executable(main.client client.cpp generated/main.client.cpp)
target_link_libraries(main.client ${Boost_LIBRARIES})

# add object server
add_executable(main.server server.cpp generated/main.server.cpp)
target_link_libraries(main.server ${Boost_LIBRARIES})

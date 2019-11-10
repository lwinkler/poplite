#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	A game for 2 players. Guess words of the category starting with the right letter.
#
project(word_list_game)

find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log filesystem program_options REQUIRED )
include_directories(${Boost_INCLUDE_DIR})

pop_add_objects("client;server" "client.hpp;client.cpp;server.hpp;server.cpp")
# alternative:
# pop_add_object(server server.hpp server.cpp ${CMAKE_CURRENT_BINARY_DIR}/client.iface.hpp)
# pop_add_object(client client.hpp client.cpp ${CMAKE_CURRENT_BINARY_DIR}/server.iface.hpp)
add_executable(word_list_game word_list_game.cpp ${CMAKE_CURRENT_BINARY_DIR}/client.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/server.iface.hpp)
target_link_libraries(word_list_game ${Boost_LIBRARIES})

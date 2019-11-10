#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Example using tree-like structures.
#
project(popc_tests_tree)

pop_add_object(Tree tree.hpp tree.cc)
add_executable(main_tree main.cc ${CMAKE_CURRENT_BINARY_DIR}/Tree.iface.hpp)
target_link_libraries(main_tree ${Boost_LIBRARIES})

add_test(NAME run_tree COMMAND ./main_tree 3 4 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


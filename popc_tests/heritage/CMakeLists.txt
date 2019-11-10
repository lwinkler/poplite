#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Demonstrate the use of inheritence for parallel objects.
#
project(popc_tests_heritage)

pop_add_objects("Child;Mother" "child.hpp;child.cc;mother.hpp;mother.cc")
add_executable(main_heritage main.cc ${CMAKE_CURRENT_BINARY_DIR}/Child.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/Mother.iface.hpp)
target_link_libraries(main_heritage ${Boost_LIBRARIES})

add_test(NAME run_heritage COMMAND ./main_heritage WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


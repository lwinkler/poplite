#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Example using callback functions.
#
project(popc_tests_callback)

pop_add_objects("Titi;Toto" "titi.hpp;titi.cc;toto.hpp;toto.cc")
add_executable(main_callback main.cc ${CMAKE_CURRENT_BINARY_DIR}/Titi.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/Toto.iface.hpp)
target_link_libraries(main_callback ${Boost_LIBRARIES})

add_test(NAME run_callback COMMAND ./main_callback WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


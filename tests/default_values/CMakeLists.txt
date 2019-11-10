#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Test the use of default values for C++ methods.
#
project(tests_default_values)

pop_add_object(tester_default tester_default.hpp)
add_executable(main_default_values main.cpp ${CMAKE_CURRENT_BINARY_DIR}/tester_default.iface.hpp)
target_link_libraries(main_default_values ${Boost_LIBRARIES})

add_test(NAME run_default_values COMMAND ./main_default_values WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


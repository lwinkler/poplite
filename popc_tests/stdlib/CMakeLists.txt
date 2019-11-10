#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Example with different C++ classes from stdlib.
#
project(popc_tests_stdlib)

pop_add_object(TestMethod method.hpp method.cc)
add_executable(main_stdlib main.cc ${CMAKE_CURRENT_BINARY_DIR}/TestMethod.iface.hpp)
target_link_libraries(main_stdlib ${Boost_LIBRARIES})

add_test(NAME run_stdlib COMMAND ./main_stdlib WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


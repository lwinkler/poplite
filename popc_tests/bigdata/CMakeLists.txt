#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Tests with large amounts of data.
#
project(popc_tests_bigdata)

pop_add_object(POPObject POPObject.hpp)
add_executable(main_bigdata main.cc ${CMAKE_CURRENT_BINARY_DIR}/POPObject.iface.hpp)
target_link_libraries(main_bigdata ${Boost_LIBRARIES})

add_test(NAME run_bigdata COMMAND ./main_bigdata WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


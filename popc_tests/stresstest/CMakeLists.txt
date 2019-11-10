#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Test poplite in difficult cases.
#
project(popc_tests_stresstest)

pop_add_object(stress_test::POPObject POPObject.hpp POPObject.cc)
add_executable(main_stresstest main.cc ${CMAKE_CURRENT_BINARY_DIR}/stress_test/POPObject.iface.hpp)
target_link_libraries(main_stresstest ${Boost_LIBRARIES})

add_test(NAME run_stresstest COMMAND ./main_stresstest 200 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


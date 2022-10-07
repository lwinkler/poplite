#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Test class inheritance.
#
pop_add_objects("global_ns::child_ns::child;global_ns::parent_a;global_ns::parent_b_ns::parent_b" "child.hpp")

# Avoid warnings: Some methods are not marked override
add_definitions("-Winconsistent-missing-override")

add_executable(main_inheritance main.cpp
	${CMAKE_CURRENT_BINARY_DIR}/global_ns/parent_a.iface.hpp
	${CMAKE_CURRENT_BINARY_DIR}/global_ns/parent_b_ns/parent_b.iface.hpp
	${CMAKE_CURRENT_BINARY_DIR}/global_ns/child_ns/child.iface.hpp)

target_link_libraries(main_inheritance ${Boost_LIBRARIES})

add_test(NAME run_inheritance COMMAND ./main_inheritance WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

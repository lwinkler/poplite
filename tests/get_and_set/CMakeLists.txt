#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Test getting and setting different types.
#

# pop_add_object(TestClass test_class.hpp)
# pop_add_object(TestChildClass test_child_class.hpp ${CMAKE_CURRENT_BINARY_DIR}/TestClass.iface.hpp)
pop_add_objects("TestChildClass;TestClass" "test_child_class.hpp;test_class.hpp")
add_executable(main_get_set
	main.cpp
	${CMAKE_CURRENT_BINARY_DIR}/TestClass.iface.hpp
	${CMAKE_CURRENT_BINARY_DIR}/TestChildClass.iface.hpp)

target_link_libraries(main_get_set ${Boost_LIBRARIES})

add_test(NAME run_get_and_set COMMAND ./main_get_set WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Example of asynchronous object creation.
#
project(popc_tests_asynccreation)

pop_add_object(PopObject3 PopObject.h PopObject.cpp)
add_executable(main_asynccreation main.cpp ${CMAKE_CURRENT_BINARY_DIR}/PopObject3.iface.hpp)
target_link_libraries(main_asynccreation ${Boost_LIBRARIES})

add_test(NAME run_asynccreation COMMAND ./main_asynccreation WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


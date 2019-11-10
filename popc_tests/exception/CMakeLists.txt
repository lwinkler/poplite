#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Example on how to return and send exceptions.
#
project(popc_tests_exception)

#pop_add_objects("ClassExcep" "ClassExcep.hpp;ClassExcep.cpp;IOExceptions.cc")
add_library(popc_tests_exception ClassExcep.cpp IOExceptions.cc ${CMAKE_CURRENT_BINARY_DIR}/ClassExcep.iface.hpp)
pop_add_object(ClassExcep ClassExcep.hpp)
target_link_libraries(ClassExcep.obj popc_tests_exception)
add_executable(main_exception main.cc ${CMAKE_CURRENT_BINARY_DIR}/ClassExcep.iface.hpp)

target_link_libraries(main_exception ${Boost_LIBRARIES} popc_tests_exception)

add_test(NAME run_exception COMMAND ./main_exception WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


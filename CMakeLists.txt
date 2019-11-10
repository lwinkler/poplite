#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
project(poplite)
cmake_minimum_required(VERSION 3.2.1)

set(CMAKE_BUILD_TYPE       "Debug" CACHE BOOL "Mode of compilation")

add_definitions(-std=c++17)
find_package(Boost 1.40 COMPONENTS serialization system thread serialization context log program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(.)

set(CMAKE_CXX_FLAGS "-Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3")
set(CMAKE_CXX_FLAGS_DEBUG          "-O2 -g")
set(CMAKE_INCLUDE_CURRENT_DIR true) # automatically include current dir
set(POPLITE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Function used to add a parallel object to the compilation
function(pop_add_object object_name header_name)

	get_filename_component(_dir      ${header_name} ABSOLUTE)
	get_filename_component(_filename ${header_name} NAME_WE)

	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
	set(INC_DIRS "")
	foreach(dir ${dirs})
		# Create the include path artificially
		set(INC_DIRS "${INC_DIRS};-I${dir}")
	endforeach()
	set(INC_DIRS "${INC_DIRS};-I${CMAKE_CURRENT_BINARY_DIR}")

	# message("Add command to generate ${CMAKE_CURRENT_BINARY_DIR}/${object_name}.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/main.${object_name}.cpp")
	# note: there are conflicts while generating multiple files with one command
	#       fixed in Cmake 3.2.1: http://www.cmake.org/Bug/view.php?id=15116
	# Not fixed yet !
	string(REPLACE "::" "/" obj_name_path "${object_name}")
	string(REPLACE "::" "." obj_name_dot "${object_name}")
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${obj_name_path}.iface.hpp
			${CMAKE_CURRENT_BINARY_DIR}/${obj_name_path}.iface.cpp
			${CMAKE_CURRENT_BINARY_DIR}/${obj_name_path}.brok.cpp
			${CMAKE_CURRENT_BINARY_DIR}/main.${obj_name_dot}.cpp
			COMMAND ${POPLITE_DIR}/parser/popgen.py ${header_name} ${object_name} ${CMAKE_CURRENT_BINARY_DIR} -- ${INC_DIRS} 
		DEPENDS ${header_name}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	# extra arguments are seen as dependencies
	add_executable(${obj_name_dot}.obj ${CMAKE_CURRENT_BINARY_DIR}/main.${obj_name_dot}.cpp ${CMAKE_CURRENT_BINARY_DIR}/${obj_name_path}.brok.cpp ${ARGN})
	target_link_libraries(${obj_name_dot}.obj ${Boost_LIBRARIES} )

endfunction(pop_add_object)

# Function used to add several parallel objects to the compilation (it is faster to treat them in bulk)
function(pop_add_objects classnames dependencies)
	include_directories(${CMAKE_CURRENT_BINARY_DIR})

	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
	set(INC_DIRS "")
	foreach(dir ${dirs})
		# Create the include path artificially
		set(INC_DIRS "${INC_DIRS};-I${dir}")
	endforeach()

	# message("Add command to generate:${cmd_out}"
	list(GET dependencies 0 header_name)
	set(outputs "")

	foreach(cl ${classnames})
		string(REPLACE "::" "/" obj_name_path "${cl}")
		string(REPLACE "::" "." obj_name_dot "${cl}")
		set(output "${CMAKE_CURRENT_BINARY_DIR}/${obj_name_path}.iface.hpp")

		list(APPEND outputs
			${CMAKE_CURRENT_BINARY_DIR}/${obj_name_path}.iface.hpp
			${CMAKE_CURRENT_BINARY_DIR}/${obj_name_path}.iface.cpp
			${CMAKE_CURRENT_BINARY_DIR}/${obj_name_path}.brok.cpp
			${CMAKE_CURRENT_BINARY_DIR}/main.${obj_name_dot}.cpp)

		add_executable(${obj_name_dot}.obj ${CMAKE_CURRENT_BINARY_DIR}/main.${obj_name_dot}.cpp ${CMAKE_CURRENT_BINARY_DIR}/${obj_name_path}.brok.cpp ${dependencies})
		target_link_libraries(${obj_name_dot}.obj ${Boost_LIBRARIES})
	endforeach()
	string(REPLACE ";" "," classnames2 "${classnames}")

	add_custom_command(OUTPUT ${outputs} COMMAND ${POPLITE_DIR}/parser/popgen.py ${header_name} ${classnames2} ${CMAKE_CURRENT_BINARY_DIR} -- ${INC_DIRS} DEPENDS ${header_name}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endfunction(pop_add_objects)

# Function used to add several parallel objects to the compilation. Class names are read automatically by cmake (experimental)
function(pop_add_all_classes header_name)
	include_directories(${CMAKE_CURRENT_BINARY_DIR})

	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
	set(INC_DIRS "")
	foreach(dir ${dirs})
		# Create the include path artificially
		set(INC_DIRS "${INC_DIRS};-I${dir}")
	endforeach()

	# message("Look for parallel classes in ${header_name}")
	execute_process(COMMAND ${POPLITE_DIR}/parser/poplist.py ${header_name} -- ${INC_DIRS} OUTPUT_VARIABLE all_classes RESULT_VARIABLE res WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        pop_add_objects(${all_classes} "${header_name};${ARGN}")
endfunction(pop_add_all_classes)

enable_testing()
add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(popc_tests)

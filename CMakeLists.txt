project(poplite)
cmake_minimum_required(VERSION 3.2.1)

set(CMAKE_BUILD_TYPE       "Debug" CACHE BOOL "Mode of compilation")

add_definitions(-std=c++17)
find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log program_options REQUIRED )
include_directories(${Boost_INCLUDE_DIR})
include_directories(.)

set(CMAKE_CXX_FLAGS "-Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3")
set(CMAKE_CXX_FLAGS_DEBUG          "-O2 -g")
set(CMAKE_INCLUDE_CURRENT_DIR true) # automatically include current dir
set(POPLITE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Function to add the targets and commands for a new object
function(pop_add_class_generation_commands tarfile i_hpp i_cpp b_cpp o_cpp)
	add_custom_command(OUTPUT ${i_hpp} COMMAND tar xf ${tarfile} ${i_hpp} DEPENDS ${tarfile})
	add_custom_command(OUTPUT ${i_cpp} COMMAND tar xf ${tarfile} ${i_cpp} DEPENDS ${tarfile} ${i_hpp})
	add_custom_command(OUTPUT ${b_cpp} COMMAND tar xf ${tarfile} ${b_cpp} DEPENDS ${tarfile} ${i_hpp})
	add_custom_command(OUTPUT ${o_cpp} COMMAND tar xf ${tarfile} ${o_cpp} DEPENDS ${tarfile} ${i_cpp})
endfunction(pop_add_class_generation_commands)

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

	# message("Add command to generate gen/${object_name}.iface.hpp gen/main.${object_name}.cpp")
	# note: there are conflicts while generating multiple files with one command
	#       fixed in Cmake 3.2.1: http://www.cmake.org/Bug/view.php?id=15116
	# Not fixed yet !
	string(REPLACE "::" "/" obj_name_path "${object_name}")
	string(REPLACE "::" "." obj_name_dot "${object_name}")
	set(tarfile "gen/${header_name}.tar")
	add_custom_command(OUTPUT ${tarfile} COMMAND ${POPLITE_DIR}/parser/popgen.py ${header_name} ${object_name} -- ${INC_DIRS} DEPENDS ${header_name})

	pop_add_class_generation_commands(${tarfile} gen/${obj_name_path}.iface.hpp gen/${obj_name_path}.iface.cpp gen/${obj_name_path}.brok.cpp gen/main.${obj_name_dot}.cpp)

	# extra arguments are seen as dependencies
	add_executable(${obj_name_dot}.obj gen/main.${obj_name_dot}.cpp gen/${obj_name_path}.brok.cpp ${ARGN})
	target_link_libraries(${obj_name_dot}.obj ${Boost_LIBRARIES} )

endfunction(pop_add_object)

# Function used to add several parallel objects to the compilation (it is faster to treat them in bulk)
function(pop_add_objects classnames dependencies)

	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
	set(INC_DIRS "")
	foreach(dir ${dirs})
		# Create the include path artificially
		set(INC_DIRS "${INC_DIRS};-I${dir}")
	endforeach()

	# message("Add command to generate:${cmd_out}"
	list(GET dependencies 0 header_name)
	set(tarfile "gen/${header_name}.tar")

	foreach(cl ${classnames})
		string(REPLACE "::" "/" obj_name_path "${cl}")
		string(REPLACE "::" "." obj_name_dot "${cl}")
		# add_library(gen/main.${obj_name_path}.cpp)

		pop_add_class_generation_commands(${tarfile} gen/${obj_name_path}.iface.hpp gen/${obj_name_path}.iface.cpp gen/${obj_name_path}.brok.cpp gen/main.${obj_name_dot}.cpp)

		add_executable(${obj_name_dot}.obj gen/main.${obj_name_dot}.cpp gen/${obj_name_path}.brok.cpp ${dependencies})
		target_link_libraries(${obj_name_dot}.obj ${Boost_LIBRARIES})
	endforeach()
	string(REPLACE ";" "," classnames2 "${classnames}")

	add_custom_command(OUTPUT ${tarfile} COMMAND ${POPLITE_DIR}/parser/popgen.py ${header_name} ${classnames2} -- ${INC_DIRS} DEPENDS ${header_name})

endfunction(pop_add_objects)

add_subdirectory(examples)
add_subdirectory(popc_tests)

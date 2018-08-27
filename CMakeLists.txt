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
	add_custom_command(OUTPUT ${tarfile}
	COMMAND ${POPLITE_DIR}/parser/popgen.py ${header_name} ${object_name} -- ${INC_DIRS}
	DEPENDS ${header_name})

	add_custom_command(OUTPUT gen/${obj_name_path}.iface.hpp COMMAND tar xf ${tarfile} gen/${obj_name_path}.iface.hpp DEPENDS ${tarfile})
	add_custom_command(OUTPUT gen/${obj_name_path}.iface.cpp COMMAND tar xf ${tarfile} gen/${obj_name_path}.iface.cpp DEPENDS ${tarfile})
	add_custom_command(OUTPUT gen/${obj_name_path}.brok.cpp  COMMAND tar xf ${tarfile} gen/${obj_name_path}.brok.cpp  DEPENDS ${tarfile})
	add_custom_command(OUTPUT gen/main.${obj_name_dot}.cpp   COMMAND tar xf ${tarfile} gen/main.${obj_name_dot}.cpp   DEPENDS ${tarfile})

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

	foreach(cl ${classnames})
		string(REPLACE "::" "/" cl2 "${cl}")
		string(REPLACE "::" "." cl3 "${cl}")
		set(cmd_out "${cmd_out};gen/${cl2}.iface.hpp;gen/${cl2}.iface.cpp;gen/${cl2}.brok.cpp;gen/main.${cl3}.cpp")
		add_executable(${cl3}.obj gen/main.${cl3}.cpp gen/${cl2}.brok.cpp ${dependencies})
		target_link_libraries(${cl3}.obj ${Boost_LIBRARIES})
		# add_library(gen/main.${cl2}.cpp)
	endforeach()
	string(REPLACE ";" "," classnames2 "${classnames}")

	# message("Add command to generate:${cmd_out}")
	list(GET dependencies 0 header)
	add_custom_command(
		OUTPUT ${cmd_out}
		COMMAND ${POPLITE_DIR}/parser/popgen.py ${header} ${classnames2} -- ${INC_DIRS}
		DEPENDS ${dependencies}
	)

endfunction(pop_add_objects)

add_subdirectory(examples)
add_subdirectory(popc_tests)

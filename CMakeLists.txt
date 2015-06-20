project(poplite)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE       "Debug" CACHE BOOL "Mode of compilation")

add_definitions(-std=c++11)
find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log REQUIRED )
include_directories(${Boost_INCLUDE_DIR})
include_directories(.)

set(CMAKE_CXX_FLAGS "-Wextra -Wno-unused-parameter -Wno-deprecated-register")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3")
set(CMAKE_CXX_FLAGS_DEBUG          "-O2 -g")
set(CMAKE_INCLUDE_CURRENT_DIR true) # automatically include current dir


# Function used to add a parallel object to the compilation
function(pop_add_object object_name header_name)
	get_filename_component(_ext      ${header_name} EXT)
	get_filename_component(_dir      ${header_name} ABSOLUTE)
	get_filename_component(_filename ${header_name} NAME_WE)

	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
	set(INC_DIRS "")
	foreach(dir ${dirs})
		# Create the include path artificially
		set(INC_DIRS "${INC_DIRS};-I${dir}")
	endforeach()

	# extra arguments are seen as dependencies
	set (extra_macro_args ${ARGN})

	# message("Add command to generate generated/${_filename}.ids${_ext} generated/${_filename}.iface${_ext} generated/${_filename}.brok${_ext} generated/main.${object_name}.cpp")
	add_custom_command(OUTPUT generated/${_filename}.iface${_ext} generated/${_filename}.ids${_ext} generated/${_filename}.brok${_ext} generated/main.${object_name}.cpp
	COMMAND ${POPLITE_DIR}/parser/popgen.py ${header_name} ${INC_DIRS}
	DEPENDS ${header_name})
	add_executable(main.${object_name} generated/main.${object_name}.cpp ${extra_macro_args})
	target_link_libraries(main.${object_name} ${Boost_LIBRARIES})
endfunction(pop_add_object)

add_subdirectory(examples)

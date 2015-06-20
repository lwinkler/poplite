project(poplite_getset_example)
cmake_minimum_required(VERSION 2.6)

# Options to be set with ccmake or cmake-gui
set(CMAKE_BUILD_TYPE       "Debug" CACHE BOOL "Mode of compilation")
set(POPLITE_DIR            "../.." CACHE PATH "Directory of poplite")

set(CMAKE_CXX_FLAGS "-Wextra -Wno-unused-parameter -Wno-deprecated-register")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3")
set(CMAKE_CXX_FLAGS_DEBUG          "-O2 -g")

add_definitions(-std=c++11)
find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
include_directories(. ${POPLITE_DIR} generated)

function(pop_add_header header_name)
	get_filename_component(_ext      ${header_name} EXT)
	get_filename_component(_dir      ${header_name} ABSOLUTE)
	get_filename_component(_filename ${header_name} NAME_WE)
endfunction(pop_add_header)

function(pop_add_object object_name header_name)
	get_filename_component(_ext      ${header_name} EXT)
	get_filename_component(_dir      ${header_name} ABSOLUTE)
	get_filename_component(_filename ${header_name} NAME_WE)
	message("add command to generate generated/${_filename}.ids${_ext} generated/${_filename}.iface${_ext} generated/${_filename}.brok${_ext} generated/main.${object_name}.cpp")
	add_custom_command(OUTPUT generated/${_filename}.ids${_ext} generated/${_filename}.iface${_ext} generated/${_filename}.brok${_ext} generated/main.${object_name}.cpp
	COMMAND ${POPLITE_DIR}/parser/popgen.py ${header_name} -I${POPLITE_DIR}) ## TODO: full includes
	# add_custom_command(OUTPUT generated/main.${object_name}.cpp 
	# COMMAND ${POPLITE_DIR}/parser/popgen.py ${header_name} -I${POPLITE_DIR}) ## TODO: full includes
	add_executable(main.${object_name} generated/main.${object_name}.cpp)
	target_link_libraries(main.${object_name} ${Boost_LIBRARIES})
endfunction(pop_add_object)


# add main
# pop_add_header(test_class.hpp)
pop_add_object(TestClass test_class.hpp)
add_executable(main main.cpp generated/test_class.iface.hpp)
target_link_libraries(main ${Boost_LIBRARIES})

# add object main
# Add custom target for source generation
# add_custom_target(generate ${POPLITE_DIR}/parser/pop_gen_src TestClass test_class.hpp)
# add_custom_command(OUTPUT generated/main.TestClass.cpp  COMMAND ${POPLITE_DIR}/parser/pop_gen_src TestClass test_class.hpp)
# add_custom_command(OUTPUT generated/iface.TestClass.hpp COMMAND ${POPLITE_DIR}/parser/pop_gen_src TestClass test_class.hpp)

#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	An example illustrating the use of templates for parallel classes and methods.
#
project(templates)
pop_add_object(template_ns::printer printer.hpp)
pop_add_object(template_ns::storage storage.hpp)
pop_add_object(template_ns::proxy_storage proxy_storage.hpp ${CMAKE_CURRENT_BINARY_DIR}/template_ns/storage.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/template_ns/storage.iface.cpp)
pop_add_object(template_ns::map_storage map_storage.hpp)
add_executable(main_templates main.cpp
	${CMAKE_CURRENT_BINARY_DIR}/template_ns/storage.iface.hpp
	${CMAKE_CURRENT_BINARY_DIR}/template_ns/map_storage.iface.hpp
	${CMAKE_CURRENT_BINARY_DIR}/template_ns/proxy_storage.iface.hpp
	${CMAKE_CURRENT_BINARY_DIR}/template_ns/printer.iface.hpp
	${CMAKE_CURRENT_BINARY_DIR}/template_ns/storage.iface.cpp
	${CMAKE_CURRENT_BINARY_DIR}/template_ns/map_storage.iface.cpp
	${CMAKE_CURRENT_BINARY_DIR}/template_ns/proxy_storage.iface.cpp
	${CMAKE_CURRENT_BINARY_DIR}/template_ns/printer.iface.cpp
)

target_link_libraries(main_templates ${Boost_LIBRARIES})

add_test(NAME run_templates COMMAND ./main_templates WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


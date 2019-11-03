
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

add_custom_target(run_templates ./main_templates DEPENDS main_templates template_ns.storage.obj template_ns.map_storage.obj template_ns.proxy_storage.obj template_ns.printer.obj)


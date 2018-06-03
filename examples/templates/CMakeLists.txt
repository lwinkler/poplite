include_directories(gen)

pop_add_object(template_ns::printer printer.hpp)
pop_add_object(template_ns::storage storage.hpp)
pop_add_object(template_ns::proxy_storage proxy_storage.hpp gen/template_ns/storage.iface.hpp gen/template_ns/storage.iface.cpp)
pop_add_object(template_ns::map_storage map_storage.hpp)
add_executable(main_templates main.cpp
	gen/template_ns/storage.iface.hpp gen/template_ns/map_storage.iface.hpp gen/template_ns/proxy_storage.iface.hpp gen/template_ns/printer.iface.hpp
	gen/template_ns/storage.iface.cpp gen/template_ns/map_storage.iface.cpp gen/template_ns/proxy_storage.iface.cpp gen/template_ns/printer.iface.cpp
)

target_link_libraries(main_templates ${Boost_LIBRARIES})

add_custom_target(run_templates ./main_templates DEPENDS main_templates template_ns.storage.obj template_ns.map_storage.obj template_ns.proxy_storage.obj template_ns.printer.obj)


include_directories(gen)

pop_add_template_object(template_ns::storage storage.hpp "int;std::string")
add_executable(main_templates main.cpp gen/template_ns/storage.iface.hpp)

target_link_libraries(main_templates ${Boost_LIBRARIES})

add_custom_target(run_templates ./main_templates DEPENDS main_templates template_ns.storage.obj)


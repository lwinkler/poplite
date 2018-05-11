include_directories(gen)

pop_add_objects("global_ns::child_ns::child;global_ns::parent_a;global_ns::parent_b_ns::parent_b" "child.hpp")

add_executable(main_inheritance main.cpp gen/global_ns/parent_a.iface.hpp gen/global_ns/parent_b_ns/parent_b.iface.hpp gen/global_ns/child_ns/child.iface.hpp)

target_link_libraries(main_inheritance ${Boost_LIBRARIES})

add_custom_target(run_inheritance ./main_inheritance DEPENDS main_inheritance)

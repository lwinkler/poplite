include_directories(gen)

pop_add_objects("child_ns::child;parent_a;parent_b_ns::parent_b" "child.hpp")

add_executable(main_inheritance main.cpp gen/parent_a.iface.hpp gen/parent_b_ns__parent_b.iface.hpp gen/child_ns__child.iface.hpp)

target_link_libraries(main_inheritance ${Boost_LIBRARIES})

add_custom_target(run_inheritance ./main_inheritance DEPENDS main_inheritance)

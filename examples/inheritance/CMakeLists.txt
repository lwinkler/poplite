project(poplite_getset_example)

include_directories(gen)

# TODO: Multiple classes in one header: rename hpp files with class name
pop_add_objects("child;parent_a;parent_b" "child.hpp")
add_executable(main_inheritance main_inheritance.cpp gen/parent_a.iface.hpp gen/parent_b.iface.hpp gen/child.iface.hpp)

target_link_libraries(main_inheritance ${Boost_LIBRARIES})


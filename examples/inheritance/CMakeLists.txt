project(poplite_getset_example)

include_directories(gen)

# TODO: Multiple classes in one header: rename hpp files with class name
pop_add_object(parent_a parent_a.hpp)
pop_add_object(parent_b parent_b.hpp gen/parent_a.iface.hpp)
pop_add_object(child child.hpp gen/parent_b.iface.hpp)
add_executable(main_inheritance main_inheritance.cpp gen/parent_a.iface.hpp gen/parent_b.iface.hpp gen/child.iface.hpp)

target_link_libraries(main_inheritance ${Boost_LIBRARIES})


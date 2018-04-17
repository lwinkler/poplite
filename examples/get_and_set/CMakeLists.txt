project(poplite_getset_example)

include_directories(gen)

pop_add_object(TestClass test_class.hpp)
pop_add_object(TestChildClass test_child_class.hpp gen/test_class.iface.hpp)
add_executable(main main.cpp gen/test_class.iface.hpp gen/test_child_class.iface.hpp)

target_link_libraries(main ${Boost_LIBRARIES})


project(poplite_getset_example)

include_directories(gen)

# pop_add_object(TestClass test_class.hpp)
# pop_add_object(TestChildClass test_child_class.hpp gen/TestClass.iface.hpp)
pop_add_objects("TestChildClass;TestClass" "test_child_class.hpp;test_class.hpp")
add_executable(main main.cpp gen/TestClass.iface.hpp gen/TestChildClass.iface.hpp)

target_link_libraries(main ${Boost_LIBRARIES})


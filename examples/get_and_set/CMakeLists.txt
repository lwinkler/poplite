project(poplite_getset_example)

include_directories(generated)

pop_add_object(TestClass test_class.hpp)
add_executable(main main.cpp generated/test_class.iface.hpp)
target_link_libraries(main ${Boost_LIBRARIES})


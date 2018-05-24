project(popc_tests_stdlib)

include_directories(gen)

pop_add_object(TestMethod method.hpp method.cc)
add_executable(main_stdlib main.cc gen/TestMethod.iface.hpp)
target_link_libraries(main_stdlib ${Boost_LIBRARIES})

add_custom_target(run_stdlib ./main_stdlib DEPENDS main_stdlib TestMethod.obj)


project(popc_tests_stresstest)

include_directories(gen)

pop_add_object(stress_test::POPObject POPObject.hpp POPObject.cc)
add_executable(main_stresstest main.cc gen/stress_test/POPObject.iface.hpp)
target_link_libraries(main_stresstest ${Boost_LIBRARIES})

add_custom_target(run_stresstest ./main_stresstest 500 DEPENDS main_stresstest stress_test.POPObject.obj)


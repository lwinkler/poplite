project(popc_tests_stresstest)

pop_add_object(stress_test::POPObject POPObject.hpp POPObject.cc)
add_executable(main_stresstest main.cc ${CMAKE_CURRENT_BINARY_DIR}/stress_test/POPObject.iface.hpp)
target_link_libraries(main_stresstest ${Boost_LIBRARIES})

add_custom_target(run_stresstest ./main_stresstest 200 DEPENDS main_stresstest stress_test.POPObject.obj)


project(popc_tests_stresstest)

include_directories(gen)

pop_add_object(POPObject2 POPObject.hpp POPObject.cc)
add_executable(main_stresstest main.cc gen/POPObject2.iface.hpp)
target_link_libraries(main_stresstest ${Boost_LIBRARIES})

add_custom_target(run_stresstest ./main_stresstest DEPENDS main_stresstest)


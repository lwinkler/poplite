project(popc_tests_bigdata)

include_directories(gen)

pop_add_object(POPObject POPObject.hpp)
add_executable(main_bigdata main.cc gen/POPObject.iface.hpp)
target_link_libraries(main_bigdata ${Boost_LIBRARIES})

add_custom_target(run_bigdata ./main_bigdata DEPENDS main_bigdata POPObject.obj)


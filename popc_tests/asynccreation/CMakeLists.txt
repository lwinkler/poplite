project(popc_tests_asynccreation)

include_directories(gen)

pop_add_object(POPObject3 POPObject.hpp POPObject.cc)
add_executable(main_asynccreation main.cc gen/POPObject3.iface.hpp)
target_link_libraries(main_asynccreation ${Boost_LIBRARIES})

add_custom_target(run_asynccreation ./main_asynccreation DEPENDS main_asynccreation)


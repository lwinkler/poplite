project(popc_tests_callback)

include_directories(gen)

pop_add_objects("Titi;Toto" "titi.hpp;titi.cc;toto.hpp;toto.cc")
add_executable(main_callback main.cc gen/Titi.iface.hpp gen/Toto.iface.hpp)
target_link_libraries(main_callback ${Boost_LIBRARIES})

add_custom_target(run_callback ./main_callback DEPENDS main_callback Titi.obj Toto.obj)


project(popc_tests_callback)

pop_add_objects("Titi;Toto" "titi.hpp;titi.cc;toto.hpp;toto.cc")
add_executable(main_callback main.cc ${CMAKE_CURRENT_BINARY_DIR}/Titi.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/Toto.iface.hpp)
target_link_libraries(main_callback ${Boost_LIBRARIES})

add_custom_target(run_callback ./main_callback DEPENDS main_callback Titi.obj Toto.obj)


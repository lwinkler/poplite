project(tests_default_values)

pop_add_object(tester_default tester_default.hpp)
add_executable(main_default_values main.cpp ${CMAKE_CURRENT_BINARY_DIR}/tester_default.iface.hpp)
target_link_libraries(main_default_values ${Boost_LIBRARIES})

add_custom_target(run_default_values ./main_default_values DEPENDS main_default_values tester_default.obj)


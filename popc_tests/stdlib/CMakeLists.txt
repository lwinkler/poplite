project(popc_tests_stdlib)

pop_add_object(TestMethod method.hpp method.cc)
add_executable(main_stdlib main.cc ${CMAKE_CURRENT_BINARY_DIR}/TestMethod.iface.hpp)
target_link_libraries(main_stdlib ${Boost_LIBRARIES})

add_custom_target(run_stdlib ./main_stdlib DEPENDS main_stdlib TestMethod.obj)


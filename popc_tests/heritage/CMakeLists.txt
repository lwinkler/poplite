project(popc_tests_heritage)

pop_add_objects("Child;Mother" "child.hpp;child.cc;mother.hpp;mother.cc")
add_executable(main_heritage main.cc ${CMAKE_CURRENT_BINARY_DIR}/Child.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/Mother.iface.hpp)
target_link_libraries(main_heritage ${Boost_LIBRARIES})

add_custom_target(run_heritage ./main_heritage DEPENDS main_heritage Child.obj Mother.obj)


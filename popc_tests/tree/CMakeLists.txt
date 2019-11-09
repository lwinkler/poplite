project(popc_tests_tree)

pop_add_object(Tree tree.hpp tree.cc)
add_executable(main_tree main.cc ${CMAKE_CURRENT_BINARY_DIR}/Tree.iface.hpp)
target_link_libraries(main_tree ${Boost_LIBRARIES})

add_custom_target(run_tree ./main_tree 3 4 DEPENDS main_tree Tree.obj)


project(popc_tests_tree)

include_directories(gen)

pop_add_object(Tree tree.hpp tree.cc)
add_executable(main_tree main.cc gen/Tree.iface.hpp)
target_link_libraries(main_tree ${Boost_LIBRARIES})

add_custom_target(run_tree ./main_tree DEPENDS main_tree 3 4)


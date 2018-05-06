project(popc_tests_heritage)

include_directories(gen)

pop_add_objects("Child;Mother" "child.hpp;child.cc;mother.hpp;mother.cc")
add_executable(main_heritage main_heritage.cc gen/Child.iface.hpp gen/Mother.iface.hpp)
target_link_libraries(main_heritage ${Boost_LIBRARIES})

add_custom_target(run_heritage ./main_heritage DEPENDS main_heritage)


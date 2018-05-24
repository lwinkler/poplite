project(popc_tests_exception)

include_directories(gen)

#pop_add_objects("ClassExcep" "ClassExcep.hpp;ClassExcep.cpp;IOExceptions.cc")
add_library(popc_tests_exception ClassExcep.cpp IOExceptions.cc gen/ClassExcep.iface.hpp)
pop_add_object(ClassExcep ClassExcep.hpp)
target_link_libraries(ClassExcep.obj popc_tests_exception)
add_executable(main_exception main.cc gen/ClassExcep.iface.hpp)

target_link_libraries(main_exception ${Boost_LIBRARIES} popc_tests_exception)

add_custom_target(run_exception ./main_exception DEPENDS main_exception ClassExcep.obj)


project(popc_tests_asynccreation)

pop_add_object(PopObject3 PopObject.h PopObject.cpp)
add_executable(main_asynccreation main.cpp ${CMAKE_CURRENT_BINARY_DIR}/PopObject3.iface.hpp)
target_link_libraries(main_asynccreation ${Boost_LIBRARIES})

add_custom_target(run_asynccreation ./main_asynccreation DEPENDS main_asynccreation PopObject3.obj)


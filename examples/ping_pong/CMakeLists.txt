
# pop_add_object(ping_pong ping_pong.hpp ping_pong.cpp)
pop_add_object(pop::mutex mutex.hpp)
pop_add_object(ping_pong ping_pong.hpp ping_pong.cpp ${CMAKE_CURRENT_BINARY_DIR}/pop/mutex.iface.hpp)
# pop_add_all_classes(mutex.hpp)
# pop_add_all_classes(ping_pong.hpp ping_pong.cpp ${CMAKE_CURRENT_BINARY_DIR}/pop/mutex.iface.hpp)
add_executable(main_ping main.cpp ${CMAKE_CURRENT_BINARY_DIR}/ping_pong.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/pop/mutex.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/pop/mutex.brok.cpp)
target_link_libraries(main_ping ${Boost_LIBRARIES})

add_custom_target(run_ping_pong ./main_ping async 100 localhost --pop-path `pwd` DEPENDS main_ping ping_pong.obj pop.mutex.obj)

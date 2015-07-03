project(poplite_getset_example)

include_directories(gen)

pop_add_object(ping_pong ping_pong.hpp ping_pong.cpp)
add_executable(main_ping main_ping.cpp gen/ping_pong.iface.hpp)
target_link_libraries(main_ping ${Boost_LIBRARIES})


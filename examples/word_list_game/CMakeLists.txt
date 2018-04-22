project(poplite_world_list_game)

include_directories(gen)

find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log filesystem program_options REQUIRED )
include_directories(${Boost_INCLUDE_DIR})

# pop_add_object(server server.hpp server.cpp gen/client.iface.hpp)
# pop_add_object(client client.hpp client.cpp gen/server.iface.hpp)
pop_add_objects("client;server" "client.hpp;client.cpp;server.hpp;server.cpp")
add_executable(word_list_game word_list_game.cpp gen/client.iface.hpp gen/server.iface.hpp)
target_link_libraries(word_list_game ${Boost_LIBRARIES})

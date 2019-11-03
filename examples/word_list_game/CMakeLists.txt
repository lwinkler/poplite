
find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log filesystem program_options REQUIRED )
include_directories(${Boost_INCLUDE_DIR})

# pop_add_object(server server.hpp server.cpp ${CMAKE_CURRENT_BINARY_DIR}/client.iface.hpp)
# pop_add_object(client client.hpp client.cpp ${CMAKE_CURRENT_BINARY_DIR}/server.iface.hpp)
pop_add_objects("client;server" "client.hpp;client.cpp;server.hpp;server.cpp")
add_executable(word_list_game word_list_game.cpp ${CMAKE_CURRENT_BINARY_DIR}/client.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/server.iface.hpp)
target_link_libraries(word_list_game ${Boost_LIBRARIES})

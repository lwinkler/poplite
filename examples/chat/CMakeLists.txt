include_directories(gen)

# pop_add_object(chat_client chat_client.hpp chat_client.cpp)
pop_add_objects("chat_client" "chat_client.hpp;chat_client.cpp;gen/chat_client.iface.hpp")
# pop_add_objects("chat_client" "chat_client.hpp")
add_executable(main_chat main.cpp gen/chat_client.iface.hpp)
target_link_libraries(main_chat ${Boost_LIBRARIES})


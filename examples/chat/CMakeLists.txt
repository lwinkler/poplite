project(poplite_getset_example)

include_directories(gen)

pop_add_object(chat_client chat_client.hpp chat_client.cpp)
add_executable(main.chat main.chat.cpp gen/chat_client.iface.hpp)
target_link_libraries(main.chat ${Boost_LIBRARIES})


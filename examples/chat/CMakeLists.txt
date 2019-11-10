#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	A simple chat client.
#
project(chat)

# pop_add_object(chat_client chat_client.hpp chat_client.cpp)
pop_add_objects("chat_client" "chat_client.hpp;chat_client.cpp")
# pop_add_objects("chat_client" "chat_client.hpp")
add_executable(main_chat main.cpp ${CMAKE_CURRENT_BINARY_DIR}/chat_client.iface.hpp)
target_link_libraries(main_chat ${Boost_LIBRARIES})


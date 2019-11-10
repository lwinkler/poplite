#
#	Copyright (c) Laurent Winkler 2019
#
#	Distributed under the Boost Software License, Version 1.0. (See accompanying
#	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#   
#	Objects sending notifications to each others.
#
project(ping_pong)

pop_add_object(pop::mutex mutex.hpp)
pop_add_object(ping_pong ping_pong.hpp ping_pong.cpp)
# pop_add_all_classes(mutex.hpp)
# pop_add_all_classes(ping_pong.hpp ping_pong.cpp ${CMAKE_CURRENT_BINARY_DIR}/pop/mutex.iface.hpp)
add_executable(main_ping main.cpp ${CMAKE_CURRENT_BINARY_DIR}/ping_pong.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/pop/mutex.iface.hpp ${CMAKE_CURRENT_BINARY_DIR}/pop/mutex.brok.cpp)
target_link_libraries(main_ping ${Boost_LIBRARIES})

add_test(NAME run_ping_pong COMMAND ./main_ping async 100 localhost --pop-path ${CMAKE_CURRENT_BINARY_DIR} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 


FLAGS=-std=c++11 -g -I.
LIBS=-lboost_serialization -lboost_system -lboost_thread -lboost_serialization -lboost_context -lpthread

TARGETS=com/server com/client example/main

all: ${TARGETS}
clean:
	rm -f *.o */*.o ${TARGETS}

%.o : %.cpp
	c++ ${FLAGS} -c $< -o $@

com/server : com/server.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

server_spawn : server_spawn.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

com/client : com/client.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

example/main : example/main.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

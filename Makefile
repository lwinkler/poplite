
FLAGS=-std=c++11 -g -I.
LIBS=-lboost_serialization -lboost_system -lboost_date_time -lboost_thread -lboost_serialization -lboost_coroutine -lboost_context -lpthread

TARGETS=com/server com/client

all: ${TARGETS}
clean:
	rm -f *.o ${TARGETS}

%.o : %.cpp
	c++ ${FLAGS} -c $< -o $@

com/server : com/server.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

server_spawn : server_spawn.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

com/client : com/client.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

client32 : client32.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

server32 : server32.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

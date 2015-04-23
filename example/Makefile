
FLAGS=-std=c++11 -g -I..
LIBS=-lboost_serialization -lboost_system -lboost_thread -lboost_serialization -lboost_context -lboost_log -lpthread

TARGETS=main remote_main

all: ${TARGETS}
clean:
	rm -f *.o */*.o ${TARGETS}

%.o : %.cpp
	c++ ${FLAGS} -c $< -o $@


main : main.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

remote_main : remote_main.o
	c++ ${FLAGS} $< -o $@ ${LIBS}


FLAGS=-std=c++11 -g -I. -I.. -Igenerated
LIBS=-lboost_serialization -lboost_system -lboost_thread -lboost_serialization -lboost_context -lboost_log -lpthread

TARGETS=main main.TestClass

all: ${TARGETS}
clean:
	rm -f *.o */*.o ${TARGETS}

%.o : %.cpp
	c++ ${FLAGS} -c $< -o $@


main : main.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

main.TestClass : generated/main.TestClass.o
	c++ ${FLAGS} $< -o $@ ${LIBS}

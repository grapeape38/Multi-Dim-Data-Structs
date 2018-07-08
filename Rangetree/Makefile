CXX=g++
CXXFLAGS=-std=c++11 -g
OBJS=RangetreeDriver.o RangetreeQueries.o Rangetree.o Point.o LinkedList.o


TESTOBJS=RangetreeTest.o RangetreeQueries.o Rangetree.o Point.o LinkedList.o

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o rangetree

test: $(TESTOBJS)
	$(CXX) $(CXXFLAGS) $(TESTOBJS) -o test

Rangetree.o: Rangetree.cpp RangetreeQueries.cpp LinkedList.cpp Point.cpp Rangetree.h 
RangetreeQueries.o: Rangetree.h
RangetreeDriver.o: Rangetree.cpp Point.cpp RangetreeQueries.cpp 
LinkedList.o: LinkedList.cpp Rangetree.h Point.cpp
Point.o: Point.cpp

RangetreeTest.o: RangetreeTest.cpp Point.cpp RangetreeQueries.cpp

clean:
	rm -rf *.o rangetree test

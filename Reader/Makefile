INCLUDE = -I./ -I$(shell root-config --incdir)
OPFLAGS = -O2
CFLAGS = -Wall -Wextra -Wno-unused-local-typedefs -std=c++11 $(INCLUDE) $(OPFLAGS)
LDFLAGS = $(shell root-config --libs) -lTreePlayer -lHistPainter


all: produceExampleHist

produceExampleHist: produceExampleHist.o PhysicsObjects.o Reader.o
	@ g++ $+ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.cpp
	@ g++ $(CFLAGS) -c $+ -o $@

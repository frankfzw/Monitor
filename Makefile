CXXFLAGS = -std=c++0x -Wall -g 
LDFLAGS = -std=c++0x -Wall -g
CXX = g++

TARGETS = monitor

all: $(TARGETS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

	monitor.o	parser.h property.h monitor.h monitor.cpp
	parser.o	parser.h property.h parser.cpp
	utility.o	utility.h utility.cpp

monitor:        monitor.o parser.o utility.o
	$(CXX) $(LDFLAGS) -o $@ $^ -lpcap

clean:
	rm -f *~ *.o $(TARGETS)

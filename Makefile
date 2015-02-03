CFLAGS = -Wall -g -std=c++0x
LFLAGS = -Wall -g -std=c++0x
CC = g++

TARGETS = monitor

all: $(TARGETS)

.cc.o:
	$(CC) $(CFLAGS) -c -o $@ $<

	monitor.o	parser.h property.h monitor.h monitor.cpp
	parser.o	parser.h property.h parser.cpp

monitor:        monitor.o parser.o
	$(CC) $(LFLAGS) -o $@ $^ -lpcap

clean:
	rm -f *~ *.o $(TARGETS)

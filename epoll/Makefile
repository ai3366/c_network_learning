CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

LIBS =	-pthread

server:	server.o
	$(CXX) -o server server.o $(LIBS)

all:	server

clean:
	rm -f server.o server

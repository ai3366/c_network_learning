CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		block_io_client.o

LIBS =

TARGET =	block_io_client

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		block_io.o

LIBS =	-pthread

TARGET =	block_io

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

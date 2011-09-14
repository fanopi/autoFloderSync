CXXFLAGS =	-O2 -g -Wall -lpthread -fmessage-length=0

OBJS +=		test.o
OBJS +=		threads.o

LIBS =

TARGET =	YF

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

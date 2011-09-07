CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		YF.o

LIBS =

TARGET =	YF

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

INCLUDE_DIRS = 
LIB_DIRS = 
CC=g++
DRIVER = capture

CDEFS= 
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= 
CPPLIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video -lrt -lpthread

HFILES= logging.h frame_handling.h util.h timing.h
CFILES= 
CPPFILES=  logging.cpp frame_handling.cpp util.cpp timing.cpp

SRCS= ${HFILES} ${CFILES}
CPPOBJS= ${CPPFILES:.cpp=.o}

all:	capture time

clean:
	-rm -f *.o *.d
	-rm -f capture
	-rm -r time

distclean:
	-rm -f *.o *.d

${DRIVER}: ${OBJS} 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ capture.cpp $(CPPFILES) $(OBJS) `pkg-config opencv --cflags --libs` $(CPPLIBS)

time: ${OBJS} 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ timingTest.cpp $(CPPFILES) $(OBJS) `pkg-config opencv --cflags --libs` $(CPPLIBS)

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<

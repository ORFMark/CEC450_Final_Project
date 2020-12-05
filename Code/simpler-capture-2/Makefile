INCLUDE_DIRS = 
LIB_DIRS = 
CC=g++
DRIVER = capture

CDEFS= 
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS) -s
LIBS= -lrt
CPPLIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video

HFILES= logging.h
CFILES= 
CPPFILES= capture.cpp logging.cpp frame_handling.cpp util.cpp

SRCS= ${HFILES} ${CFILES}
CPPOBJS= ${CPPFILES:.cpp=.o}

all:	capture

clean:
	-rm -f *.o *.d
	-rm -f capture

distclean:
	-rm -f *.o *.d

${DRIVER}: ${OBJS} 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(CPPFILES) $(OBJS) `pkg-config opencv --cflags --libs` $(CPPLIBS)


depend:

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<

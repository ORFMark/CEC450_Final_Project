INCLUDE_DIRS = 
LIB_DIRS = 
CC=g++
DRIVER = capture

CDEFS= 
<<<<<<< HEAD
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS) -s
LIBS= -lrt -lpthread
CPPLIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_imgcodecs -lopencv_video -lpthread -lrt
=======
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS) -lpthread -lrt
LIBS= -lrt -lpthread
CPPLIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video
>>>>>>> 33a0cad75190ecd462740fece63a11cbae4682bf

HFILES= logging.h frame_handling.h util.h
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
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(CPPFILES) $(OBJS) $(CPPLIBS)


depend:

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<

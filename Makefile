GCC=gcc
HEADERS=ds/ll/ll.h sock/csock.h sock/ssock.h sock/data.h
OBJS=ds/ll/ll.o sock/csock.o sock/ssock.o sock/data.o
FLAGS=-DAPP

sock: ${OBJS}

#%.o : %.c ${HEADERS}
#	${GCC} -c $<

ds/ll/%.o : 
	cd ds; make

sock/%.o :
	cd sock; make

clean: 
	rm -f *.o


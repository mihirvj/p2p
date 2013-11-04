GCC=gcc
DS_H=ds/ll/ll.h 
SSOCK_H=sock/ssock.h sock/data.h 
CSOCK_H=sock/csock.h sock/data.h
OBJS=ds/ll/ll.o sock/csock.o sock/ssock.o sock/data.o boot.o cpeer.o speer.o
FLAGS=-DAPP

all: ${OBJS}

objs/%.o: %.c ${DS_H} ${SSOCK_H} ${CSOCK_H}
	${GCC} -c $< 

boot: objs/boot.o ${DS_H} ${SSOCK_H}
	${GCC} boot.c -o boot

pc: objs/cpeer.o ${CSOCK_H}
	${GCC} cpeer.c -o cpeer

ps: objs/speer.o ${SSOCK_H}
	${GCC} speer.c -o speer

ds/ll/%.o : 
	cd ds; make

sock/%.o :
	cd sock; make

clean: 
	rm -f *.o


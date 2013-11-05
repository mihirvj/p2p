GCC=gcc
DS_H=ds/ll/ll.h 
SSOCK_H=sock/ssock.h sock/data.h 
CSOCK_H=sock/csock.h sock/data.h
OBJS=ds/ll/ll.o sock/csock.o sock/ssock.o sock/data.o boot.o cpeer.o speer.o
EXEC_PATH=exec

FLAGS=-DAPP

all: ${OBJS} boot ps pc
	mv *.o objs/
	mv sock/*.o sock/objs/

%.o: %.c ${DS_H} ${SSOCK_H} ${CSOCK_H}
	${GCC} -c $< -o $@

ds/ll/%.o : 
	cd ds; make

sock/%.o :
	cd sock; make

boot: boot.o ${DS_H} ${SSOCK_H}
	${GCC} boot.c -o $(EXEC_PATH)/boot

pc: cpeer.o ${CSOCK_H}
	${GCC} cpeer.c -o $(EXEC_PATH)/cpeer

ps: speer.c ${SSOCK_H}
	${GCC} speer.c -o $(EXEC_PATH)/speer

clean: 
	rm -f *.o
	rm -f objs/*.o
	cd ds; make clean
	cd sock; make clean


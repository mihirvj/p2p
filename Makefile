GCC=gcc
S_HEADERS=ds/ll/ll.h sock/ssock.h sock/data.h 
C_HEADERS=sock/csock.h sock/data.h
S_OBJS=ds/ll/ll.o sock/ssock.o sock/data.o 
C_OBJS=sock/csock.o sock/data.o
BOOT_OBJ=boot.o 
CPEER_OBJ=cpeer.o 
SPEER_OBJ=speer.o
EXEC_PATH=exec
PLIB=-lpthread

FLAGS=-DAPP

all: boot speer cpeer
	mv sock/*.o sock/objs/

boot: ${S_OBJS} ${HEADERS}
	${GCC} -o $(EXEC_PATH)/boot boot.c ${S_OBJS} $(PLIB) ${FLAGS}

speer: ${S_OBJS} ${HEADERS}
	${GCC} -o $(EXEC_PATH)/speer speer.c ${S_OBJS} ${FLAGS}

cpeer: ${C_OBJS} ${HEADERS}
	${GCC} -o $(EXEC_PATH)/cpeer cpeer.c ${C_OBJS} ${FLAGS}

%.o: %.c ${HEADERS}
	${GCC} -c $< -o $@

clean: 
	rm -f *.o
	rm -f exec/*
	cd ds; make clean
	cd sock; make clean


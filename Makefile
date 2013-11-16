GCC=gcc
S_HEADERS=ds/ll/hostll.h ds/ll/rfcll.h sock/ssock.h sock/data.h 
C_HEADERS=sock/csock.h sock/data.h sock/ssock.h
S_OBJS=ds/ll/hostll.o ds/ll/rfcll.o sock/ssock.o sock/data.o parser/parse.o 
C_OBJS=sock/csock.o sock/data.o sock/ssock.o parser/parse.o
BOOT_OBJ=boot.o 
CPEER_OBJ=cpeer.o 
EXEC_PATH=exec
PLIB=-lpthread

FLAGS=-DAPP -g -DGRAN1

all: boot cpeer
	mv sock/*.o sock/objs/
	mkdir ./exec/rfc 
	cp exec/cpeer sim/cpeer
	@echo "-----------------compilation success---------------"

boot: ${S_OBJS} ${HEADERS}
	${GCC} -o $(EXEC_PATH)/boot boot.c ${S_OBJS} $(PLIB) ${FLAGS}

#speer: ${S_OBJS} ${HEADERS}
#	${GCC} -o $(EXEC_PATH)/speer speer.c ${S_OBJS} ${FLAGS}

cpeer: ${C_OBJS} ${HEADERS}
	${GCC} -o $(EXEC_PATH)/cpeer cpeer.c ${C_OBJS} $(PLIB) ${FLAGS}

%.o: %.c ${C_HEADERS} ${S_HEADERS}
	${GCC} -c $< -o $@ ${FLAGS}

clean: 
	rm -f *.o
	rm -r exec/*
	cd ds; make clean
	cd sock; make clean	
        

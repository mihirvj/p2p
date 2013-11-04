GCC=gcc
HEADERS=ll/ll.h
OBJS=ll/ll.o 
FLAGS=-DP2P

ll: ${OBJS}

%.o ll/%.o: %.c ll/%.c ${HEADERS}
	${GCC} -c $< .

clean:
	rm -f *.o

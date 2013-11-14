GCC=gcc
HEADERS=ll/hostll.h ll/rfcll.h
OBJS=ll/hosll.o ll/rfcll.o
FLAGS=-DP2P

ll: ${OBJS}

%.o ll/%.o: %.c ll/%.c ${HEADERS}
	${GCC} -c $< 

clean:
	rm -f ll/*.o

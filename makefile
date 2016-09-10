SHELL = /bin/sh
CC = gcc

y86emul:${OBJS}
	${CC} -o y86emul y86emul.c 

y86dis:${OBJS}
	${CC} -o y86dis y86dis.c 

clean:
	-rm -f *.o y86emul y86dis core *.core
	
.c.o:
	${CC} ${INCLUDES} -c $<



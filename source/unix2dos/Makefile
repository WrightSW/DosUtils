#
# makefile for unix2dos
#
#
CC= g++
#CFLAGS= -O
CFLAGS=
#LFLAGS= -static
LFLAGS=
#LIBS= -lstdc++
LIBS=

OBS= unix2dos.o

all:unix2dos

.cpp.o:
	$(CC) $(CFLAGS) -c $<

unix2dos: $(OBS)
	$(CC) -o unix2dos $(OBS) $(LFLAGS) $(LIBS)

man:
	/usr/bin/groff -Tascii -mandoc unix2dos.man >docs/unix2dos.1
	/usr/bin/col -bx <docs/unix2dos.1 >docs/unix2dos.txt
	/bin/chmod 644 docs/*

clean:
	/bin/rm -f *.o

cleanall:
	/bin/rm -f *.o unix2dos
	/bin/rm -f docs/*

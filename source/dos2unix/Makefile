#
# makefile for dos2unix
#
#
CC= g++
#CFLAGS= -O
CFLAGS=
#LFLAGS= -static
LFLAGS=
LIBS= -lstdc++

OBS= dos2unix.o

all:dos2unix

.cpp.o:
	$(CC) $(CFLAGS) -c $<

dos2unix: $(OBS)
	$(CC) -o dos2unix $(OBS) $(LFLAGS) $(LIBS)

man:
	/usr/bin/groff -Tascii -mandoc dos2unix.man >docs/dos2unix.1
	/usr/bin/col -bx <docs/dos2unix.1 >docs/dos2unix.txt
	/bin/chmod 644 docs/*
clean:
	/bin/rm -f *.o

cleanall:
	/bin/rm -f *.o dos2unix
	/bin/rm -f docs/*

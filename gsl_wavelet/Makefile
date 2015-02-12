CFLAGS = -O2 -Wall

INCL_DIR = 

LIBS = -lgsl

all: dwt

dwt: dwt.o
	gcc $(CFLAGS) -o dwt dwt.o $(LIBS)

dwt.o: dwt.c
	gcc $(CFLAGS) $(INCL_DIR) -c dwt.c 

.PHONY:
clean:
	rm -rf dwt *~ *.o


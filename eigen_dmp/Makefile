CC = g++
CFLAGS = -Wall -O2 -I/usr/include/eigen3
LIBS = -lcgi_mosaic -lcgi_logger

all: eigen_dmp

eigen_dmp: eigen_dmp.o
	$(CC) -o eigen_dmp $(CFLAGS) eigen_dmp.o $(LIBS)

eigen_dmp.o: eigen_dmp.cpp
	$(CC) -c $(CFLAGS) eigen_dmp.cpp 

clean:
	rm -f eigen_dmp *.o *~

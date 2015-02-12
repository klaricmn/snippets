CC = g++ 
OPTS=-Wall -O2
LIBS = -lboost_filesystem -lopencv_core -lopencv_objdetect

all: dmp

dmp: dmp.o openCV_dmp.o
	$(CC) -o dmp dmp.o openCV_dmp.o $(OPTS) $(LIBS)

dmp.o: dmp.cpp openCV_dmp.hpp
	$(CC) -c $(OPTS) dmp.cpp

openCV_dmp.o: openCV_dmp.cpp openCV_dmp.hpp
	$(CC) -c $(OPTS) openCV_dmp.cpp

.PHONY: clean
clean: 
	rm -f *.o *~ dmp

CXX=g++
CXXFLAGS=-Wall `pkg-config  --cflags Magick++-6.Q16 `
LIBS=`pkg-config  --libs Magick++-6.Q16 `

all: test

test: test.o
	$(CXX) -o test test.o $(LIBS)

test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c test.cpp

.PHONY:
clean: 
	rm -rf *~ *.o test *.png

CPPFLAGS = -O2 -Wall

INCL_DIR = -I /usr/include/ImageMagick-6

LIBS = -lcgi_core `pkg-config --libs Magick++`

all: jpeg_from_buffer

jpeg_from_buffer: jpeg_from_buffer.o
	g++ $(CPPFLAGS) -o jpeg_from_buffer jpeg_from_buffer.o $(LIBS)

jpeg_from_buffer.o: jpeg_from_buffer.cpp
	g++ $(CPPFLAGS) $(INCL_DIR) -c jpeg_from_buffer.cpp 

.PHONY:
clean:
	rm -rf jpeg_from_buffer *~ *.o


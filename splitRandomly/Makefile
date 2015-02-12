CC = g++
OPTS = -Wall 

APP_NAME = splitRandomly

INSTALL_DIR = "/usr/research/bin"

#

all: $(APP_NAME)

$(APP_NAME): main.o
	$(CC) $(OPTS) -o $(APP_NAME) main.o

main.o: main.cpp
	$(CC) $(OPTS) -c main.cpp

#

install: all
	cp $(APP_NAME) $(INSTALL_DIR)/$(APP_NAME)

#

clean:
	rm -f *.o *~ $(APP_NAME)
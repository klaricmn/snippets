CPPFLAGS = -O2 -Wall

INCL_DIR = 

LIBS = -Wl,--as-needed -lboost_system -lboost_thread -lboost_filesystem

APPS = accumulate cdf filesystem iter_split threadpool tokenizer

all: $(APPS)

accumulate: accumulate.o
	g++ $(CPPFLAGS) -o $@ $< $(LIBS)

cdf: cdf.o
	g++ $(CPPFLAGS) -o $@ $< $(LIBS)

filesystem: filesystem.o
	g++ $(CPPFLAGS) -o $@ $< $(LIBS)

iter_split: iter_split.o
	g++ $(CPPFLAGS) -o $@ $< $(LIBS)

threadpool: threadpool.o
	g++ $(CPPFLAGS) -o $@ $< $(LIBS)

tokenizer: tokenizer.o
	g++ $(CPPFLAGS) -o $@ $< $(LIBS)

%.o: %.cpp
	g++ $(CPPFLAGS) $(INCL_DIR) -c $<

.PHONY:
clean:
	rm -rf $(APPS) *~ *.o


all: check

# A rule that runs the unit tests
check: runner
	@LD_LIBRARY_PATH="../src/.libs" ./runner

# How to build the test runner (Add new object files here as you add more tests)
runner: runner.o NodeTestSuite.o StatsTestSuite.o
	g++ -g -I /usr/local/cxxtest/include/ -I ../src/ -o $@ $^ -L ../src/.libs/ -lsst

# compile all of the generates source code files
.cpp.o:
	g++ -g -I /usr/local/cxxtest/include/ -I ../src/ -c -o $@ $<

# How to generate the test runner (this is the scaffolding that makes things go)
runner.cpp: 
	cxxtestgen.pl --error-printer --root -o $@

# turn your test files (the headers) into source code
.h.cpp:
	cxxtestgen.pl --part  $< -o $@

.PHONY:
clean:
	rm -f runner *.o

.NOTPARALLEL:

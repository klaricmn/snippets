EXAMPLE_LIBRARY_VERSION=1:0:0

lib_LTLIBRARIES = libsst.la
libsst_la_SOURCES = InternalNode.h InternalNode.cpp Node.h Node.cpp SortedSumTree.h SortedSumTree.cpp BruteForceSum.h BruteForceSum.cpp Timing.h Timing.cpp Statistics.h Statistics.cpp

libsst_la_LDFLAGS = -version-info $(EXAMPLE_LIBRARY_VERSION)

sstdir = $(includedir)/sst
sst_HEADERS = InternalNode.h Node.h SortedSumTree.h BruteForceSum.h Timing.h Statistics.h
# A simple makefile for CSE 100 P2

# We use this in order to feed in the correct version of g++
# (version 4.8) where it's installed under a different name in
# Gradescope. Change the CXX variable assignment at your own risk.
CXX ?= g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS=-g

all: autocomplete benchtrie firewall
#firewall hashStats

benchtrie: benchtrie.o util.o
	$(CXX) $(CXXFLAGS) -o benchtrie benchtrie.o util.o

autocomplete: autocomplete.o util.o
	$(CXX) $(CXXFLAGS) -o autocomplete autocomplete.o util.o

firewall: BloomFilter.o firewall.o MurmurHash3.o
	$(CXX) $(CXXFLAGS) -o firewall BloomFilter.o firewall.o MurmurHash3.o

autocomplete.o: autocomplete.cpp DictionaryTrie.hpp TNode.hpp
	$(CXX) $(CXXFLAGS) -c autocomplete.cpp

benchtrie.o: benchtrie.cpp DictionaryTrie.hpp TNode.hpp
	$(CXX) $(CXXFLAGS) -c benchtrie.cpp

BloomFilter.o: BloomFilter.cpp BloomFilter.hpp MurmurHash3.cpp MurmurHash3.h
	$(CXX) $(CXXFLAGS) -c BloomFilter.cpp

firewall.o: firewall.cpp
	$(CXX) $(CXXFLAGS) -c firewall.cpp

MurmurHash3.o: MurmurHash3.cpp MurmurHash3.h
	$(CXX) $(CXXFLAGS) -c MurmurHash3.cpp

#benchtrie: util.o DictionaryTrie.o

#hashStats: BloomFilter.o

#test: util.o DictionaryTrie.o

util.o: util.cpp util.hpp
	$(CXX) $(CXXFLAGS) -c util.cpp

clean:
	rm -f test autocomplete benchtrie firewall hashStats *.o core* *~
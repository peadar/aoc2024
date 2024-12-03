OPT ?= -O0 -pipe
CXXFLAGS += -g --std=c++2a -Wall -Wextra $(OPT) -I../include
ASFLAGS += -g --std=c++2a -Wall -Wextra $(OPT) -I../include

.PHONY: all clean
VPATH = ../lib

PARTOBJS ?= solve.o

all: solve

%.s: %.cc
	$(CXX) $(ASFLAGS) -S -o $@ $^

solve: $(PARTOBJS) aoc-main.o timeit.o
	$(CXX) -o $@ $^  -lboost_chrono
 
check: all
	./solve ../data/`basename $$PWD`.txt
time: all
	./solve -t ../data/`basename $$PWD`.txt
clean:
	rm -f *.o solve core.*

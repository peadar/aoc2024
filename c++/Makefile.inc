OPT ?= -O0 -pipe
CXXFLAGS += -g --std=c++2a -Wall -Wextra $(OPT) -I..
ASFLAGS += -g --std=c++2a -Wall -Wextra $(OPT) -I..

.PHONY: all clean
VPATH = ..

PARTOBJS ?= solve.o

all: solve

%.s: %.cc
	$(CXX) $(ASFLAGS) -S -o $@ $^

solve: $(PARTOBJS) aoc-main.o timeit.o
	$(CXX) -o $@ $^

check: all
	./solve ../../data/`basename $$PWD`.txt
time: all
	./solve -t ../../data/`basename $$PWD`.txt
clean:
	rm -f *.o solve core.*

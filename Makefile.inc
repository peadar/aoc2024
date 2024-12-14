OPT ?= -O0 -pipe
CXXFLAGS += -g --std=c++2a -Wall -Wextra $(OPT) -I../include
ASFLAGS += -g --std=c++2a -Wall -Wextra $(OPT) -I../include

.PHONY: all clean
VPATH = ../lib

PARTOBJS ?= solve.o

all: solve

%.s: %.cc
	$(CXX) $(ASFLAGS) -S -o $@ $^

solve: $(PARTOBJS) aoc-main.o timeit.o unicode.o
	$(CXX) -o $@ $^  -lboost_chrono
 
INPUT=../data/`basename $$PWD`.txt

check: all
	./solve $(INPUT)
time: all
	./solve -t $(INPUT)
clean:
	rm -f *.o solve core.*
gdb: all
	gdb --args ./solve $(INPUT)

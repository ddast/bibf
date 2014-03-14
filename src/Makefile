CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -pedantic-errors -std=c++0x
LDFLAGS=
LDLIBS=-lboost_program_options

SRCS=bibf.cpp Bibliography.cpp Constants.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: bibf

bibf: $(OBJS)
	$(CXX) $(LDFLAGS) -o bibf $(OBJS) $(LDLIBS)

%.o: %.cpp %.hpp

clean:
	rm -f $(OBJS)

dist-clean: clean
	rm -f bibf

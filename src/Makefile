#------------------------------------------------------------------------------

prefix=/usr
bindir=${prefix}/bin
binname=bibf
DESTDIR=

CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -pedantic-errors -std=c++0x
LDFLAGS=
LDLIBS=-lboost_program_options

#------------------------------------------------------------------------------

OBJS=bibf.o Bibliography.o Constants.o Parser.o Strings.o

#------------------------------------------------------------------------------

all: $(binname)

$(binname):	$(OBJS)
	$(CXX) $(LDFLAGS) -o $(binname) $(OBJS) $(LDLIBS)

bibf.o:	bibf.cpp bibf.hpp Bibliography.hpp Strings.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

Bibliography.o:	Bibliography.cpp Bibliography.hpp Constants.hpp DataStructure.hpp Parser.hpp Strings.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

Constants.o:	Constants.cpp Constants.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

Parser.o:	Parser.cpp Parser.hpp DataStructure.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

Strings.o:	Strings.cpp Strings.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

distclean:	clean
	rm -f $(binname)

install:	$(binname)
	install -d $(DESTDIR)$(bindir)
	install $(binname) $(DESTDIR)$(bindir)/$(binname)

uninstall:	
	rm $(DESTDIR)$(bindir)/$(binname)


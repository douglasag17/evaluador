SRCDIR   := src
BUILDDIR := build
INCLUDE  := include
BINDIR   := bin
LDFLAGS  := -pthread
CXXFLAGS := -pedantic-errors -Wall -Wextra -std=c++11 -I include

all: dirs $(BINDIR)/evaluator

VPATH := src include build

$(BINDIR)/evaluator: evaluator.o init.o reg.o ctrl.o rep.o
	$(CXX) -o $@ $(LDFLAGS) $^

$(BUILDDIR)/evaluator.o: evaluator.cpp init.h reg.h ctrl.h rep.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BUILDDIR)/init.o: init.cpp init.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BUILDDIR)/reg.o: reg.cpp reg.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BUILDDIR)/ctrl.o: ctrl.cpp ctrl.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BUILDDIR)/rep.o: rep.cpp rep.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -f $(SRCDIR)/*.cpp~ $(INCLUDE)/*.h~ makefile~ *.o
	rm -f -R $(BUILDDIR)
	rm -f -R $(BINDIR)

.PHONY: dirs
dirs:
	@test -d build || mkdir build
	@test -d bin || mkdir bin
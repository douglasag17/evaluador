SRCDIR   := src
BINDIR   := bin
CXXFLAGS := -std=c++11 -I src
#-pedantic-errors -Wall -Wextra 
all: dirs $(BINDIR)/evaluator

VPATH := src bin

$(SRCDIR)/init.o: init.cpp init.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(SRCDIR)/reg.o: reg.cpp reg.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(SRCDIR)/ctrl.o: ctrl.cpp ctrl.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(SRCDIR)/rep.o: rep.cpp rep.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BINDIR)/evaluator: evaluator.o init.o reg.o ctrl.o rep.o
	$(CXX) -o $@ $^
	
$(SRCDIR)/evaluator.o: evaluator.cpp init.h reg.h ctrl.h rep.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -f $(SRCDIR)/*.cpp~ $(SRCDIR)/*.h~ makefile~ $(SRCDIR)/*.o
	rm -f -R $(BINDIR)

.PHONY: dirs
dirs:
	@test -d bin || mkdir bin
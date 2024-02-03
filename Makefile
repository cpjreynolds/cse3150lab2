CXXFLAGS=-Wall -g --std=c++17

# testing target
TESTTARGET=lab2test.out
# runnable target
RUNTARGET=lab2.out

# all source files including test
SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)

# only the regular main file
#RSOURCES:=$(filter-out %.test.cpp,$(SOURCES))
# only the testing main file
#TSOURCES:=$(filter-out lab2.cpp,$(SOURCES))

.PHONY: all clean check run

all: $(RUNTARGET) $(TESTTARGET)

check: $(TESTTARGET)
	./$(TESTTARGET)

run: $(RUNTARGET)
	./$(RUNTARGET)

$(TESTTARGET): $(SOURCES)
	$(CXX) $(CPPFLAGS) -DTESTING $(CXXFLAGS) $^ -o $@

$(RUNTARGET): $(SOURCES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf \
		$(OBJECTS)					\
		$(RUNTARGET)				\
		$(RUNTARGET:.out=.out.dSYM)	\
		$(TESTTARGET)				\
		$(TESTTARGET:.out=.out.dSYM)

CXXFLAGS=-Wall -g --std=c++17

# testing target
TESTTARGET=lab2test.out
# runnable target
RUNTARGET=lab2.out

# all source files including test
SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)

# only the regular main file
ROBJECTS:=$(filter-out lab2.test.o,$(OBJECTS))
# only the testing main file
TOBJECTS:=$(filter-out lab2.o,$(OBJECTS))

.PHONY: all clean check run

all: $(RUNTARGET) $(TESTTARGET)

check: $(TESTTARGET)
	./$(TESTTARGET)

run: $(RUNTARGET)
	./$(RUNTARGET)

$(TESTTARGET): $(TOBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(RUNTARGET): $(ROBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

clean:
	rm -f \
		$(OBJECTS)		\
		$(RUNTARGET)	\
		$(TESTTARGET)

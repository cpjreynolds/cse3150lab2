CXXFLAGS=-Wall -g --std=c++17

TARGET=lab2.out

SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)

.PHONY: all clean check

all: $(TARGET)

check: all
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@


clean:
	rm -f \
		$(OBJECTS)	\
		$(TARGET)

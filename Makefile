BDIR = build
SDIR = src
IDIR = include
ODIR = obj
TARGET = $(BDIR)/build-0
LDLIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CXX = g++
CXXFLAGS = -g -I$(IDIR)/ -I$(SDIR)/ -fPIC -Wall -Werror -Wextra -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -O2

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cpp, $(ODIR)/%.o, $(wildcard */*.cpp))
HEADERS = $(wildcard */*.h)

$(ODIR)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS)  $(LDLIBS) -o $@

clean:
	-rm -f obj/*.o
	-rm -f $(TARGET)

BDIR = build
SDIR = src
IDIR = include
ODIR = obj
TARGET = $(BDIR)/$(buildname)
LDLIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CXX = g++
CXXFLAGS = -I$(IDIR)/ -I$(SDIR)/ -g -fPIC -Wall -Werror -Wextra -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -O2

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c*, $(ODIR)/%.o, $(wildcard */*.c*))
HEADERS = $(wildcard */*.h)

$(ODIR)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS)  $(LDLIBS) -o $@

clean:
	-rm -f $(TARGET)

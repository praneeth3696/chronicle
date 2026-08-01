CXX = g++
CXXFLAGS = -Wall -g -std=c++20 -Iinclude
TARGET = chronicle

SRCS = src/main.cpp src/console.cpp src/input_buffer.cpp src/command.cpp src/pager.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) ironhold.world

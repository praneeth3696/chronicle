CXX=g++
CXXFLAGS= -Wall -g -Iinclude
TARGET = chronicle

all:$(TARGET)

$(TARGET): src/main.cpp src/console.cpp src/input_buffer.cpp src/command.cpp
		$(CXX) $(CXXFLAGS) src/main.cpp src/console.cpp src/input_buffer.cpp src/command.cpp -o $(TARGET)

clean:
		rm -f $(TARGET)

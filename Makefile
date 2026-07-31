CXX=g++
CXXFLAGS= -Wall -g
TARGET = chronicle

all:$(TARGET)

$(TARGET): src/main.cpp src/console.cpp src/input_buffer.cpp
		$(CXX) $(CXXFLAGS) src/main.cpp src/console.cpp src/input_buffer.cpp -o $(TARGET)

clean:
		rm -f $(TARGET)

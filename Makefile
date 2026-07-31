CXX=g++
CXXFLAGS= -Wall -g
TARGET = chronicle

all:$(TARGET)

$(TARGET): src/main.cpp src/console.cpp
		$(CXX) $(CXXFLAGS) src/main.cpp src/console.cpp -o $(TARGET)

clean:
		rm -f $(TARGET)

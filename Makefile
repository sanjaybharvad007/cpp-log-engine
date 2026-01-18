CXX = g++
CXXFLAGS = -std=c++17 -pthread -O3 -Wall
SRC = src/main.cpp
TARGET = log_engine

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

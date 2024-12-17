CXX := g++
CXXFLAGS := -std=c++11 -O2 -pthread

SRC := array_summary.cpp
TARGET := array-summary

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

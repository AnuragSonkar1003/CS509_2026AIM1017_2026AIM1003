CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -std=c++17

all: assignment4

assignment4: src/main.cpp src/kmeans.cpp src/fastmap.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f assignment4

.PHONY: all clean

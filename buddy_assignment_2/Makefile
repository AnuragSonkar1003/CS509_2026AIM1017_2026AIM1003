CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -std=c++17

SRC = src/main.cpp src/undirected_csr.cpp src/triangle_counting.cpp \
      src/betweenness_centrality.cpp src/connected_components.cpp \
      src/test_runner.cpp

all: assignment2 graphgen

assignment2: $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

graphgen: tools/graph_gen.cpp
	$(CXX) $(CXXFLAGS) -o $@ tools/graph_gen.cpp

clean:
	rm -f assignment2 graphgen

.PHONY: all clean

CXX      = g++
CXXFLAGS = -O2 -Wall -Wextra -std=c++17

SRC      = src/main.cpp src/csr.cpp src/gd.cpp src/gd_io.cpp src/maxflow.cpp src/maxflow_io.cpp
TARGET   = assignment3_buddy.exe

GEN_SRC    = tools/maxflow_gen.cpp
GEN_TARGET = maxflow_gen.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

gen: $(GEN_TARGET)

$(GEN_TARGET): $(GEN_SRC)
	$(CXX) $(CXXFLAGS) -o $(GEN_TARGET) $(GEN_SRC)

clean:
	rm -f $(TARGET) $(GEN_TARGET)

.PHONY: all gen clean

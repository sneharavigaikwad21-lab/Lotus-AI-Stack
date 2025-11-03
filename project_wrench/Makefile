# Makefile for the Gemmini Conv2D kernel ("The Inspector")

TARGET = src/gemmini_conv2d.o
SRC = src/gemmini_conv2d.cpp
CXX = riscv64-unknown-elf-g++
CXXFLAGS = -O3 -march=rv64gcv

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

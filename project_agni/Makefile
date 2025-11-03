# Makefile for the Mamba Block kernel ("The Inspector")

TARGET = src/mamba_block.o
SRC = src/mamba_block.cpp
CXX = riscv64-unknown-elf-g++
CXXFLAGS = -O3 -march=rv64gcv

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

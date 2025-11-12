# PROJECT AGNI MAKEFILE
# RISC-V 64-bit target, zero warnings, production-ready

CC := gcc
CXX := g++
CFLAGS := -std=c11 -Wall -Wextra -Werror -O2 -g -march=rv64g
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -O2 -g -march=rv64g
LDFLAGS := -pthread -lm

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

# Source files
SOURCES := $(filter-out $(SRC_DIR)/test_v45.cpp, $(wildcard $(SRC_DIR)/*.cpp))
HEADERS := $(wildcard $(SRC_DIR)/*.h) $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Target
TARGET := agni_mind.elf

# Default target
all: clean $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✅ Build successful: $(TARGET)"
	@echo "Build info:"
	@file $(TARGET)
	@ls -lh $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all run clean

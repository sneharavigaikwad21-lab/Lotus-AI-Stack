#
# PROJECT AGNI (V37 "ZERO-BUG" FIX) - MASTER BUILD SYSTEM
# Forged by: Integration Architect (The Foundry)
# Status: V37 "HARD GO" - Corrects "Goliath Trap" (Bug #2: Missing files/includes)
#

# --- 1. Toolchain Definition (Golden Knowledge) ---
CC        = riscv64-unknown-elf-gcc
OBJCOPY   = riscv64-unknown-elf-objcopy
READELF   = riscv64-unknown-elf-readelf

# --- 2. Compiler Flags (Golden Knowledge) ---
# CFLAGS are for C and ASM
CFLAGS    = -march=rv64gc -mabi=lp64d -ffreestanding -O2 -g
# CXXFLAGS are for C++
CXXFLAGS  = $(CFLAGS) -std=c++17
# LDFLAGS are for linking
LDFLAGS   = -T project_agni.ld -nostdlib -Wl,-Map=agni_mind.map

# --- 3. V37 "GOLDEN KNOWLEDGE" FIX ---
# "THE" "GOLDEN" "INCLUDE" "PATH" (Mandate V37.2)
# This allows C++ code to find "gemmini_rocc.h" and "noc_hal.h"
CXXFLAGS += -I Agni_wrench

# --- 4. Source Files (V37 Foundation Fix) ---
# "crt0.S" is GOLDEN
SRC_ASM   = crt0.S

# "THE" "GOLDEN" "SOURCE" "LIST" (Mandate V37.2)
SRC_CPP   = main.cpp \
            scheduler.cpp \
            mamba_block.cpp

OBJ_ASM   = $(SRC_ASM:.S=.o)
OBJ_CPP   = $(SRC_CPP:.cpp=.o)

# --- 5. "Stitching" Interface Files (GOLDEN) ---
MODEL_BIN_WRENCH = iree_wrench_model.bin
MODEL_BIN_KEY    = iree_key_model.bin

MODEL_OBJ_WRENCH = $(MODEL_BIN_WRENCH:.bin=.o)
MODEL_OBJ_KEY    = $(MODEL_BIN_KEY:.bin=.o)

# List of all objects to be linked
LINK_OBJS = $(OBJ_ASM) $(OBJ_CPP) $(MODEL_OBJ_WRENCH) $(MODEL_OBJ_KEY)

# --- 6. Target ---
TARGET    = agni_mind.elf

# --- 7. Main Build Rules (GOLDEN) ---
all: $(TARGET)

$(TARGET): $(LINK_OBJS) project_agni.ld
	@echo "LD (Stitching) -> $@"
	$(CC) $(LINK_OBJS) $(LDFLAGS) -o $@

# --- 8. Compilation Rules (GOLDEN) ---
%.o: %.S
	@echo "AS -> $@"
	$(CC) $(CFLAGS) -c $< -o $@

# (V37 FIX: Use CXXFLAGS for C++ files)
%.o: %.cpp
	@echo "CC -> $@"
	$(CC) $(CXXFLAGS) -c $< -o $@

# --- 9. "THE STITCHING" Rules (GOLDEN) ---
$(MODEL_OBJ_WRENCH): $(MODEL_BIN_WRENCH)
	@echo "STITCH (Wrench/IREE) -> $@"
	$(OBJCOPY) -I binary -O elf64-littleriscv \
	--rename-section .data=.wrench_model_data \
	$< $@
	@echo "VERIFY (Wrench): Checking .wrench_model_data section..."
	@$(READELF) -S $@ | grep -q ".wrench_model_data" || \
	  (echo "ERROR: V37 STITCHING FAILED!"; exit 1)

$(MODEL_OBJ_KEY): $(MODEL_BIN_KEY)
	@echo "STITCH (Key/IREE) -> $@"
	$(OBJCOPY) -I binary -O elf64-littleriscv \
	--rename-section .data=.key_model_data \
	$< $@
	@echo "VERIFY (Key): Checking .key_model_data section..."
	@$(READELF) -S $@ | grep -q ".key_model_data" || \
	  (echo "ERROR: V37 STITCHING FAILED!"; exit 1)

# --- 10. Utility Rules ---
clean:
	@echo "CLEAN"
	rm -f $(TARGET) $(OBJ_ASM) $(OBJ_CPP) $(MODEL_OBJ_WRENCH) $(MODEL_OBJ_KEY) *.map

.PHONY: all clean

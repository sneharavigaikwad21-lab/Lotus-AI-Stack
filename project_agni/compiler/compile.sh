#!/bin/bash

# This script represents a realistic "Multi-Stage Stitching" (V6) compilation
# pipeline for the "Mamba S6" kernel.

iree-compile \\
  --iree-hal-target-backends=llvm-cpu \\
  --iree-llvm-target-triple=riscv64-unknown-linux-gnu \\
  --iree-llvm-target-cpu-features="+v" \\
  --iree-hal-executable-linker-script=../kernel/linker.ld \\
  --iree-input-type=mhlo \\
  --iree-vm-bytecode-module-strip-source-map=true \\
  --iree-vm-emit-polyglot-zip=false \\
  ../kernel/mamba_s6.cpp \\
  -o ../kernel/mamba_s6.vmfb

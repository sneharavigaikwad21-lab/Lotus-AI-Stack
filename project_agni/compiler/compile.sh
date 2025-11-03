#!/bin/bash

# This script represents the "Multi-Stage Stitching" (V6) compilation
# pipeline for the "Mamba S6" kernel.

iree-compile \\
  --iree-hal-target-backends=riscv \\
  --iree-riscv-target-cpu=key \\
  --iree-riscv-v-select-vector-size=1024 \\
  --iree-hal-executable-linker-script=../kernel/linker.ld \\
  --iree-input-type=mhlo \\
  --iree-vm-bytecode-module-strip-source-map=true \\
  --iree-vm-emit-polyglot-zip=false \\
  --iree-preprocessing-pass-pipeline="iree-pre-stitch,iree-custom-fusion,iree-vector-dispatch" \\
  --iree-codegen-pass-pipeline="iree-vector-tiling,iree-buffer-stitching,iree-final-vector-codegen" \\
  ../kernel/mamba_s6.cpp \\
  -o ../kernel/mamba_s6.vmfb

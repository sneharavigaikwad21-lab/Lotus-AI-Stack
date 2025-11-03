import tvm
from tvm import te

# Define the Gemmini target
gemmini_target = tvm.target.Target(
    "llvm -mtriple=riscv64-unknown-linux-gnu -mcpu=rocket -mattr=+gemmini"
)

# Define the memory layout of the scratchpad
gemmini_scratchpad_layout = {
    "width": 8,
    "height": 8,
    "depth": 256,
}

# Register the Gemmini target
tvm.target.register_target("gemmini", gemmini_target)

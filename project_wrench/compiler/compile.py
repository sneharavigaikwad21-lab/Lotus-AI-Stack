import tvm
from tvm import te
from tvm.relay.backend import Executor, Runtime
from tvm.relay.op.contrib import ilaflex

# Import the Gemmini target definition
from gemmini_target import gemmini_target

# Define the interface for the extern C++ module
@tvm.register_func("tvm.contrib.lotus_sight.conv2d")
def lotus_sight_conv2d(input, weight):
    # This is a TVM extern function that calls the C++ kernel
    return tvm.tir.call_extern("int32", "LotusSight_conv2d", input.data, weight.data)

@tvm.register_func("tvm.contrib.lotus_sight.matmul")
def lotus_sight_matmul(input, weight):
    # This is a TVM extern function that calls the C++ kernel
    return tvm.tir.call_extern("int32", "LotusSight_matmul", input.data, weight.data)

# Create a Relay graph that uses the extern functions
x = tvm.relay.var("x", shape=(1, 1, 8, 8), dtype="int8")
w = tvm.relay.var("w", shape=(1, 1, 3, 3), dtype="int8")
y = tvm.relay.Call(tvm.relay.op.get("tvm.contrib.lotus_sight.conv2d"), [x, w])
z = tvm.relay.Call(tvm.relay.op.get("tvm.contrib.lotus_sight.matmul"), [y, w])
module = tvm.IRModule.from_expr(z)

# Compile the module for the Gemmini target
with tvm.transform.PassContext(opt_level=3):
    lib = tvm.relay.build(module, target=gemmini_target)

# Save the compiled module
lib.export_library("../kernel/lotus_sight.so")

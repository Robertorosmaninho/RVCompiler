// RUN: %RVCompiler %inputs/overflow.rv --jit | FileCheck %s
// CHECK: -9223372036854775808
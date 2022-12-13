// RUN: %RVCompiler %inputs/overflow.rv
// RUN: lli overflow.ll | FileCheck %s

// XFAIL: *

// CHECK: -9223372036854775808
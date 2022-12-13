// RUN: %RVCompiler %inputs/complex.rv
// RUN: lli complex.ll | FileCheck %s
// XFAIL: *
// CHECK: 13

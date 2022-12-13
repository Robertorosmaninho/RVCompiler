// RUN: %RVCompiler %inputs/int64Max.rv
// RUN: lli int64Max.ll | FileCheck %s
// XFAIL: *


// CHECK: 9223372036854775807
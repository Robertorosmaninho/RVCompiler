// RUN: %RVCompiler %inputs/division.rv && FileCheck %s < division.ll

// CHECK: ; ModuleID = 'division.rv'
// CHECK-NEXT: source_filename = "division.rv"
// CHECK-EMPTY:
// CHECK-NEXT: @0 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
// CHECK-EMPTY:
// CHECK-NEXT: define void @main() {
// CHECK-NEXT: entry:
// CHECK-NEXT: %divtmp = sdiv i64 4, 2
// CHECK-NEXT: %calltmp = call i64 (ptr, ...) @printf(ptr @0, i64 %divtmp)
// CHECK-NEXT: ret void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: declare i64 @printf(ptr, ...)

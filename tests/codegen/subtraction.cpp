// RUN: %RVCompiler %inputs/subtraction.rv && FileCheck %s < subtraction.ll

// CHECK: ; ModuleID = 'subtraction.rv'
// CHECK-NEXT: source_filename = "subtraction.rv"
// CHECK-EMPTY:
// CHECK-NEXT: @0 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
// CHECK-EMPTY:
// CHECK-NEXT: define void @main() {
// CHECK-NEXT: entry:
// CHECK-NEXT: %subtmp = sub i64 10, 5
// CHECK-NEXT: %calltmp = call i64 (ptr, ...) @printf(ptr @0, i64 %subtmp)
// CHECK-NEXT: ret void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: declare i64 @printf(ptr, ...)

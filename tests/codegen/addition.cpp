// RUN: %RVCompiler %inputs/addition.rv && FileCheck %s < addition.ll

// CHECK: ; ModuleID = 'addition.rv'
// CHECK-NEXT: source_filename = "addition.rv"
// CHECK-EMPTY:
// CHECK-NEXT: @0 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
// CHECK-EMPTY:
// CHECK-NEXT: define void @main() {
// CHECK-NEXT: entry:
// CHECK-NEXT: %addtmp = add i64 1, 1
// CHECK-NEXT: %calltmp = call i64 (ptr, ...) @printf(ptr @0, i64 %addtmp)
// CHECK-NEXT: ret void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: declare i64 @printf(ptr, ...)

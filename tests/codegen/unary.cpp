// RUN: %RVCompiler %inputs/unary.rv && FileCheck %s < unary.ll

// CHECK: ; ModuleID = 'unary.rv'
// CHECK-NEXT: source_filename = "unary.rv"
// CHECK-EMPTY:
// CHECK-NEXT: @0 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
// CHECK-EMPTY:
// CHECK-NEXT: define void @main() {
// CHECK-NEXT: entry:
// CHECK-NEXT: %negtmp = sub i64 0, 1
// CHECK-NEXT: %calltmp = call i64 (ptr, ...) @printf(ptr @0, i64 %negtmp)
// CHECK-NEXT: ret void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: declare i64 @printf(ptr, ...)

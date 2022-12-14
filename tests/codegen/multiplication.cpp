// RUN: %RVCompiler %inputs/multiplication.rv && FileCheck %s < multiplication.ll

// CHECK: ; ModuleID = 'multiplication.rv'
// CHECK-NEXT: source_filename = "multiplication.rv"
// CHECK-EMPTY:
// CHECK-NEXT: @0 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
// CHECK-EMPTY:
// CHECK-NEXT: define void @main() {
// CHECK-NEXT: entry:
// CHECK-NEXT: %multmp = mul i64 7, 7
// CHECK-NEXT: %calltmp = call i64 (ptr, ...) @printf(ptr @0, i64 %multmp)
// CHECK-NEXT: ret void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: declare i64 @printf(ptr, ...)

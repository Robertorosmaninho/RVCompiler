// RUN: %RVCompiler %inputs/divByZero.rv && FileCheck %s < divByZero.ll

// CHECK: ; ModuleID = 'RV Module'
// CHECK-NEXT: source_filename = "RV Module"
// CHECK-EMPTY:
// CHECK-NEXT: @0 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
// CHECK-EMPTY:
// CHECK-NEXT: define void @main() {
// CHECK-NEXT: entry:
// CHECK-NEXT: %subtmp = sub i64 2, 2
// CHECK-NEXT: %divtmp = sdiv i64 2, %subtmp
// CHECK-NEXT: %calltmp = call i64 (ptr, ...) @printf(ptr @0, i64 %divtmp)
// CHECK-NEXT: ret void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: declare i64 @printf(ptr, ...)

// RUN: %RVCompiler %inputs/int64Max.rv && FileCheck %s < int64Max.ll

// CHECK: ; ModuleID = 'RV Module'
// CHECK-NEXT: source_filename = "RV Module"
// CHECK-EMPTY:
// CHECK-NEXT: @0 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
// CHECK-EMPTY:
// CHECK-NEXT: define void @main() {
// CHECK-NEXT: entry:
// CHECK-NEXT: %calltmp = call i64 (ptr, ...) @printf(ptr @0, i64 9223372036854775807)
// CHECK-NEXT: ret void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: declare i64 @printf(ptr, ...)

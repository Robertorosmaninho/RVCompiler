// RUN: %RVCompiler %inputs/negativeSubtraction.rv && FileCheck %s < negativeSubtraction.ll

// CHECK: ; ModuleID = 'negativeSubtraction.rv'
// CHECK-NEXT: source_filename = "negativeSubtraction.rv"
// CHECK-EMPTY:
// CHECK-NEXT: @0 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
// CHECK-EMPTY:
// CHECK-NEXT: define void @main() {
// CHECK-NEXT: entry:
// CHECK-NEXT: %subtmp = sub i64 1, 2
// CHECK-NEXT: %calltmp = call i64 (ptr, ...) @printf(ptr @0, i64 %subtmp)
// CHECK-NEXT: ret void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: declare i64 @printf(ptr, ...)

// RUN: %RVCompiler %inputs/complex.rv && FileCheck %s < complex.ll

// CHECK: ; ModuleID = 'complex.rv'
// CHECK-NEXT: source_filename = "complex.rv"
// CHECK-EMPTY:
// CHECK-NEXT: @0 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
// CHECK-EMPTY:
// CHECK-NEXT: define void @main() {
// CHECK-NEXT: entry:
// CHECK-NEXT:   %divtmp = sdiv i64 20, 2
// CHECK-NEXT:   %subtmp = sub i64 5, 7
// CHECK-NEXT:   %multmp = mul i64 2, %subtmp
// CHECK-NEXT:   %addtmp = add i64 %divtmp, %multmp
// CHECK-NEXT:   %calltmp = call i64 (ptr, ...) @printf(ptr @0, i64 %addtmp)
// CHECK-NEXT:   ret void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: declare i64 @printf(ptr, ...)

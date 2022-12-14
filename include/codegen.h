//===- codegen.h - LLVM IR Code Generation --------------------*- C++ -*-===//
//
//                  Runtime Verification - Test Assignment
//
//===--------------------------------------------------------------------===//
// Author: Roberto Rosmaninho
// Last change: 14/12/22
//
// This file contains the declaration of IRGen class, which has only the
// raw_ostream to output the LLVM IR Module into a file and the functions
// declarations to generate and run it.
//===--------------------------------------------------------------------===//

#ifndef RVCOMPILER_CODEGEN_H
#define RVCOMPILER_CODEGEN_H

#include "node.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

class IRGen {
  llvm::raw_ostream *os;

public:
  explicit IRGen(llvm::raw_ostream *os);
  void generateCode(CallExprAST &root);
  void runJIT();
};

#endif // RVCOMPILER_CODEGEN_H

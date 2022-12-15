//===- node.h - AST Node --------------------------------------*- C++ -*-===//
//
//                  Runtime Verification - Test Assignment
//
//===--------------------------------------------------------------------===//
// Author: Roberto Rosmaninho
// Last change: 14/12/22
//
// This file contains the declarations of node classes defining the AST
// generated by our parser. The AST root is the ExprAST, and all other
// classes (leafs) subclass it.
//===--------------------------------------------------------------------===//

#ifndef RVCOMPILER_NODE_H
#define RVCOMPILER_NODE_H

#include "llvm/IR/Verifier.h"

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() = default;
  virtual llvm::Value *codegen() = 0;
};

/// NumberExprAST - Expression class for numeric literals like "1".
/// This language only accepts 64-bits integers
class NumberExprAST : public ExprAST {
  long long Num;

public:
  explicit NumberExprAST(long Num) : Num(Num) {}
  llvm::Value *codegen() override;
};

/// UnaryExprAST - Expression class for a the unary operator ("-": NEG)
class UnaryExprAST : public ExprAST {
  char Op;
  ExprAST *RHS;

public:
  UnaryExprAST(char op, ExprAST *RHS) : Op(op), RHS(RHS) {}
  llvm::Value *codegen() override;
};

/// BinaryExprAST - Expression class for a binary operators.
class BinaryExprAST : public ExprAST {
  char Op;
  ExprAST *LHS, *RHS;

public:
  BinaryExprAST(char op, ExprAST *LHS, ExprAST *RHS)
      : Op(op), LHS(LHS), RHS(RHS) {}
  llvm::Value *codegen() override;
};

/// CallExprAST - Expression class for function calls.
/// We will use it only to call printf
class CallExprAST : public ExprAST {
  std::string Callee;
  ExprAST *Arg;

public:
  CallExprAST(std::string Callee, ExprAST *Arg)
      : Callee(std::move(Callee)), Arg(Arg) {}
  llvm::Value *codegen() override;
  std::string getCallee() const { return Callee; }
};

#endif // RVCOMPILER_NODE_H

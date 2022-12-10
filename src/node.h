#include <iostream>
#include <memory>
#include <vector>

#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

// FIXME: Try to Implement Visitor interface

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
  NumberExprAST(long Num) : Num(Num) {}
  llvm::Value *codegen() override;
};

/// UnaryExprAST - Expression class for a the unary operator ("-": NEG)
class UnaryExprAST : public ExprAST {
  char Op;
  ExprAST *RHS;

public:
  UnaryExprAST(char op, ExprAST *RHS)
    : Op(op), RHS(std::move(RHS)) {}
  llvm::Value *codegen() override;
};

/// BinaryExprAST - Expression class for a binary operators.
class BinaryExprAST : public ExprAST {
  char Op;
  ExprAST *LHS, *RHS;

public:
  BinaryExprAST(char op, ExprAST *LHS, ExprAST *RHS)
    : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  llvm::Value *codegen() override;
};


/// CallExprAST - Expression class for function calls.
/// We will use it only to call printf
class CallExprAST : public ExprAST {
  std::string Callee;
  ExprAST *Arg;

public:
  CallExprAST(const std::string &Callee, ExprAST *Arg)
    : Callee(Callee), Arg(std::move(Arg)) { }
  llvm::Value *codegen() override;
  std::string getCallee() const { return Callee; }
};

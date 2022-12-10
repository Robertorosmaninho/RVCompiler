#include <iostream>
#include <memory>
#include <vector>

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() {}
};

/// NumberExprAST - Expression class for numeric literals like "1".
/// This language only accepts 64-bits integers
class NumberExprAST : public ExprAST {
  long long Num;

public:
  NumberExprAST(long Num) : Num(Num) {}
  long long getNum() const { return Num; }
};

/// UnaryExprAST - Expression class for a the unary operator ("-": NEG)
class UnaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> RHS;

public:
  UnaryExprAST(char op, std::unique_ptr<ExprAST> RHS)
    : Op(op), RHS(std::move(RHS)) {}
};

/// BinaryExprAST - Expression class for a binary operators.
class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
    : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};


/// CallExprAST - Expression class for function calls.
/// We will use it only to call printf
class CallExprAST : public ExprAST {
  std::string Callee;
  std::unique_ptr<ExprAST> Arg;

public:
  CallExprAST(const std::string &Callee, std::unique_ptr<ExprAST> Arg)
    : Callee(Callee), Arg(std::move(Arg)) {}
  std::string getCallee() const { return Callee; }
};

//NumberExprAST* exp = dynamic_cast<NumberExprAST*>($3);
//printf("%lld\n", exp->getNum());
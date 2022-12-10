#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <llvm/Support/raw_ostream.h>

#include "codegen.h"

#include "llvm/IR/Value.h"

using namespace llvm;

static std::unique_ptr<IRBuilder<>> Builder;
static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;

Value *LogErrorV(const char *Str) {
  llvm::errs() << Str << "\n";
  return nullptr;
}

Value *NumberExprAST::codegen() {
  llvm::errs() << "Codegen for NumberExprAST\n";
  return ConstantInt::get(*TheContext, APInt(64, Num, true)); // 64bits signed int
}

Value *UnaryExprAST::codegen() {
  llvm::errs() << "Codegen for UnaryExprAST\n";
  Value *R = RHS->codegen();
  if (!R)
    return nullptr;

  if (Op == '-')
    return Builder->CreateNeg(R, "negtmp");
  else
    return LogErrorV("invalid unary operator");
}

Value *BinaryExprAST::codegen() {
  llvm::errs() << "Codegen for BinaryExprAST\n";
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  switch (Op) {
  case '+':
    return  Builder->CreateAdd(L, R, "addtmp");
  case '-':
    return Builder->CreateSub(L, R, "subtmp");
  case '*':
    return Builder->CreateMul(L, R, "multmp");
  case '/':
    return Builder->CreateSDiv(L, R, "divtmp");

  default:
    return LogErrorV("invalid binary operator");
  }
}

Value *CallExprAST::codegen() {
  llvm::errs() << "Codegen for CallExprAST\n";
  // Look up the name in the global module table.

  if (getCallee() != "print")
    return LogErrorV("Unknown function");

  auto i32 = IntegerType::getInt32Ty(*TheContext);
  auto i8_ptr = PointerType::get(Type::getInt8Ty(*TheContext), 0);
  auto funcType = FunctionType::get(i32, i8_ptr, true);

  auto insert_point = Builder->GetInsertPoint();
  auto CalleeF = TheModule->getOrInsertFunction("printf", funcType);

  if (!CalleeF)
    return LogErrorV("Unknown function referenced");

  std::vector<Value *> ArgsV;
  ArgsV.push_back(Arg->codegen());

  return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

CodeGenContext::CodeGenContext() {
  llvm::errs() << "Initializing codegen context\n";

  // Open a new context and module.
  TheContext = std::make_unique<LLVMContext>();

  // Creating the LLVM Module
  TheModule = std::make_unique<Module>("RV Module", *TheContext);

  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<>>(*TheContext);
}

/* Compile the AST into a module */
void CodeGenContext::generateCode(CallExprAST& root) {
    llvm::errs() << "Generating code...\n";

    /* Create the top level interpreter function to call as entry */
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(*TheContext), false);
    Function *mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", TheModule.get());
    BasicBlock *bblock = BasicBlock::Create(*TheContext, "entry", mainFunction);

    // Setting the start point to write functions
    Builder->SetInsertPoint(bblock);

    // Emit bytecode for the toplevel block
    root.codegen();

    ReturnInst::Create(*TheContext, bblock);

    TheModule->print(errs(), nullptr);
}

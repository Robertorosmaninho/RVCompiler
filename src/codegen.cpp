//===- FileCheck.cpp - Check that File's Contents match what is expected --===//
//
//                  Runtime Verification - Test Assignment
//
//===----------------------------------------------------------------------===//
// Author: Roberto Rosmaninho
// Last change: 14/12/22
//
// This file contains the definitions of the codegen Node functions and the
// definitions of IRGen functions.
//
// This file initiates all the required information for code generation and for
// calling the JIT compilation.
//===----------------------------------------------------------------------===//

#include "include/codegen.h"
#include "include/jit.h"

#include "llvm/IR/NoFolder.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"

using namespace llvm;
using namespace llvm::orc;

static std::unique_ptr<IRBuilder<llvm::NoFolder>> Builder;
static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;

Value *LogError(const char *Str) { llvm_unreachable(Str); }

Value *NumberExprAST::codegen() {
  return Builder->getInt(APInt(64, Num, true)); // 64bits signed int
}

Value *UnaryExprAST::codegen() {
  Value *R = RHS->codegen();
  if (!R)
    return LogError("Unknown operand in unary expression\n");

  if (Op == '-')
    return Builder->CreateNeg(R, "negtmp");
  else
    return LogError("invalid unary operator\n");
}

Value *BinaryExprAST::codegen() {
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return LogError("Unknown operand in binary expression\n");

  switch (Op) {
  case '+':
    return Builder->CreateAdd(L, R, "addtmp");
  case '-':
    return Builder->CreateSub(L, R, "subtmp");
  case '*':
    return Builder->CreateMul(L, R, "multmp");
  case '/': {
    return Builder->CreateSDiv(L, R, "divtmp");
  }
  default:
    return LogError("invalid binary operator\n");
  }
}

Value *CallExprAST::codegen() {
  // Look up the name in the global module table.
  if (getCallee() != "print")
    return LogError("Unknown function\n");

  auto i64 = Builder->getInt64Ty();
  auto i8_ptr = PointerType::get(Type::getInt8Ty(*TheContext), 0);
  auto funcType = FunctionType::get(i64, {i8_ptr}, true);

  auto CalleeF = TheModule->getOrInsertFunction("printf", funcType);

  if (!CalleeF)
    return LogError("Unknown function referenced\n");

  llvm::Value *formatStr = Builder->CreateGlobalStringPtr("%lld\n");
  auto argValue = Arg->codegen();

  if (!argValue)
    return LogError("Unknown argument for print.\n");

  return Builder->CreateCall(CalleeF, {formatStr, argValue}, "calltmp");
}

IRGen::IRGen(llvm::raw_ostream *output) {
  // Setting the output raw_stream to print the module
  os = output;

  // Open a new context and module.
  TheContext = std::make_unique<LLVMContext>();

  // Creating the LLVM Module
  TheModule = std::make_unique<Module>("RV Module", *TheContext);

  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<NoFolder>>(*TheContext);
}

/* Compile the AST into a module */
void IRGen::generateCode(CallExprAST &root) {
  /* Create the top level interpreter function to call as entry */
  FunctionType *ftype = FunctionType::get(Type::getVoidTy(*TheContext), false);
  Function *mainFunction = Function::Create(ftype, GlobalValue::ExternalLinkage,
                                            "main", TheModule.get());
  BasicBlock *bblock = BasicBlock::Create(*TheContext, "entry", mainFunction);

  // Setting the start point to write functions
  Builder->SetInsertPoint(bblock);

  // Emit bytecode for the toplevel block
  auto printFunction = root.codegen();
  if (!printFunction)
    return;

  // Creating void return instruction
  Builder->CreateRetVoid();

  TheModule->print(*os, nullptr);
}

void IRGen::runJIT() {
  // Initializing data needed by TheJIT
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  // Create an LLJIT instance.
  auto rvJIT = JIT::create(TheModule, TheContext);

  // Registering the printf function on the JIT.
  rvJIT->registerSymbols([&](llvm::orc::MangleAndInterner interner) {
    llvm::orc::SymbolMap symbolMap;
    symbolMap[interner("printf")] =
        llvm::JITEvaluatedSymbol::fromPointer(printf);
    return symbolMap;
  });

  // Getting the main function address from the module to be executed.
  auto mainAddr = rvJIT->lookup("main");
  auto *mainFunc = mainAddr->toPtr<long long(void)>();

  // Executing the main function.
  mainFunc();
}

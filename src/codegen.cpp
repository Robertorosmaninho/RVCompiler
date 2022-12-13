#include "include/codegen.h"
#include "include/JIT.h"

#include "llvm/IR/NoFolder.h"
#include "llvm/Support/TargetSelect.h"

using namespace llvm;
using namespace llvm::orc;

static std::unique_ptr<IRBuilder<llvm::NoFolder>> Builder;
static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;

Value *LogErrorV(const char *Str) {
  llvm::errs() << Str << "\n";
  return nullptr;
}

Value *NumberExprAST::codegen() {
  return ConstantInt::get(*TheContext,
                          APInt(64, Num, true)); // 64bits signed int
}

Value *UnaryExprAST::codegen() {
  Value *R = RHS->codegen();
  if (!R)
    return nullptr;

  if (Op == '-')
    return Builder->CreateNeg(R, "negtmp");
  else
    return LogErrorV("invalid unary operator");
}

Value *BinaryExprAST::codegen() {
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  switch (Op) {
  case '+':
    return Builder->CreateAdd(L, R, "addtmp");
  case '-':
    return Builder->CreateSub(L, R, "subtmp");
  case '*':
    return Builder->CreateMul(L, R, "multmp");
  case '/': {
    if (ConstantInt *CI = dyn_cast<ConstantInt>(R))
      if (CI->isZero())
        return LogErrorV("Division by 0 isn't allowed.");
    return Builder->CreateSDiv(L, R, "divtmp");
  }
  default:
    return LogErrorV("invalid binary operator");
  }
}

Value *CallExprAST::codegen() {
  // Look up the name in the global module table.

  if (getCallee() != "print")
    return LogErrorV("Unknown function");

  auto i64 = Builder->getInt64Ty();
  auto i8_ptr = PointerType::get(Type::getInt8Ty(*TheContext), 0);
  auto funcType = FunctionType::get(i64, {i8_ptr}, true);

  auto CalleeF = TheModule->getOrInsertFunction("printf", funcType);

  if (!CalleeF)
    return LogErrorV("Unknown function referenced");

  llvm::Value *formatStr = Builder->CreateGlobalStringPtr("%lld\n");
  auto argValue = Arg->codegen();

  if (!argValue)
    return LogErrorV("Unknown argument for print.");

  return Builder->CreateCall(CalleeF, {formatStr, argValue}, "calltmp");
}

CodeGenContext::CodeGenContext(llvm::raw_ostream *output) {
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
void CodeGenContext::generateCode(CallExprAST &root) {
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

  ReturnInst::Create(*TheContext, bblock);

  TheModule->print(*os, nullptr);
}

void CodeGenContext::runJIT() {
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

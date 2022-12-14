//===- jit.h - Just In Time Compiler Implementation -----------*- C++ -*-===//
//
//                  Runtime Verification - Test Assignment
//
//===--------------------------------------------------------------------===//
// Author: Roberto Rosmaninho
// Last change: 14/12/22
//
// This file contains the declaration of the JIT class, which has only the
// unique_ptr to the jit object and the functions needed to execute the LLVM
// IR generated by the compiler.
//===--------------------------------------------------------------------===//

#ifndef RVCOMPILER_JIT_H
#define RVCOMPILER_JIT_H

#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <memory>
#include <type_traits>

// Make the JIT safe
static ExitOnError ExitOnErr;

class JIT {
private:
  std::unique_ptr<llvm::orc::LLJIT> rvJIT;

public:
  JIT(std::unique_ptr<llvm::orc::LLJIT> _rvjit) : rvJIT(std::move(_rvjit)) {}

  // Create the JIT with the given module and context.
  static llvm::Expected<JIT>
  create(std::unique_ptr<llvm::Module> &module,
         std::unique_ptr<llvm::LLVMContext> &context) {
    auto rvJIT = ExitOnErr(llvm::orc::LLJITBuilder().create());
    auto tsModule =
        llvm::orc::ThreadSafeModule(std::move(module), std::move(context));

    ExitOnErr(rvJIT->addIRModule(std::move(tsModule)));

    return JIT(std::move(rvJIT));
  }

  // Function to add symbols to the JIT session. In this case used to add
  // printf.
  void registerSymbols(
      llvm::function_ref<llvm::orc::SymbolMap(llvm::orc::MangleAndInterner)>
          symbolMap) {
    auto &mainJitDylib = rvJIT->getMainJITDylib();
    llvm::cantFail(mainJitDylib.define(
        absoluteSymbols(symbolMap(llvm::orc::MangleAndInterner(
            mainJitDylib.getExecutionSession(), rvJIT->getDataLayout())))));
  }

  // Function used to get a function on the JIT to be executed.
  llvm::Expected<llvm::orc::ExecutorAddr> lookup(const std::string &name) {
    auto symbol = rvJIT->lookup(name);

    if (!symbol)
      return symbol.takeError();

    return symbol;
  }
};

#endif // RVCOMPILER_JIT_H

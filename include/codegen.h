#include "node.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

class CodeGenContext {
  llvm::raw_ostream *os;

public:
  explicit CodeGenContext(llvm::raw_ostream *os);
  void generateCode(CallExprAST &root);
  void runJIT();
};
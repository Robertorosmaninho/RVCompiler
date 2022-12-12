#include "llvm/IR/IRBuilder.h"

#include "node.h"

using namespace llvm;

class CodeGenContext {
    llvm::raw_ostream *os;
public:
    explicit CodeGenContext(llvm::raw_ostream *os);
    void generateCode(CallExprAST& root);
};
#include <iostream>
#include "codegen.h"
//#include "parser.hpp"

extern CallExprAST *printCall;
extern int yyparse(void);

int main() {
    yyparse();

    if (printCall == nullptr)
        std::cout << "Parsing Errro!\n";

    CodeGenContext *context = new CodeGenContext();
    context->generateCode(*printCall);

    return 0;
}

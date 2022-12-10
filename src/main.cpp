#include <iostream>
#include "node.h"

extern CallExprAST *printCall;
extern int yyparse(void);

int main() {
    yyparse();
    return 0;
}

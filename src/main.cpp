#include <iostream>
#include "parser.h"

extern "C" int yyparse(void);

int main() {
    yyparse();
    return 0;
}

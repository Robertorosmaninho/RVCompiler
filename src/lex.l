/* definitions */
%{
#include "node.h"
#include "parser.hpp"
%}

%%

[0-9]+           { yylval.numAST = new NumberExprAST(atoll(yytext)); return INT; }
"print"          { yylval.string = new std::string(yytext, yyleng); return PRINT; }

"("              { return LP; }
"*"              { return MUL; }
"/"              { return DIV; }
"+"              { return ADD; }
"-"              { return SUB; }
")"              { return RP; }
";"              { return SEMICOLON; }

[ \t]            { }
\n               { return EOL; }
.                { printf("Unkown character: %s\n", yytext); } yyterminate();

%%

int yywrap() { return 0; }

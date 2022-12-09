/* definitions */
%{
#include "parser.tab.h"
%}

%%

[0-9]+           { yylval.num = atoi(yytext); return INT; }

"print"          { return PRINT; }
"("              { return LP; }
"*"              { return MUL; }
"/"              { return DIV; }
"+"              { return ADD; }
"-"              { return SUB; }
")"              { return RP; }
";"              { return SEMICOLON; }

[ \t]            { }
\n               { return EOL; }
.                { printf("Unkown character: %s\n", yytext); }

%%

int yywrap() { return 0; }

/* definitions */
%{

#include "stdint.h"

int yyerror(const char *);
int yylex();

%}

%union {
    uint64_t num;
}

%token<num> INT
%type<num> exp

%token PRINT
%token LP
%token RP

%token SEMICOLON

%token ADD
%token SUB
%token MUL
%token DIV
%token NEG

%token EOL

%left ADD SUB
%left MUL DIV
%nonassoc NEG

/* rules */
%%

input:
|   pgm input
;

pgm:
    PRINT LP exp RP SEMICOLON EOL { printf("%lld\n", $3); }
;

exp:
    INT                       { $$ = $1; }
|   LP exp RP                 { $$ = $2; }
|   SUB exp      %prec NEG    { $$ = -$2; }
|   exp ADD exp               { $$ = $1 + $3; }
|   exp SUB exp               { $$ = $1 - $3; }
|   exp MUL exp               { $$ = $1 * $3; }
|   exp DIV exp               { $$ = $1 / $3; }
;

%%

int yyerror(const char* err) {
    printf("ERROR: %s\n", err);

    return 0;
}


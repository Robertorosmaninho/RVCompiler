/* definitions */
%{

#include "stdint.h"
#include <FlexLexer.h>
#include "include/node.h"

int yyerror(const char *);
auto lexer = new yyFlexLexer();
#define yylex() lexer->yylex();

CallExprAST *printCall; /* the top level root node of our Expr AST */

%}

%union {
    CallExprAST *printCall;
    NumberExprAST *numAST;
    ExprAST *exprAST;
    std::string *string;
}

%token<numAST> INT

%type<exprAST> exp
%type<printCall> input pgm

%token<string> PRINT
%token LP
%token RP

%token SEMICOLON

%token ADD
%token SUB
%token MUL
%token DIV
%token NEG

%left ADD SUB
%left MUL DIV
%nonassoc NEG

%start input

/* rules */
%%
input:
    pgm { printCall = $1; YYACCEPT; }
;

pgm:
    PRINT LP exp RP SEMICOLON { $$ = new CallExprAST(*$1, $3); }
;

exp:
    INT                     { $$ = $1; }
|   LP exp RP               { $$ = $2; }
|   SUB exp      %prec NEG  { $$ = new UnaryExprAST('-', $2); }
|   exp ADD exp             { $$ = new BinaryExprAST('+', $1, $3); }
|   exp SUB exp             { $$ = new BinaryExprAST('-', $1, $3); }
|   exp MUL exp             { $$ = new BinaryExprAST('*', $1, $3); }
|   exp DIV exp             { $$ = new BinaryExprAST('/', $1, $3); }
;

%%

int yyerror(const char* err) {
    printf("ERROR: %s\n", err);

    return 0;
}

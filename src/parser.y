/* definitions */
%{

#include "stdint.h"
#include "node.h"

int yyerror(const char *);
int yylex();

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

%token EOL

%left ADD SUB
%left MUL DIV
%nonassoc NEG

%start pgm

/* rules */
%%
input:
|    pgm { printCall = $1; }
;

pgm:
    PRINT LP exp RP SEMICOLON EOL {
        $$ = new CallExprAST(*$1, std::make_unique<ExprAST>(*$3))  }
;

exp:
    INT                       { $$ = $1; }
|   LP exp RP                 { $$ = $2; }
|   SUB exp      %prec NEG  { $$ = new UnaryExprAST('-', std::make_unique<ExprAST>(*$2)); }
|   exp ADD exp             { $$ = new BinaryExprAST('+', std::make_unique<ExprAST>(*$1), std::make_unique<ExprAST>(*$3)); }
|   exp SUB exp             { $$ = new BinaryExprAST('-', std::make_unique<ExprAST>(*$1), std::make_unique<ExprAST>(*$3)); }
|   exp MUL exp             { $$ = new BinaryExprAST('*', std::make_unique<ExprAST>(*$1), std::make_unique<ExprAST>(*$3)); }
|   exp DIV exp             { $$ = new BinaryExprAST('/', std::make_unique<ExprAST>(*$1), std::make_unique<ExprAST>(*$3)); }
;

%%

int yyerror(const char* err) {
    printf("ERROR: %s\n", err);

    return 0;
}



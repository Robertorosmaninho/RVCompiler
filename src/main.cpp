

#include <cassert>
#include <fstream>
#include <iostream>

#include "FlexLexer.h"
#include "include/codegen.h"

extern CallExprAST *printCall;
extern yyFlexLexer *lexer;

extern int yyparse();

std::string getInputFileName(char *input) {
  auto path = std::string(input);

  size_t sep = path.find_last_of("\\/");
  if (sep != std::string::npos)
    path = path.substr(sep + 1, path.size() - sep - 1);

  size_t dot = path.find_last_of('.');
  assert(".rv" == path.substr(dot, path.size() - dot));
  return path.substr(0, dot);
}

int main(int argc, char *argv[]) {
  // Defining usage and returning if no input file was given
  if (argc < 2) {
    std::cout << "Usage: ./RVCompiler input_file\n";
    return 1;
  }

  // Opening input file.
  std::ifstream inputFile(argv[1]);
  if (!inputFile)
    std::cout << "Error opening file!\n";

  // Telling the lexer to scan from the input file.
  lexer->yyrestart(inputFile);

  // Parsing the program.
  yyparse();

  inputFile.close();

  // Checking if our program was correctly parsed and the AST created.
  if (printCall == nullptr)
    std::cout << "Parsing Error!\n";

  auto outputFile = getInputFileName(argv[1]) + ".ll";

  // Initializing the output LLVM IR file.
  std::error_code EC;
  llvm::raw_ostream *os = new raw_fd_ostream(outputFile, EC);

  auto *irGen = new IRGen(os);
  irGen->generateCode(*printCall);

  if (argc > 2 && strcmp(argv[2], "--jit") == 0)
    irGen->runJIT();

  return 0;
}

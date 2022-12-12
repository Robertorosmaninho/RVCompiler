#include <iostream>
#include <fstream>
#include <cassert>
#include "include/codegen.h"

extern CallExprAST *printCall;
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

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
        std::cout << "Usage: ./RVCompiler input_file";
        return 1;
    }

    // Opening input file.
    std::ifstream inputFile(argv[1]);
    if (!inputFile)
        std::cout << "Error opening file!\n";

    // Reading the program's single line.
    std::string programLine;
    getline(inputFile, programLine);

    inputFile.close();

    // Creating a buffer containing the program. This buffer will be parsers input instead of the stdin.
    YY_BUFFER_STATE buffer = yy_scan_string(programLine.c_str());

    // Parsing the program.
    yyparse();

    // Deleting buffer to avoid memory leak.
    yy_delete_buffer(buffer);

    // Checking if our program was correctly parsed and the AST created.
    if (printCall == nullptr)
        std::cout << "Parsing Error!\n";

    auto outputFile = getInputFileName(argv[1]) + ".ll";

    // Initializing the output LLVM IR file.
    std::error_code EC;
    llvm::raw_ostream *os = new raw_fd_ostream(outputFile, EC);

    auto *context = new CodeGenContext(os);
    context->generateCode(*printCall);

    return 0;
}

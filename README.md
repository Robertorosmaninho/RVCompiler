# RVCompiler
Runtime Verification - Test Assignment

This project contains a simple compiler that reads a program written in the language described by the grammar below, parses it creates an AST, and then generates the LLVM bytecodes of the program. The compiler outputs this LLVM bytecodes as `filename.ll`, but can also execute it using a simple JIT implementation that prints the program's output on the standard output.

## Grammar
```
syntax Pgm ::= "print" "(" Exp ")" ";"
syntax Exp ::= Int
             | "(" Exp ")"
             | "-" Exp
             | Exp "*" Exp
             | Exp "/" Exp
             | Exp "+" Exp
             | Exp "-" Exp
syntax Int ::= r"[0-9]+"

```

This compiler is written mainly in C++, built with the softwares listed below, and was tested on a MacOS 13.0.1.
Two major decisions made on the design of this project were whether to use the C or C++ API of Bison and Flex. Unfortunately, the C++ interface of these programs is really poor, and most people still use the C API of them until those days.
However, the Flex C++ API has a great advantage: it can read the input from a file as istream, and since this compiler is written in C++, passing the input file directly to the scanner pays for other downsides. The Bison C++ API doesn't have significant benefits, so I chose not to use it in this project.


# Requirements
```
cmake version 3.22.3
ninja version 1.10.2
bison (GNU Bison) 3.8.2
flex 2.6.4
LLVM 14.0.0
```

This compiler can be build using CMake and Ninja as shown below:

# Building
```
mkdir build
cd build

cmake .. -G Ninja
ninja
```

# Executing llvm-lit Tests
```
ninja check
```

# Generating LLVM Bytecode
```
./bin/RVCompiler ../tests/inputs/complex.rv
```

# Executing program with RV-JIT

```
./bin/RVCompiler ../tests/inputs/complex.rv --jit
```

# Executing LLVM Bytecode with lli
```
./bin/RVCompiler ../tests/inputs/complex.rv
lli complexTest.ll
```

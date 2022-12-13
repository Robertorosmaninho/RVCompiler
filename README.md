# RVCompiler
Runtime Verification - Test Assignment

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

# Requirements
```
cmake version 3.22.3
ninja version 1.10.2
bison (GNU Bison) 3.8.2
flex 2.6.4
LLVM 14.0.0
```

# Building
```
mkdir build
cd build

cmake .. -G Ninja
ninja
```
# Executing llvm-lit Tests
```
cd build
ninja check
```

# Generating LLVM Bytecode
```
./RVCompiler complexTest.rv
```

# Executing program with RV-JIT

```
./RVCompiler complexTest.rv --jit
```

# Executing LLVM Bytecode with lli
```
./RVCompiler complexTest.rv
lli complexTest.ll
```

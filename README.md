# RVCompiler
Runtime Verification - Test Assignment

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

# Testing
```
./RVCompiler complexTest.rv
lli complexTest.ll
```


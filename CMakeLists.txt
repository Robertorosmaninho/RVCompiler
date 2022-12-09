cmake_minimum_required(VERSION 3.5)
project(RVCompiler)

set(CMAKE_CXX_STANDARD 14)

## Flex/Bison configuration
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

## LLVM configuration
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

if (WIN32)
    set (ADDITIONAL_FLEX_FLAGS "--wincompat")
else()
    set (ADDITIONAL_FLEX_FLAGS "")
endif()
FLEX_TARGET(LEXER ${CMAKE_CURRENT_SOURCE_DIR}/src/lex.l
${CMAKE_CURRENT_BINARY_DIR}/lexer.c DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h COMPILE_FLAGS ${ADDITIONAL_FLEX_FLAGS})
BISON_TARGET(PARSER ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y
${CMAKE_CURRENT_BINARY_DIR}/parser.c COMPILE_FLAGS "-d -t")

flex_target(lexer lexer.ll "${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp")
bison_target(parser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)

ADD_FLEX_BISON_DEPENDENCY(LEXER PARSER)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})


# FIXME
file(GLOB source_files
    ${CMAKE_CURRENT_BINARY_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${source_files} ${BISON_PARSER_OUTPUTS} ${FLEX_LEXER_OUTPUTS})
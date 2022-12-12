cmake_minimum_required(VERSION 3.5)
project(RVCompiler)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

## Flex/Bison configuration
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

## LLVM configuration
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

if (WIN32)
    set (ADDITIONAL_FLEX_FLAGS "--wincompat")
else()
    set (ADDITIONAL_FLEX_FLAGS "")
endif()

BISON_TARGET(PARSER ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y
        ${CMAKE_CURRENT_BINARY_DIR}/lib/parser.cpp DEFINES_FILE
        ${CMAKE_CURRENT_BINARY_DIR}/lib/parser.hpp COMPILE_FLAGS "-d -t")

FLEX_TARGET(LEXER ${CMAKE_CURRENT_SOURCE_DIR}/src/lex.l
        ${CMAKE_CURRENT_BINARY_DIR}/lib/lexer.cpp DEFINES_FILE
        ${CMAKE_CURRENT_BINARY_DIR}/lib/lexer.h COMPILE_FLAGS
        ${ADDITIONAL_FLEX_FLAGS})

ADD_FLEX_BISON_DEPENDENCY(LEXER PARSER)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)


file(GLOB source_files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/codegen.cpp
)

add_executable(${PROJECT_NAME} ${source_files}
    ${BISON_PARSER_OUTPUTS} ${FLEX_LEXER_OUTPUTS})

# Link against LLVM libraries
target_link_libraries(${PROJECT_NAME} ${llvm_libs})

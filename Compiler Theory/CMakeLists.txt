cmake_minimum_required(VERSION 3.20)
project(Compiler_Theory)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler_Theory MainClass.cpp
                                Lexer.cpp
                                Token.cpp
                                Parser.cpp
                                ASTNodes.cpp
                                XMLVisitorNodes.cpp
                                SemanticVisitorNodes.cpp
                                SymbolTable.cpp
                                CodeGeneratorVisitorNodes.cpp)

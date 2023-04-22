//
// Created by Matthias on 11/04/2023.
//
//Inclusion of relevant header File
#include "ASTNodes.h"

//Creating Struct Scope, which will be used by Symbol Table
struct Scope{
    map<string,map<string,string>> scope;
};

//Symbol Table class with its relevant variables and methods
class SymbolTable{
public:
    SymbolTable()=default;
    vector<Scope> scopeStack;
    void push(const Scope& scope);
    Scope pop();
    bool CheckIdentifierExists(const string& identifier, bool functionFlag);
    string ReturnIdentifierType(const string& identifier);
    string ReturnFunctionParameters(const string& identifier);
    string ReturnIdentifierAddress(const string& identifier);
    ~SymbolTable()=default;
};
//
// Created by Matthias on 11/04/2023.
//
//Inclusion of relevant header File
#include "ASTNodes.h"

struct Scope{
    map<string,map<string,string>> scope;
};

class SymbolTable{
public:
    SymbolTable()=default;
    vector<Scope> scopeStack;
    void push(const Scope& scope);
    Scope pop();
    ~SymbolTable()=default;
};
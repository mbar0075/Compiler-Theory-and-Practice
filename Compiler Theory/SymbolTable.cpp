//
// Created by Matthias on 11/04/2023.
//
#include "SymbolTable.h"

void SymbolTable::push(const Scope& scope) {
    scopeStack.push_back(scope);
}

Scope SymbolTable::pop() {
    Scope scopeReturn=scopeStack.back();
    scopeStack.pop_back();
    return scopeReturn;
}
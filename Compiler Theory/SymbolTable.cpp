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

bool SymbolTable::CheckIdentifierExists(const string& identifier) {
    auto iter = scopeStack.end();
    for(iter--; iter >= scopeStack.begin(); iter--){
        if((*iter).scope.find(identifier) != (*iter).scope.end()){
            return true;
        }
    }
    return false;
}

string SymbolTable::ReturnIdentifierType(const string &identifier) {
    auto iter= scopeStack.end();
    for(iter--; iter >= scopeStack.begin(); iter--){
        if((*iter).scope.find(identifier) != (*iter).scope.end()){
            return (*iter).scope[identifier]["type"];
        }
    }
    return "No Type";
}

string SymbolTable::ReturnFunctionParameters(const string &identifier) {
    auto iter= scopeStack.end();
    for(iter--; iter >= scopeStack.begin(); iter--){
        if((*iter).scope.find(identifier) != (*iter).scope.end()){
            return (*iter).scope[identifier]["FormalParam"];
        }
    }
    return "No Parameters";
}
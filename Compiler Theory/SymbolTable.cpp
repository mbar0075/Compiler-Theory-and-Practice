//Inclusion of the relevant header file
#include "SymbolTable.h"

//Method which pushes a Scope onto the symbol table scopeStack
void SymbolTable::push(const Scope& scope) {
    scopeStack.push_back(scope);
}
//Method which pops and returns, the last pushed Scope from the scopeStack
Scope SymbolTable::pop() {
    Scope scopeReturn=scopeStack.back();
    scopeStack.pop_back();
    return scopeReturn;
}
//Method which given an Identifier and a FunctionFlag (to determine whether identifier is a function), checks whether Identifier exists
bool SymbolTable::CheckIdentifierExists(const string& identifier, bool functionFlag) {
    //Iterating through all the Scopes in the scopeStack, in the order of the last pushed scope
    auto iter = scopeStack.end();
    for(iter--; iter >= scopeStack.begin(); iter--){
        //If Identifier is found, returning true
        if((*iter).scope.find(identifier) != (*iter).scope.end()&&!functionFlag){
            return true;
        }
        //Checking whether identifier is a function or not, if it is checking whether it has FormalParam and returning true
        //If not returning False, as identifier is not a function, but FunctionFlag is set to true
        if((*iter).scope.find(identifier) != (*iter).scope.end()&&functionFlag){
            if((*iter).scope[identifier].find("FormalParam")!= (*iter).scope[identifier].end()){
                return true;
            }
            else{
                return false;
            }
        }
    }
    //Returning false, as Identifier was not found
    return false;
}
//Method which given an Identifier, returns the Identifier Type
string SymbolTable::ReturnIdentifierType(const string &identifier) {
    //Iterating through all the Scopes in the scopeStack, in the order of the last pushed scope
    auto iter= scopeStack.end();
    for(iter--; iter >= scopeStack.begin(); iter--){
        //If Identifier is found, returning its Type
        if((*iter).scope.find(identifier) != (*iter).scope.end()){
            return (*iter).scope[identifier]["type"];
        }
    }
    //Returning "No Type", as Identifier was not found
    return "No Type";
}
//Method which given an Identifier, returns the Identifier Formal Parameters
string SymbolTable::ReturnFunctionParameters(const string &identifier) {
    auto iter= scopeStack.end();
    for(iter--; iter >= scopeStack.begin(); iter--){
        //If Identifier is found, returning its Formal Parameters
        if((*iter).scope.find(identifier) != (*iter).scope.end()){
            return (*iter).scope[identifier]["FormalParam"];
        }
    }
    //Returning "No Parameters", as Identifier was not found
    return "No Parameters";
}
//Method which given an Identifier, returns the Identifier Address
string SymbolTable::ReturnIdentifierAddress(const string &identifier) {
    auto iter= scopeStack.end();
    for(iter--; iter >= scopeStack.begin(); iter--){
        //If Identifier is found, returning its Address
        if((*iter).scope.find(identifier) != (*iter).scope.end()){
            return (*iter).scope[identifier]["Address"];
        }
    }
    //Returning 0
    return "0";
}
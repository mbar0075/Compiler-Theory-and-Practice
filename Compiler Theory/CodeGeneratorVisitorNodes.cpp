//Inclusion of the relevant header File
#include "VisitorNodes.h"

void CodeGeneratorVisitorNode::visit( ASTNode *pointer){}
void CodeGeneratorVisitorNode::visit( ASTStatement *pointer){}
void CodeGeneratorVisitorNode::visit( ASTProgram *pointer){
    //Creating a new Scope and pushing it onto the symbol Table
    Scope initialScope;
    symbolTable->push(initialScope);
    //Retrieving access to the last scope pushed in the symbol table
    auto scopeIter = symbolTable->scopeStack.end();scopeIter--;
    //Setting the currentStoreFunctionName to main
    currentStoredFunctionName="main";
    functionNames.emplace_back("main");
    //Updating the print key for the currentStoredFunctionName in the symbol table
    (*scopeIter).scope[currentStoredFunctionName]["print"]+=".main\n";
    string currentFunctionName=currentStoredFunctionName;
    //Iterating through all the statements in the program, and resetting the currentStoredFunctionName, for every iteration
    for(auto iter = pointer->program.begin(); iter < pointer->program.end(); iter++)
    {
        currentStoredFunctionName=currentFunctionName;
        ((*iter))->accept(this);
    }
    currentStoredFunctionName=currentFunctionName;
    scopeIter = symbolTable->scopeStack.begin();
    //Updating the print key for the currentStoredFunctionName in the symbol table
    (*scopeIter).scope[currentStoredFunctionName]["print"]+="halt\n";
    PrintProgram();
    //Popping initialScope from the symbol Table
    symbolTable->pop();
}
void CodeGeneratorVisitorNode::visit( ASTBlock *pointer){
    //Creating a new Scope and pushing it onto the symbol Table
    Scope blockScope;
    symbolTable->push(blockScope);
    string currentFunctionName=currentStoredFunctionName;
    //Iterating through all the statements in the block, and resetting the currentStoredFunctionName, for every iteration
    for(auto iter = pointer->statements.begin(); iter < pointer->statements.end(); iter++)
    {
        currentStoredFunctionName=currentFunctionName;
        ((*iter))->accept(this);
    }
    //Popping blockScope from the symbol Table
    symbolTable->pop();
}
void CodeGeneratorVisitorNode::visit( ASTPrintStatement *pointer){
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTDelayStatement *pointer){
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTForStatement *pointer){
    if(pointer->variableDecl!= nullptr){
        pointer->variableDecl->accept(this);
    }
    pointer->expression->accept(this);
    if(pointer->assignment!= nullptr) {
        pointer->assignment->accept(this);
    }
    pointer->block->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTWhileStatement *pointer){
    pointer->expression->accept(this);
    pointer->block->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTIfStatement *pointer){
    pointer->expression->accept(this);
    pointer->firstBlock->accept(this);
    if(pointer->secondBlock!= nullptr){
        pointer->secondBlock->accept(this);
    }
}
void CodeGeneratorVisitorNode::visit( ASTVariableDecl *pointer){
    pointer->type->accept(this);
    pointer->identifier->accept(this);
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTAssignment *pointer){
    pointer->identifier->accept(this);
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTFunctionDecl *pointer){
    //Creating a new Scope and pushing it onto the symbol Table
    Scope funScope;
    symbolTable->push(funScope);
    //Retrieving access to the last scope pushed in the symbol table
    auto scopeIter = symbolTable->scopeStack.end();scopeIter--;
    //Updating the currentStoredFunctionName, to the identifier
    string identifier=pointer->identifier->identifier;
    currentStoredFunctionName=identifier;
    //Updating the print key for the currentStoredFunctionName in the symbol table
    (*scopeIter).scope[currentStoredFunctionName]["print"]+="\n."+identifier+"\n";
    functionNames.emplace_back(identifier);
    if(pointer->formalParams!= nullptr){
        pointer->formalParams->accept(this);
    }

//    pointer->type->accept(this);
    pointer->block->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTFunctionCall *pointer){
    pointer->identifier->accept(this);
    if(pointer->actualParams!= nullptr){
        pointer->actualParams->accept(this);
    }
}
void CodeGeneratorVisitorNode::visit( ASTFormalParams *pointer){
    //Retrieving access to the last scope pushed in the symbol table
    auto scopeIter = symbolTable->scopeStack.end();scopeIter--;
    //Updating the print key for the currentStoredFunctionName in the symbol table
    (*scopeIter).scope[currentStoredFunctionName]["print"]+="push "+to_string(pointer->formalParams.size())+"\nalloc\n";
    //Looping through all the formalParams vector and calling the accept method
    for(auto iter = pointer->formalParams.begin(); iter < pointer->formalParams.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}
void CodeGeneratorVisitorNode::visit( ASTFormalParam *pointer){
    pointer->identifier->accept(this);
    pointer->type->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTType *pointer){}
void CodeGeneratorVisitorNode::visit( ASTIdentifier *pointer){}
void CodeGeneratorVisitorNode::visit( ASTMultiplicativeOp *pointer){
    pointer->leftFactor->accept(this);
    pointer->rightFactor->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTAdditiveOp *pointer){
    pointer->leftTerm->accept(this);
    pointer->rightTerm->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTRelationalOp *pointer){
    pointer->leftSimpleExpr->accept(this);
    pointer->rightSimpleExpr->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTRtrnStatement *pointer){
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTFactor *pointer){}
void CodeGeneratorVisitorNode::visit( ASTLiteral *pointer){}
void CodeGeneratorVisitorNode::visit( ASTIntLiteral *pointer){}
void CodeGeneratorVisitorNode::visit( ASTFloatLiteral *pointer){}
void CodeGeneratorVisitorNode::visit( ASTBoolLiteral *pointer){}
void CodeGeneratorVisitorNode::visit( ASTColourLiteral *pointer){}
void CodeGeneratorVisitorNode::visit( ASTPadRead *pointer){
    pointer->firstExpression->accept(this);
    pointer->secondExpression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTPadRandi *pointer){
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTPadWidth *pointer){}
void CodeGeneratorVisitorNode::visit( ASTPadHeight *pointer){}
void CodeGeneratorVisitorNode::visit( ASTTerm *pointer){
    for(auto iter = pointer->factors.begin(); iter <pointer->factors.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}
void CodeGeneratorVisitorNode::visit( ASTExpr *pointer){}
void CodeGeneratorVisitorNode::visit( ASTSubExpr *pointer){
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTSimpleExpr *pointer){
    for(auto iter = pointer->terms.begin(); iter <pointer->terms.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}
void CodeGeneratorVisitorNode::visit( ASTUnary *pointer){
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTActualParams *pointer){
    for(auto iter = pointer->expressions.begin(); iter < pointer->expressions.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}
void CodeGeneratorVisitorNode::visit( ASTPixelStatement *pointer){
    for(auto iter = pointer->expressions.begin(); iter < pointer->expressions.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}

void CodeGeneratorVisitorNode::PrintProgram() {
    for(int i=0;i<symbolTable->scopeStack.size();i++){
        auto iter = symbolTable->scopeStack.begin();advance(iter, i);
        for(auto & functionName : functionNames){
            if((*iter).scope.find(functionName) != (*iter).scope.end()) {
                cout << symbolTable->scopeStack[i].scope[functionName]["print"];
            }
        }
    }
}
CodeGeneratorVisitorNode::~CodeGeneratorVisitorNode(){
    functionNames.clear();
    symbolTable.reset();
}


#include "VisitorNodes.h"

void SemanticVisitorNode::visit(ASTNode * pointer){}
void SemanticVisitorNode::visit( ASTStatement *pointer){}
void SemanticVisitorNode::visit( ASTType *pointer){}
void SemanticVisitorNode::visit( ASTFactor *pointer){}
void SemanticVisitorNode::visit( ASTLiteral *pointer){}
void SemanticVisitorNode::visit( ASTTerm *pointer){}
void SemanticVisitorNode::visit( ASTExpr *pointer){}
void SemanticVisitorNode::visit( ASTSubExpr *pointer){}
void SemanticVisitorNode::visit( ASTSimpleExpr *pointer){}
void SemanticVisitorNode::visit( ASTIdentifier *pointer){
    string identifier=pointer->identifier;
    bool exists=symbolTable->CheckIdentifierExists(identifier);
    if(!exists){
        cerr<<"\nSemantic Error: Cannot Call the Identifier: \""<<identifier<<"\" which is not declared"<<endl;
        exit(5);
    }
    currentStoredType=symbolTable->ReturnIdentifierType(identifier);
}

void SemanticVisitorNode::visit(ASTProgram * pointer) {
    Scope initialScope;
    symbolTable->push(initialScope);
    string currentFunctionName=currentStoredFunctionName;
    for(auto iter = pointer->program.begin(); iter < pointer->program.end(); iter++)
    {
        currentStoredFunctionName=currentFunctionName;
        ((*iter))->accept(this);
    }
    symbolTable->pop();
}
void SemanticVisitorNode::visit(ASTBlock * pointer) {
    Scope blockScope;
    symbolTable->push(blockScope);
    string currentFunctionName=currentStoredFunctionName;
    for(auto iter = pointer->statements.begin(); iter < pointer->statements.end(); iter++)
    {
        currentStoredFunctionName=currentFunctionName;
        ((*iter))->accept(this);
    }
    symbolTable->pop();
}
void SemanticVisitorNode::visit(ASTVariableDecl * pointer) {
    assignmentFlag=true;
    pointer->expression->accept(this);
    string storedType=currentStoredType;

    auto iter = symbolTable->scopeStack.end();iter--;

    string identifier=pointer->identifier->identifier;
    string type=pointer->type->value;
    bool exists=symbolTable->CheckIdentifierExists(identifier);
    if(exists){
        cerr<<"\nSemantic Error: Identifier already exists, cannot create Identifier with name: \""<<identifier<<"\""<<endl;
        exit(5);
    }
    if(type!=storedType){
        cerr<<"\nType Mismatch Error: Identifier "<<identifier<<" of type \""<<type<<"\" cannot be assigned \""<<storedType<<"\" type"<<endl;
        exit(6);
    }
    (*iter).scope[identifier]["type"]=type;
    assignmentFlag=false;
}
void SemanticVisitorNode::visit( ASTRtrnStatement *pointer){
    pointer->expression->accept(this);
    string storedType=currentStoredType;
    string functionType=symbolTable->ReturnIdentifierType(currentStoredFunctionName);
    if(functionType=="No Type"){
        cerr<<"\nSemantic Error: Program is not Expected to return a \""<<storedType<<"\" type"<<endl;
        exit(5);
    }
    if(storedType!=functionType){
        cerr<<"\nType Mismatch Error: Function "<<currentStoredFunctionName<<", expected to return \""<<functionType<<"\" type but returned \""<<storedType<<"\" type"<<endl;
        exit(6);
    }
    returnFlag=false;
}
void SemanticVisitorNode::visit( ASTDelayStatement *pointer){
    pointer->expression->accept(this);
    string storedType=currentStoredType;
    if(storedType!="int"){
        cerr<<"\nType Mismatch Error: Delay Statement"<<R"(, expected to have "int" type but has ")"<<storedType<<"\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTPrintStatement *pointer){
    pointer->expression->accept(this);
    string storedType=currentStoredType;
    if(storedType!="int"){
        cerr<<"\nType Mismatch Error: Print Statement"<<R"(, expected to have "int" type but has ")"<<storedType<<"\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTPixelStatement *pointer){
    const size_t size=pointer->expressions.size();
    for (int i = 0; i <size; i++) {
        pointer->expressions[i]->accept(this);
        string storedType=currentStoredType;
        if(storedType!="int"&& i<=(size-2)){
            cerr<<"\nType Mismatch Error: Pixel Statement: "<<pointer->value<<" Parameter "<<(i+1)<<R"(, expected to have "int" type but has ")"<<storedType<<"\" type"<<endl;
            exit(6);
        }
        else if(storedType!="colour"&& i==(size-1)){
            cerr<<"\nType Mismatch Error: Pixel Statement: "<<pointer->value<<" Parameter "<<(i+1)<<R"(, expected to have "colour" type but has ")"<<storedType<<"\" type"<<endl;
            exit(6);
        }
    }
}
void SemanticVisitorNode::visit( ASTForStatement *pointer){
    if(pointer->variableDecl!= nullptr){
        pointer->variableDecl->accept(this);
    }
    pointer->expression->accept(this);
    if(pointer->assignment!= nullptr) {
        pointer->assignment->accept(this);
    }
    pointer->block->accept(this);
}
void SemanticVisitorNode::visit( ASTWhileStatement *pointer){
    pointer->expression->accept(this);
    pointer->block->accept(this);
}
void SemanticVisitorNode::visit( ASTIfStatement *pointer){
    pointer->expression->accept(this);
    pointer->firstBlock->accept(this);
    if(pointer->secondBlock!= nullptr){
        pointer->secondBlock->accept(this);
    }
}
void SemanticVisitorNode::visit( ASTAssignment *pointer){
    assignmentFlag=true;
    string identifier=pointer->identifier->identifier;
    bool exists=symbolTable->CheckIdentifierExists(identifier);
    if(!exists){
        cerr<<"\nSemantic Error: Cannot Assign a value to the Identifier: \""<<identifier<<"\" which is not declared"<<endl;
        exit(5);
    }
    pointer->expression->accept(this);
    string storedType=currentStoredType;
    string type=symbolTable->ReturnIdentifierType(identifier);
    if(type!=storedType){
        cerr<<"\nType Mismatch Error: Identifier "<<identifier<<" of type \""<<type<<"\" cannot be assigned \""<<storedType<<"\" type"<<endl;
        exit(6);
    }
    assignmentFlag=false;
}
void SemanticVisitorNode::visit( ASTFunctionDecl *pointer){
    returnFlag=true;
    auto iter = symbolTable->scopeStack.end();iter--;

    string identifier=pointer->identifier->identifier;
    string type=pointer->type->value;
    bool exists=symbolTable->CheckIdentifierExists(identifier);
    if(exists){
        cerr<<"\nSemantic Error: Identifier already exists, cannot create Function with name: \""<<identifier<<"\""<<endl;
        exit(5);
    }
    (*iter).scope[identifier]["type"]=type;
    currentStoredFunctionName=identifier;
    if(pointer->formalParams!= nullptr){
        pointer->formalParams->accept(this);
    }
    pointer->block->accept(this);
    if(pointer->formalParams!= nullptr) {
        RemoveFunctionParameters(pointer->formalParams);
    }
    if(returnFlag){
        cerr<<"\nSemantic Error: Function with name: \""<<identifier<<"\" has no \""<<type<<"\" type return"<<endl;
        exit(5);
    }
    returnFlag=false;
}

void SemanticVisitorNode::RemoveFunctionParameters(const shared_ptr<ASTFormalParams>& pointer) const {
    for(auto iter = pointer->formalParams.begin(); iter < pointer->formalParams.end(); iter++)
    {
        string identifier=((*iter))->identifier->identifier;
        //Removing variable from outer scope
        auto iter2 = symbolTable->scopeStack.end();iter2--;
        auto iter3 = (*iter2).scope.find(identifier);
        if (iter3 != (*iter2).scope.end()) {
            (*iter2).scope.erase(iter3);
        }
    }
}
void SemanticVisitorNode::visit( ASTFunctionCall *pointer){
    auto iter = symbolTable->scopeStack.end();iter--;

    string identifier=pointer->identifier->identifier;
    bool exists=symbolTable->CheckIdentifierExists(identifier);
    if(!exists){
        cerr<<"\nSemantic Error: Function does not exist, cannot call function: \""<<identifier<<"\""<<endl;
        exit(5);
    }
    currentStoredFunctionName=identifier;
    if(pointer->actualParams!= nullptr){
        pointer->actualParams->accept(this);
    }

    currentStoredType=symbolTable->ReturnIdentifierType(identifier);
}
void SemanticVisitorNode::visit( ASTFormalParams *pointer){
    for(auto iter = pointer->formalParams.begin(); iter < pointer->formalParams.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}
void SemanticVisitorNode::visit( ASTFormalParam *pointer){
    auto iter = symbolTable->scopeStack.end();iter--;
    string identifier=pointer->identifier->identifier;
    string type=pointer->type->value;
    bool exists=symbolTable->CheckIdentifierExists(identifier);
    if(exists){
        cerr<<"\nSemantic Error: Formal Parameter Identifier already exists, cannot create Function parameter with name: \""<<identifier<<"\""<<endl;
        exit(5);
    }
    (*iter).scope[currentStoredFunctionName]["FormalParam"]+=" "+type;
    (*iter).scope[identifier]["type"]=type;
}

void SemanticVisitorNode::visit( ASTMultiplicativeOp *pointer){
    pointer->leftFactor->accept(this);
    string leftType=currentStoredType;
    zeroFlag=false;
    pointer->rightFactor->accept(this);
    string rightType=currentStoredType;
    if(leftType!=rightType){
        cerr<<"\nType Mismatch Error: Cannot apply the "<<pointer->multiplicativeOp<<" operator between \""<<leftType<<"\" type and \""<<rightType<<"\" type"<<endl;
        exit(6);
    }
    if(pointer->multiplicativeOp=="/" && zeroFlag){
        cerr<<"\nSemantic Error: Cannot Divide by 0"<<endl;
        exit(7);
    }
    zeroFlag=false;
}
void SemanticVisitorNode::visit( ASTAdditiveOp *pointer){
    pointer->leftTerm->accept(this);
    string leftType=currentStoredType;
    pointer->rightTerm->accept(this);
    string rightType=currentStoredType;
    if(leftType!=rightType){
        cerr<<"\nType Mismatch Error: Cannot apply the "<<pointer->additiveOp<<" operator between \""<<leftType<<"\" type and \""<<rightType<<"\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTRelationalOp *pointer){
    if(assignmentFlag){
        cerr<<"\nSemantic Error: Cannot have an Assignment with a relational Operator, operator: \""<<pointer->relationalOp<<"\""<<endl;
        exit(5);
    }
    pointer->leftSimpleExpr->accept(this);
    string leftType=currentStoredType;
    pointer->rightSimpleExpr->accept(this);
    string rightType=currentStoredType;
    if(leftType!=rightType){
        cerr<<"\nType Mismatch Error: Cannot apply the "<<pointer->relationalOp<<" operator between \""<<leftType<<"\" type and \""<<rightType<<"\" type"<<endl;
        exit(6);
    }
    currentStoredType="bool";
}
void SemanticVisitorNode::visit( ASTIntLiteral *pointer){
    currentStoredType="int";
    if(pointer->value==0){
        zeroFlag=true;
    }
}
void SemanticVisitorNode::visit( ASTFloatLiteral *pointer){
    currentStoredType="float";
    if(pointer->value==0){
        zeroFlag=true;
    }
}
void SemanticVisitorNode::visit( ASTBoolLiteral *pointer){
    currentStoredType="bool";
}
void SemanticVisitorNode::visit( ASTColourLiteral *pointer){
    currentStoredType="colour";
}
void SemanticVisitorNode::visit( ASTPadRead *pointer){
    pointer->firstExpression->accept(this);
    string Type=currentStoredType;
    if(Type!="int"){
        cerr<<"\nType Mismatch Error: PadRead can only take \"int\" type"<<endl;
        exit(6);
    }
    pointer->secondExpression->accept(this);
    Type=currentStoredType;
    if(Type!="int"){
        cerr<<"\nType Mismatch Error: PadRead can only take \"int\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTPadRandi *pointer){
    pointer->expression->accept(this);
    string Type=currentStoredType;
    if(Type!="int"){
        cerr<<"\nType Mismatch Error: PadRandi can only take \"int\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTPadWidth *pointer){
    currentStoredType="int";
}
void SemanticVisitorNode::visit( ASTPadHeight *pointer){
    currentStoredType="int";
}

void SemanticVisitorNode::visit( ASTUnary *pointer){
    string uOperator=pointer->UnaryOperator;
    pointer->expression->accept(this);
    string Type=currentStoredType;
    if(!(uOperator=="not"&&Type=="bool"||(uOperator=="-"&&(Type=="int"||Type=="float")))){
        cerr<<"\nType Mismatch Error: Unary operator: "<<uOperator<<" cannot be followed by \""<<Type<<"\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTActualParams *pointer){
    string allParams=symbolTable->ReturnFunctionParameters(currentStoredFunctionName);
    stringstream stream(allParams);
    string currentType;
    getline(stream, currentType, ' ');//Removing the first space
    if(allParams.empty()&&!pointer->expressions.empty()){
        cerr<<"\nSemantic Error: Function Call \""<<currentStoredFunctionName<<"\" has unwanted parameters, when should be given no parameters"<<endl;
        exit(5);
    }
    for(auto iter = pointer->expressions.begin(); iter < pointer->expressions.end(); iter++)
    {
        currentType="";
        getline(stream, currentType, ' ');
        ((*iter))->accept(this);
        string actualParamType=currentStoredType;
        if(currentType.empty()){
            cerr<<"\nSemantic Error: Function Call \""<<currentStoredFunctionName<<"\" has unwanted parameters, when should be only given \""<<allParams<<"\" parameters"<<endl;
            exit(5);
        }
        if(actualParamType!=currentType){
            cerr<<"\nType Mismatch Error: Function Call \""<<currentStoredFunctionName<<"\" given parameter with \""<<actualParamType<<"\" type, when should be given \""<<currentType<<"\" type"<<endl;
            exit(6);
        }
    }

    if(!stream.eof()){
        cerr<<"\nSemantic Error: Function Call \""<<currentStoredFunctionName<<"\" has missing parameters, when should be given \""<<allParams<<"\" parameters"<<endl;
        exit(5);
    }
}



SemanticVisitorNode::~SemanticVisitorNode() {
    symbolTable.reset();
}
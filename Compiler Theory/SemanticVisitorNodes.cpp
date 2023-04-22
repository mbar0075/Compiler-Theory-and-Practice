//Inclusion of the relevant header File
#include "VisitorNodes.h"

//visit methods for the relevant AST Nodes passed as parameters
void SemanticVisitorNode::visit(ASTNode * pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void SemanticVisitorNode::visit( ASTStatement *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void SemanticVisitorNode::visit( ASTType *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void SemanticVisitorNode::visit( ASTFactor *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void SemanticVisitorNode::visit( ASTLiteral *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void SemanticVisitorNode::visit( ASTTerm *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void SemanticVisitorNode::visit( ASTExpr *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void SemanticVisitorNode::visit( ASTSimpleExpr *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void SemanticVisitorNode::visit(ASTClearStatement *pointer) {/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void SemanticVisitorNode::visit(ASTProgram * pointer) {
    //Creating a new Scope and pushing it onto the symbol Table
    Scope initialScope;
    symbolTable->push(initialScope);
    string currentFunctionName=currentStoredFunctionName;
    //Iterating through all the statements in the program, and resetting the currentStoredFunctionName, for every iteration
    for(auto iter = pointer->program.begin(); iter < pointer->program.end(); iter++)
    {
        currentStoredFunctionName=currentFunctionName;
        ((*iter))->accept(this);
    }
    //Popping initialScope from the symbol Table
    symbolTable->pop();
}
void SemanticVisitorNode::visit(ASTBlock * pointer) {
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
void SemanticVisitorNode::visit(ASTVariableDecl * pointer) {
    //Calling the accept method on the expression
    pointer->expression->accept(this);
    //Retrieving and storing the currentStoredType
    string storedType=currentStoredType;
    //Retrieving access to last scope pushed in the symbol table
    auto iter = symbolTable->scopeStack.end();iter--;
    string identifier=pointer->identifier->identifier;
    string type=pointer->type->value;
    //Calling CheckIdentifierExists function, to check whether identifier exists
    bool exists=symbolTable->CheckIdentifierExists(identifier,false);
    //Displaying Error, since Identifier exists
    if(exists){
        cerr<<"\nSemantic Error: Identifier already exists, cannot create Identifier with name: \""<<identifier<<"\""<<endl;
        exit(5);
    }
    //Checking for Type Mismatch Error
    if(type!=storedType){
        cerr<<"\nType Mismatch Error: Identifier "<<identifier<<" of type \""<<type<<"\" cannot be assigned \""<<storedType<<"\" type"<<endl;
        exit(6);
    }
    //Saving identifier in the last scope pushed in the symbol table with its type
    (*iter).scope[identifier]["type"]=type;
}
void SemanticVisitorNode::visit( ASTRtrnStatement *pointer){
    //Retrieving the Identifier Type
    string functionType=symbolTable->ReturnIdentifierType(currentStoredFunctionName);
    //Calling accept method on expression, and retrieving the currentStoredType
    pointer->expression->accept(this);
    string storedType=currentStoredType;
    //Checking for Semantic Error
    if(functionType=="No Type"){
        cerr<<"\nSemantic Error: Program is not Expected to return a \""<<storedType<<"\" type"<<endl;
        exit(5);
    }
    //Checking for Type Mismatch Error
    if(storedType!=functionType){
        cerr<<"\nType Mismatch Error: Function \""<<currentStoredFunctionName<<"()\", expected to return \""<<functionType<<"\" type but returned \""<<storedType<<"\" type"<<endl;
        exit(6);
    }
    //Setting returnFlag to false
    returnFlag=false;
}
void SemanticVisitorNode::visit( ASTDelayStatement *pointer){
    //Calling accept method on expression, and retrieving the currentStoredType
    pointer->expression->accept(this);
    string storedType=currentStoredType;
    //Checking for Type Mismatch Error
    if(storedType!="int"){
        cerr<<"\nType Mismatch Error: Delay Statement"<<R"(, expected to have "int" type but has ")"<<storedType<<"\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTPrintStatement *pointer){
    //Calling accept method on expression, and retrieving the currentStoredType
    pointer->expression->accept(this);
    string storedType=currentStoredType;
    //Checking for Type Mismatch Error
    if(storedType!="int"){
        cerr<<"\nType Mismatch Error: Print Statement"<<R"(, expected to have "int" type but has ")"<<storedType<<"\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTPixelStatement *pointer){
    //Iterating through all the expressions
    const size_t size=pointer->expressions.size();
    for (int i = 0; i <size; i++) {
        //Calling accept method on expression, and retrieving the currentStoredType
        pointer->expressions[i]->accept(this);
        string storedType=currentStoredType;
        //Checking for Type Mismatch Error, if first parameters are not "int" type and last parameter "colour" type
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
    //Calling the respective accept methods, if pointers are not null
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
    //Calling the respective accept methods
    pointer->expression->accept(this);
    pointer->block->accept(this);
}
void SemanticVisitorNode::visit( ASTIfStatement *pointer){
    //Calling the respective accept methods, if pointers are not null
    pointer->expression->accept(this);
    pointer->firstBlock->accept(this);
    if(pointer->secondBlock!= nullptr){
        pointer->secondBlock->accept(this);
    }
}
void SemanticVisitorNode::visit( ASTAssignment *pointer){
    //Retrieving and storing Identifier
    string identifier=pointer->identifier->identifier;
    //Retrieving whether Identifier exist from the symbol table
    bool exists=symbolTable->CheckIdentifierExists(identifier,false);
    //Checking for Semantic Error
    if(!exists){
        cerr<<"\nSemantic Error: Cannot Assign a value to the Identifier: \""<<identifier<<"\" which is not declared"<<endl;
        exit(5);
    }
    //Calling accept method on expression, and retrieving the currentStoredType
    pointer->expression->accept(this);
    string storedType=currentStoredType;
    //Retrieving Type for Identifier from symbol table
    string type=symbolTable->ReturnIdentifierType(identifier);
    //Checking for Type Mismatch Error
    if(type!=storedType){
        cerr<<"\nType Mismatch Error: Identifier "<<identifier<<" of type \""<<type<<"\" cannot be assigned \""<<storedType<<"\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTFunctionDecl *pointer){
    //Setting returnFlag to true
    returnFlag=true;
    //Retrieving access to the last scope pushed in the symbol table
    auto iter = symbolTable->scopeStack.end();iter--;
    //Retrieving identifier and type
    string identifier=pointer->identifier->identifier;
    string type=pointer->type->value;
    //Retrieving whether Identifier exists in the symbol table
    bool exists=symbolTable->CheckIdentifierExists(identifier,false);
    //Checking for Semantic Error
    if(exists){
        cerr<<"\nSemantic Error: Identifier already exists, cannot create Function with name: \""<<identifier<<"()\""<<endl;
        exit(5);
    }
    //Updating symbol table for current Identifier, and updating currentStoredFunctionName
    (*iter).scope[identifier]["type"]=type;
    currentStoredFunctionName=identifier;
    //Checking whether formalParams are not null, and if so calling the respective accept method
    if(pointer->formalParams!= nullptr){
        pointer->formalParams->accept(this);
    }
    else{
        //Else setting the Formal Parameters to None
        auto iter2 = symbolTable->scopeStack.end();iter2--;
        (*iter2).scope[currentStoredFunctionName]["FormalParam"]="";
    }
    //Calling accept method on block
    pointer->block->accept(this);
    //Checking whether formalParams are not null, and if so calling the RemoveFunctionParameters method, parameters from current scope
    if(pointer->formalParams!= nullptr) {
        RemoveFunctionParameters(pointer->formalParams);
    }
    //Checking for Semantic Error
    if(returnFlag){
        cerr<<"\nSemantic Error: Function with name: \""<<identifier<<"()\" has no \""<<type<<"\" type return"<<endl;
        exit(5);
    }
    //Setting returnFlag to false
    returnFlag=false;
}
//Method which Removes a Function's parameters from the current scope, given a pointer of Formal Parameters
void SemanticVisitorNode::RemoveFunctionParameters(const shared_ptr<ASTFormalParams>& pointer) const {
    //Looping through all the Formal Params
    for(auto iter = pointer->formalParams.begin(); iter < pointer->formalParams.end(); iter++)
    {
        //Erasing the variable from the current scope
        string identifier=((*iter))->identifier->identifier;
        auto iter2 = symbolTable->scopeStack.end();iter2--;
        auto iter3 = (*iter2).scope.find(identifier);
        if (iter3 != (*iter2).scope.end()) {
            (*iter2).scope.erase(iter3);
        }
    }
}
void SemanticVisitorNode::visit( ASTFunctionCall *pointer){
    //Retrieving access to the last scope pushed in the symbol table
    auto iter = symbolTable->scopeStack.end();iter--;
    //Retrieving and storing Identifier
    string identifier=pointer->identifier->identifier;
    //Retrieving whether Identifier exists in the symbol table
    bool exists=symbolTable->CheckIdentifierExists(identifier,true);
    //Checking for Semantic Error
    if(!exists){
        cerr<<"\nSemantic Error: Function does not exist, cannot call function: \""<<identifier<<"()\""<<endl;
        exit(5);
    }
    //Updating currentStoredFunctionName, and calling the actualParams accept method, if pointer is not null
    currentStoredFunctionName=identifier;
    if(pointer->actualParams!= nullptr){
        pointer->actualParams->accept(this);
    }
    else{
        //Retrieving the params from the symbol table
        string allParams=symbolTable->ReturnFunctionParameters(currentStoredFunctionName);
        stringstream stream(allParams);
        //Checking for Semantic Error, when Function Call has missing parameters
        if(!stream.eof()&&!allParams.empty()){
            cerr<<"\nSemantic Error: Function Call \""<<currentStoredFunctionName<<"()\" has missing parameters, when should be given \""<<allParams<<"\" parameters"<<endl;
            exit(5);
        }
    }
    //Updating the currentStoredType from symbol table
    currentStoredType=symbolTable->ReturnIdentifierType(identifier);
}
void SemanticVisitorNode::visit( ASTFormalParams *pointer){
    //Looping through all the formalParams vector and calling the accept method
    for(auto iter = pointer->formalParams.begin(); iter < pointer->formalParams.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}
void SemanticVisitorNode::visit( ASTFormalParam *pointer){
    //Retrieving access to the last scope pushed in the symbol table
    auto iter = symbolTable->scopeStack.end();iter--;
    //Retrieving Identifier and type
    string identifier=pointer->identifier->identifier;
    string type=pointer->type->value;
    //Retrieving whether Identifier exists in the symbol table
    bool exists=symbolTable->CheckIdentifierExists(identifier,false);
    //Checking for Semantic Error
    if(exists){
        cerr<<"\nSemantic Error: Formal Parameter Identifier already exists, cannot create Function parameter with name: \""<<identifier<<"\""<<endl;
        exit(5);
    }
    //Updating the symbol table with the type and Formal Params
    (*iter).scope[currentStoredFunctionName]["FormalParam"]+=" "+type;
    (*iter).scope[identifier]["type"]=type;
}
void SemanticVisitorNode::visit( ASTMultiplicativeOp *pointer){
    //Calling the accept method on the leftFactor, and storing leftType
    pointer->leftFactor->accept(this);
    string leftType=currentStoredType;
    //Setting zeroFlag to false
    zeroFlag=false;
    //Calling the accept method on the rightFactor, and storing rightType
    pointer->rightFactor->accept(this);
    string rightType=currentStoredType;
    //Checking for Type Mismatch Error
    if(leftType!=rightType){
        cerr<<"\nType Mismatch Error: Cannot apply the "<<pointer->multiplicativeOp<<" operator between \""<<leftType<<"\" type and \""<<rightType<<"\" type"<<endl;
        exit(6);
    }
    //Checking for Semantic Error , Division by zero
    if(pointer->multiplicativeOp=="/" && zeroFlag){
        cerr<<"\nSemantic Error: Cannot Divide by 0"<<endl;
        exit(7);
    }
    //Setting zeroFlag to false
    zeroFlag=false;
}
void SemanticVisitorNode::visit( ASTAdditiveOp *pointer){
    //Calling the accept method on the leftTerm, and storing leftType
    pointer->leftTerm->accept(this);
    string leftType=currentStoredType;
    //Calling the accept method on the rightTerm, and storing rightType
    pointer->rightTerm->accept(this);
    string rightType=currentStoredType;
    //Checking for Type Mismatch Error
    if(leftType!=rightType){
        cerr<<"\nType Mismatch Error: Cannot apply the "<<pointer->additiveOp<<" operator between \""<<leftType<<"\" type and \""<<rightType<<"\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTRelationalOp *pointer){
    //Calling the accept method on the leftSimpleExpr, and storing leftType
    pointer->leftSimpleExpr->accept(this);
    string leftType=currentStoredType;
    //Calling the accept method on the rightSimpleExpr, and storing rightType
    pointer->rightSimpleExpr->accept(this);
    string rightType=currentStoredType;
    //Checking for Type Mismatch Error
    if(leftType!=rightType){
        cerr<<"\nType Mismatch Error: Cannot apply the "<<pointer->relationalOp<<" operator between \""<<leftType<<"\" type and \""<<rightType<<"\" type"<<endl;
        exit(6);
    }
    //Setting currentStoredType to bool
    currentStoredType="bool";
}
void SemanticVisitorNode::visit( ASTIntLiteral *pointer){
    //Setting currentStoredType to int, and Checking whether to set zeroFlag to true
    currentStoredType="int";
    if(pointer->value==0){
        zeroFlag=true;
    }
}
void SemanticVisitorNode::visit( ASTFloatLiteral *pointer){
    //Setting currentStoredType to float, and Checking whether to set zeroFlag to true
    currentStoredType="float";
    if(pointer->value==0){
        zeroFlag=true;
    }
}
void SemanticVisitorNode::visit( ASTBoolLiteral *pointer){
    //Setting currentStoredType to bool
    currentStoredType="bool";
}
void SemanticVisitorNode::visit( ASTColourLiteral *pointer){
    //Setting currentStoredType to colour
    currentStoredType="colour";
}
void SemanticVisitorNode::visit( ASTPadRead *pointer){
    //Calling accept method on first Expression, and Retrieving Type
    pointer->firstExpression->accept(this);
    string Type=currentStoredType;
    //Checking for Type Mismatch Error
    if(Type!="int"){
        cerr<<"\nType Mismatch Error: PadRead can only take \"int\" type"<<endl;
        exit(6);
    }
    //Calling accept method on second Expression, and Retrieving Type
    pointer->secondExpression->accept(this);
    Type=currentStoredType;
    //Checking for Type Mismatch Error
    if(Type!="int"){
        cerr<<"\nType Mismatch Error: PadRead can only take \"int\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTPadRandi *pointer){
    //Calling accept method on expression, and Retrieving Type
    pointer->expression->accept(this);
    string Type=currentStoredType;
    //Checking for Type Mismatch Error
    if(Type!="int"){
        cerr<<"\nType Mismatch Error: PadRandi can only take \"int\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTPadWidth *pointer){
    //Setting the currentStoredType to int
    currentStoredType="int";
}
void SemanticVisitorNode::visit( ASTPadHeight *pointer){
    //Setting the currentStoredType to int
    currentStoredType="int";
}
void SemanticVisitorNode::visit( ASTIdentifier *pointer){
    string identifier=pointer->identifier;
    //Calling CheckIdentifierExists function, to check whether identifier exists from symbol table
    bool exists=symbolTable->CheckIdentifierExists(identifier,false);
    //Displaying Error, since Identifier does not exist
    if(!exists){
        cerr<<"\nSemantic Error: Cannot Call the Identifier: \""<<identifier<<"\" which is not declared"<<endl;
        exit(5);
    }
    //Setting the currentStored Type to the identifier Type, via look-up from symbol table
    currentStoredType=symbolTable->ReturnIdentifierType(identifier);
}
void SemanticVisitorNode::visit( ASTUnary *pointer){
    //Retrieving unaryOperator
    string uOperator=pointer->UnaryOperator;
    //Calling accept method on expression, and retrieving the currentStoredType
    pointer->expression->accept(this);
    string Type=currentStoredType;
    //Checking for Type Mismatch Error
    if(!(uOperator=="not"&&Type=="bool"||(uOperator=="-"&&(Type=="int"||Type=="float")))){
        cerr<<"\nType Mismatch Error: Unary operator: "<<uOperator<<" cannot be followed by \""<<Type<<"\" type"<<endl;
        exit(6);
    }
}
void SemanticVisitorNode::visit( ASTActualParams *pointer){
    //Retrieving the params from symbol table
    string allParams=symbolTable->ReturnFunctionParameters(currentStoredFunctionName);
    stringstream stream(allParams);
    string currentType;
    //Removing the first space
    getline(stream, currentType, ' ');
    //Checking for Semantic Error, whether function call has unwanted parameters
    if(allParams.empty()&&!pointer->expressions.empty()){
        cerr<<"\nSemantic Error: Function Call \""<<currentStoredFunctionName<<"()\" has unwanted parameters, when should be given no parameters"<<endl;
        exit(5);
    }
    //Looping through all the actual params (expressions)
    for(auto iter = pointer->expressions.begin(); iter < pointer->expressions.end(); iter++)
    {
        //Resetting currentType
        currentType="";
        //Retrieving next Type
        getline(stream, currentType, ' ');
        //Calling accept method on current expression, and retrieving currentStoredType
        ((*iter))->accept(this);
        string actualParamType=currentStoredType;
        //Checking for Semantic Error, whether function call has unwanted parameters
        if(currentType.empty()){
            cerr<<"\nSemantic Error: Function Call \""<<currentStoredFunctionName<<"()\" has unwanted parameters, when should be only given \""<<allParams<<"\" parameters"<<endl;
            exit(5);
        }
        //Checking for Type Mismatch Error, whether function call parameter does not match type
        if(actualParamType!=currentType){
            cerr<<"\nType Mismatch Error: Function Call \""<<currentStoredFunctionName<<"()\" given parameter with \""<<actualParamType<<"\" type, when should be given \""<<currentType<<"\" type"<<endl;
            exit(6);
        }
    }
    //Checking for Semantic Error, whether function call has missing parameters
    if(!stream.eof()){
        cerr<<"\nSemantic Error: Function Call \""<<currentStoredFunctionName<<"()\" has missing parameters, when should be given \""<<allParams<<"\" parameters"<<endl;
        exit(5);
    }
}
void SemanticVisitorNode::visit( ASTSubExpr *pointer){
    pointer->expression->accept(this);
}
//Destructor for class
SemanticVisitorNode::~SemanticVisitorNode() {
    symbolTable.reset();
}
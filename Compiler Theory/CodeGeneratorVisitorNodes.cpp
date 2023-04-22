//Inclusion of the relevant header File
#include "VisitorNodes.h"

void CodeGeneratorVisitorNode::visit( ASTNode *pointer){}
void CodeGeneratorVisitorNode::visit( ASTStatement *pointer){}
void CodeGeneratorVisitorNode::visit( ASTLiteral *pointer){}
void CodeGeneratorVisitorNode::visit( ASTType *pointer){}
void CodeGeneratorVisitorNode::visit( ASTFactor *pointer){}
void CodeGeneratorVisitorNode::visit( ASTExpr *pointer){}
void CodeGeneratorVisitorNode::visit( ASTFormalParam *pointer){}
void CodeGeneratorVisitorNode::visit( ASTFormalParams *pointer){}
void CodeGeneratorVisitorNode::visit( ASTSimpleExpr *pointer){}
void CodeGeneratorVisitorNode::visit( ASTTerm *pointer){}
void CodeGeneratorVisitorNode::visit( ASTProgram *pointer){
    //Creating a new Scope and pushing it onto the symbol Table
    Scope initialScope;
    symbolTable->push(initialScope);
    //Setting the currentStoreFunctionName to main
    currentStoredFunctionName="main";
    functionNames.emplace_back("main");
    //Updating the print key for the currentStoredFunctionName in the symbol table
    printList[currentStoredFunctionName]+=".main\n";
    string currentFunctionName=currentStoredFunctionName;
    //Iterating through all the statements in the program, and resetting the currentStoredFunctionName, for every iteration
    for(auto iter = pointer->program.begin(); iter < pointer->program.end(); iter++)
    {
        currentStoredFunctionName=currentFunctionName;
        ((*iter))->accept(this);
    }
    currentStoredFunctionName=currentFunctionName;
    //Updating the print key for the currentStoredFunctionName in the symbol table
    printList[currentStoredFunctionName]+="halt\n";
    PrintProgram();
    //Popping initialScope from the symbol Table
    symbolTable->pop();
}
void CodeGeneratorVisitorNode::visit( ASTBlock *pointer){
    //Creating a new Scope and pushing it onto the symbol Table
    Scope blockScope;
    symbolTable->push(blockScope);
    //Incrementing frameIndex and adding oframe instruction to the printList
    frameIndex++;
    printList[currentStoredFunctionName]+="oframe\n";
    string currentFunctionName=currentStoredFunctionName;
    //Iterating through all the statements in the block, and resetting the currentStoredFunctionName, for every iteration
    for(auto iter = pointer->statements.begin(); iter < pointer->statements.end(); iter++)
    {
        currentStoredFunctionName=currentFunctionName;
        ((*iter))->accept(this);
    }
    //Decrementing frameIndex and adding cframe instruction to the printList
    frameIndex--;
    printList[currentStoredFunctionName]+="cframe\n";
    //Popping blockScope from the symbol Table
    symbolTable->pop();
}
void CodeGeneratorVisitorNode::visit( ASTPrintStatement *pointer){
    //Accepting expression, and adding the respective print instruction
    pointer->expression->accept(this);
    printList[currentStoredFunctionName]+="print\n";
}
void CodeGeneratorVisitorNode::visit( ASTDelayStatement *pointer){
    //Accepting expression, and adding the respective delay instruction
    pointer->expression->accept(this);
    printList[currentStoredFunctionName]+="delay\n";
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
    //Adding the relevant instructions to the printList
//    printList[currentStoredFunctionName] += "push 1\nalloc\n";
    pointer->identifier->accept(this);
    pointer->expression->accept(this);
    auto iter = symbolTable->scopeStack.end();iter--;
    printList[currentStoredFunctionName] += "push "+to_string((*iter).scope.size())+"\n";
    printList[currentStoredFunctionName] += "push "+to_string(frameIndex)+"\n";
    printList[currentStoredFunctionName] += "st\n";
}
void CodeGeneratorVisitorNode::visit( ASTAssignment *pointer){
    pointer->identifier->accept(this);
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTFunctionDecl *pointer){
    //Updating the currentStoredFunctionName, to the identifier
    string identifier=pointer->identifier->identifier;
    currentStoredFunctionName=identifier;
    //Adding the function Name instruction
    printList[currentStoredFunctionName]+="\n."+identifier+"\n";
    functionNames.emplace_back(identifier);
    //Accepting block
    pointer->block->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTFunctionCall *pointer){
    //Checking whether actualParams is not a nullptr
    if(pointer->actualParams!= nullptr) {
        pointer->actualParams->accept(this);
    }
    else{
        printList[currentStoredFunctionName] +="push 0";
    }
    //Adding the push functionName instruction and call instruction to the printList
    printList[currentStoredFunctionName]+= "\npush ." +pointer->identifier->identifier + "\ncall\n";
}
void CodeGeneratorVisitorNode::visit( ASTIdentifier *pointer){
    string identifier=pointer->identifier;
    bool exists=symbolTable->CheckIdentifierExists(identifier,false);
    //Checking whether Identifier exists
    if(exists) {
        //Calling CheckIdentifierExists function, to check whether identifier exists from symbol table
        printList[currentStoredFunctionName] += "push " + symbolTable->ReturnIdentifierAddress(identifier) + "\n";
    }
    else {
        auto iter = symbolTable->scopeStack.end();iter--;
        (*iter).scope[identifier]["Address"] ="[" + to_string(frameIndex) + ":" + to_string((*iter).scope.size()+1) + "]";
        printList[currentStoredFunctionName] += "push 1\nalloc\n";
    }
}
void CodeGeneratorVisitorNode::visit( ASTMultiplicativeOp *pointer){
    //Accepting right and left factors
    pointer->rightFactor->accept(this);
    pointer->leftFactor->accept(this);
    //Adding the respective instructions to the printList
    if(pointer->multiplicativeOp=="*"){
        printList[currentStoredFunctionName]+="mul\n";
    }
    else if(pointer->multiplicativeOp=="/"){
        printList[currentStoredFunctionName]+="div\n";
    }
    else if(pointer->multiplicativeOp=="or"){
        //Since max of 1 or something is 1, thus having either 1 will result in 1
        printList[currentStoredFunctionName]+="max\n";
    }
}
void CodeGeneratorVisitorNode::visit( ASTAdditiveOp *pointer){
    //Accepting right and left terms
    pointer->rightTerm->accept(this);
    pointer->leftTerm->accept(this);
    //Adding the respective instructions to the printList
    if(pointer->additiveOp=="+"){
        printList[currentStoredFunctionName]+="add\n";
    }
    else if(pointer->additiveOp=="-"){
        printList[currentStoredFunctionName]+="sub\n";
    }
    else if(pointer->additiveOp=="and"){
        //Since min of 1 or something is 0, and thus both need to be 1
        printList[currentStoredFunctionName]+="min\n";
    }
}
void CodeGeneratorVisitorNode::visit( ASTRelationalOp *pointer){
    //Accepting right and left simple Expressions
    pointer->rightSimpleExpr->accept(this);
    pointer->leftSimpleExpr->accept(this);
    //Adding the respective instructions to the printList
    if(pointer->relationalOp=="<"){
        printList[currentStoredFunctionName]+="lt\n";
    }
    else if(pointer->relationalOp==">"){
        printList[currentStoredFunctionName]+="gt\n";
    }
    else if(pointer->relationalOp=="=="||pointer->relationalOp=="!="){
        printList[currentStoredFunctionName]+="eq\n";
    }
    else if(pointer->relationalOp==">="){
        printList[currentStoredFunctionName]+="ge\n";
    }
    else if(pointer->relationalOp=="<="){
        printList[currentStoredFunctionName]+="le\n";
    }
}
void CodeGeneratorVisitorNode::visit( ASTRtrnStatement *pointer){
    //Accepting expression, and adding the respective ret instruction to the printList
    pointer->expression->accept(this);
    printList[currentStoredFunctionName]+="ret\n";
}
void CodeGeneratorVisitorNode::visit(ASTClearStatement *pointer) {
    //Adding the respective push instruction, followed by clear instruction to the printList
    printList[currentStoredFunctionName]+="push "+pointer->value+"\n";
    printList[currentStoredFunctionName]+="clear\n";
}
void CodeGeneratorVisitorNode::visit( ASTIntLiteral *pointer){
    //Adding the respective push instruction based on negative flag to the printList
    if(negativeFlag){
        printList[currentStoredFunctionName] += "push " + to_string(-pointer->value) + "\n";
    }
    else {
        printList[currentStoredFunctionName] += "push " + to_string(pointer->value) + "\n";
    }
}
void CodeGeneratorVisitorNode::visit( ASTFloatLiteral *pointer){
    //Adding the respective push instruction, and transforming float to string to the printList
    stringstream floatValue;
    //Adding the respective push instruction based on negative flag to the printList
    if(negativeFlag){
        floatValue << -pointer->value;
        printList[currentStoredFunctionName]+="push "+floatValue.str()+"\n";
    }
    else {
        floatValue << pointer->value;
        printList[currentStoredFunctionName]+="push "+floatValue.str()+"\n";
    }
}
void CodeGeneratorVisitorNode::visit( ASTBoolLiteral *pointer){
    //Changing value if the notFlag is set
    bool value=pointer->value;
    if(notFlag){
        value=!value;
    }
    //Adding the respective push instruction, based on boolean value to the printList
    if(value){
        printList[currentStoredFunctionName]+="push 1\n";
    }
    else{
        printList[currentStoredFunctionName]+="push 0\n";
    }
}
void CodeGeneratorVisitorNode::visit( ASTColourLiteral *pointer){
    //Adding the respective push instruction to the printList
    printList[currentStoredFunctionName]+="push "+pointer->value+"\n";
}
void CodeGeneratorVisitorNode::visit( ASTPadRead *pointer){
    //Accepting the second and first expressions
    pointer->secondExpression->accept(this);
    pointer->firstExpression->accept(this);
    //Adding the read instruction to the printList
    printList[currentStoredFunctionName]+="read\n";
}
void CodeGeneratorVisitorNode::visit( ASTPadRandi *pointer){
    pointer->expression->accept(this);
    printList[currentStoredFunctionName]+="irnd\n";
}
void CodeGeneratorVisitorNode::visit( ASTPadWidth *pointer){
    //Adding the width instruction to the printList
    printList[currentStoredFunctionName]+="width\n";
}
void CodeGeneratorVisitorNode::visit( ASTPadHeight *pointer){
    //Adding the height instruction to the printList
    printList[currentStoredFunctionName]+="height\n";
}
void CodeGeneratorVisitorNode::visit( ASTSubExpr *pointer){
    //Accepting Expression
    pointer->expression->accept(this);
}
void CodeGeneratorVisitorNode::visit( ASTUnary *pointer){
    //Setting the respective flags, based on operator
    if(pointer->UnaryOperator=="not"){
        notFlag=!notFlag;
    }
    else{
        negativeFlag=!negativeFlag;
    }
    //Accepting expression
    pointer->expression->accept(this);
    //Resetting Flags
    notFlag=false;
    negativeFlag=false;
}
void CodeGeneratorVisitorNode::visit( ASTActualParams *pointer){
    //Looping through all the expressions
    auto iter = pointer->expressions.end();
    for(iter--; iter >= pointer->expressions.begin(); iter--)
    {
        ((*iter))->accept(this);
    }
    //Adding the push instruction to the printList
    printList[currentStoredFunctionName]+="push " + to_string(pointer->expressions.size());
}
void CodeGeneratorVisitorNode::visit( ASTPixelStatement *pointer){
    //Iterating through all the Pixel Expressions
    auto iter = pointer->expressions.end();
    for(iter--; iter >= pointer->expressions.begin(); iter--)
    {
        ((*iter))->accept(this);
    }
    //Adding relevant pixel instructions to the printList
    if(pointer->value=="<__pixelr>"){
        printList[currentStoredFunctionName]+="pixelr\n";
    }
    else{
        printList[currentStoredFunctionName]+="pixel\n";
    }
}
//Function which Prints the Program by iterating through the different Function Names and printing their respective print
void CodeGeneratorVisitorNode::PrintProgram() {
    for(auto & functionName : functionNames){
        cout << printList[functionName];
    }
}
//Destructor for class
CodeGeneratorVisitorNode::~CodeGeneratorVisitorNode(){
    functionNames.clear();
    symbolTable.reset();
}
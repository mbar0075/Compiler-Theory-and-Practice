//Inclusion of the relevant header File
#include "VisitorNodes.h"

void CodeGeneratorVisitorNode::visit( ASTNode *pointer){}
void CodeGeneratorVisitorNode::visit( ASTStatement *pointer){}
void CodeGeneratorVisitorNode::visit( ASTLiteral *pointer){}
void CodeGeneratorVisitorNode::visit( ASTType *pointer){}
void CodeGeneratorVisitorNode::visit( ASTFactor *pointer){}
void CodeGeneratorVisitorNode::visit( ASTExpr *pointer){}
void CodeGeneratorVisitorNode::visit( ASTSimpleExpr *pointer){}
void CodeGeneratorVisitorNode::visit( ASTTerm *pointer){}
void CodeGeneratorVisitorNode::visit( ASTFormalParam *pointer){
}
void CodeGeneratorVisitorNode::visit( ASTFormalParams *pointer){

}
void CodeGeneratorVisitorNode::visit( ASTProgram *pointer){
    //Creating a new Scope and pushing it onto the symbol Table
    Scope initialScope;
    symbolTable->push(initialScope);
    //Setting the currentStoreFunctionName to main
    currentStoredFunctionName="main";
    functionNames.emplace_back("main");
    //Updating the print key for the currentStoredFunctionName in the symbol table
    printList[currentStoredFunctionName]+=".main\npush 0\noframe\n";
    string currentFunctionName=currentStoredFunctionName;
    //Iterating through all the statements in the program, and resetting the currentStoredFunctionName, for every iteration
    for(auto iter = pointer->program.begin(); iter < pointer->program.end(); iter++)
    {
        currentStoredFunctionName=currentFunctionName;
        ((*iter))->accept(this);
    }
    currentStoredFunctionName=currentFunctionName;
    //Updating the print key for the currentStoredFunctionName in the symbol table
    printList[currentStoredFunctionName]+="cframe\nhalt\n";
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
    printList[currentStoredFunctionName]+="push 0\noframe\n";
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
    conditionalCounter++;
    pointer->expression->accept(this);
    string previousFunction=currentStoredFunctionName;
    string newKey="For "+to_string(conditionalCounter);
    currentStoredFunctionName=newKey;

    pointer->block->accept(this);
    if(pointer->assignment!= nullptr) {
        pointer->assignment->accept(this);
    }
    pointer->expression->accept(this);
    int noOfLines=CalculateJumpAddress(printList[currentStoredFunctionName]);

    printList[previousFunction]+="not\npush #PC+"+ to_string(noOfLines+3)+"\ncjmp2\n"+printList[currentStoredFunctionName];
    printList[previousFunction]+="push #PC-"+ to_string(noOfLines-1)+"\ncjmp2\n";

    //Erasing key
    auto it=printList.find(newKey);
    printList.erase (it);
    currentStoredFunctionName=previousFunction;
    conditionalCounter--;
}
void CodeGeneratorVisitorNode::visit( ASTWhileStatement *pointer){
    conditionalCounter++;
    pointer->expression->accept(this);
    string previousFunction=currentStoredFunctionName;
    string newKey="While "+to_string(conditionalCounter);
    currentStoredFunctionName=newKey;

    pointer->block->accept(this);
    pointer->expression->accept(this);
    int noOfLines=CalculateJumpAddress(printList[currentStoredFunctionName]);

    printList[previousFunction]+="not\npush #PC+"+ to_string(noOfLines+3)+"\ncjmp2\n"+printList[currentStoredFunctionName];
    printList[previousFunction]+="push #PC-"+ to_string(noOfLines-1)+"\ncjmp2\n";

    //Erasing key
    auto it=printList.find(newKey);
    printList.erase (it);
    currentStoredFunctionName=previousFunction;
    conditionalCounter--;
}
void CodeGeneratorVisitorNode::visit( ASTIfStatement *pointer){
    conditionalCounter++;
    pointer->expression->accept(this);
    string previousFunction=currentStoredFunctionName;
    string newKey="If "+to_string(conditionalCounter);
    currentStoredFunctionName=newKey;

    if(pointer->secondBlock!= nullptr){
        pointer->secondBlock->accept(this);
        int noOfLines=CalculateJumpAddress(printList[currentStoredFunctionName])+3;
        printList[previousFunction]+="push #PC+"+to_string(noOfLines)+"\ncjmp2\n"+printList[currentStoredFunctionName];
        printList[currentStoredFunctionName]="";
    }
    string currentInstruction;
    pointer->firstBlock->accept(this);
    int noOfLines=CalculateJumpAddress(printList[currentStoredFunctionName])+1;
    if(pointer->secondBlock!= nullptr){
        currentInstruction="push #PC+"+ to_string(noOfLines)+"\njmp\n";
    }
    else{
        currentInstruction="not\npush #PC+"+ to_string(noOfLines)+"\ncjmp2\n";
    }
    printList[previousFunction]+=currentInstruction+printList[currentStoredFunctionName];

    //Erasing key
    auto it=printList.find(newKey);
    printList.erase (it);
    currentStoredFunctionName=previousFunction;
    conditionalCounter--;
}
void CodeGeneratorVisitorNode::visit( ASTVariableDecl *pointer){
    //Accepting Identifier and expression
    printList[currentStoredFunctionName] += "push 1\nalloc\n";
    pointer->expression->accept(this);
    //Adding the respective instructions to the printList
    auto iter = symbolTable->scopeStack.end();iter--;

    string identifier=pointer->identifier->identifier;
    //Calculating and adding the respective identifier storage to the symbol table
    (*iter).scope[identifier]["Address"] ="[" + to_string((*iter).scope.size())  + ":" + to_string(0)+ "]";
    //Adding respective instruction to the printList

    printList[currentStoredFunctionName] += "push "+to_string((*iter).scope.size()-1)+"\n";
    printList[currentStoredFunctionName] += "push "+to_string(0)+"\n";
    printList[currentStoredFunctionName] += "st\n";
}
void CodeGeneratorVisitorNode::visit( ASTAssignment *pointer){
    string identifier=pointer->identifier->identifier;
    //Accepting expression
    pointer->expression->accept(this);
    //Retrieving the memory address from the symbol table
    string address=symbolTable->ReturnIdentifierAddress(identifier);
    string delimiter=":";
    //Adding the relevant instructions to the printList
    printList[currentStoredFunctionName] += "push "+address.substr(6, address.find(delimiter)-6)+"\n";
    printList[currentStoredFunctionName] += "push "+address.substr(address.find(delimiter)+1, (address.length()-(address.find(delimiter)+3)))+"\n";
    printList[currentStoredFunctionName] += "st\n";
}
void CodeGeneratorVisitorNode::visit( ASTFunctionDecl *pointer){
    frameIndex++;
    //Updating the currentStoredFunctionName, to the identifier
    string identifier=pointer->identifier->identifier;
    currentStoredFunctionName=identifier;
    //Adding the function Name instruction
    printList[currentStoredFunctionName]+="\n."+identifier+"\n";
    functionNames.emplace_back(identifier);



    Scope funScope;
    symbolTable->push(funScope);

    auto scopeIter = symbolTable->scopeStack.end();
    if(pointer->formalParams!= nullptr) {
        scopeIter--;
        for (auto iter = pointer->formalParams->formalParams.begin();
             iter < pointer->formalParams->formalParams.end(); iter++) {
            string paramIdentifier = ((*iter))->identifier->identifier;
            (*scopeIter).scope[paramIdentifier]["Address"] =
                    "[" + to_string((*scopeIter).scope.size()) + ":" + to_string(frameIndex) + "]";
        }
    }

    //Incrementing frameIndex and adding oframe instruction to the printList
    //Accepting block
    pointer->block->accept(this);

    //Adding the ret instruction to the printList
    printList[currentStoredFunctionName]+="ret\n";
    if(pointer->formalParams!= nullptr) {
        scopeIter = symbolTable->scopeStack.end();
        scopeIter--;
        for (auto iter = pointer->formalParams->formalParams.begin();
             iter < pointer->formalParams->formalParams.end(); iter++) {
            string paramIdentifier = ((*iter))->identifier->identifier;
            auto it = (*scopeIter).scope.find(paramIdentifier);
            (*scopeIter).scope.erase(it);
        }
    }
    symbolTable->pop();
    //Popping initialScope from the symbol Table
    frameIndex--;
}
void CodeGeneratorVisitorNode::visit( ASTFunctionCall *pointer){
    Scope funScope;
    symbolTable->push(funScope);
    printList[currentStoredFunctionName] +="oframe\n";
    //Checking whether actualParams is not a nullptr
    if(pointer->actualParams!= nullptr) {
        pointer->actualParams->accept(this);
    }
    else{
        printList[currentStoredFunctionName] +="push 0\nalloc\n";
    }
    //Adding the push functionName instruction and call instruction to the printList
    printList[currentStoredFunctionName]+= "push ." +pointer->identifier->identifier + "\ncall\n";
    printList[currentStoredFunctionName] +="cframe\n";
    symbolTable->pop();
}
void CodeGeneratorVisitorNode::visit( ASTIdentifier *pointer){
    string identifier=pointer->identifier;
    //Adding respective instruction to the printList
    printList[currentStoredFunctionName] +=  symbolTable->ReturnIdentifierAddress(identifier);
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
        printList[currentStoredFunctionName]+="or\n";
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
        printList[currentStoredFunctionName]+="and\n";
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
    else if(pointer->relationalOp=="=="){
        printList[currentStoredFunctionName]+="eq\n";
    }
    else if(pointer->relationalOp==">="){
        printList[currentStoredFunctionName]+="ge\n";
    }
    else if(pointer->relationalOp=="<="){
        printList[currentStoredFunctionName]+="le\n";
    }
    else if(pointer->relationalOp=="!="){
        printList[currentStoredFunctionName]+="neq\n";
    }
}
void CodeGeneratorVisitorNode::visit( ASTRtrnStatement *pointer){
    //Accepting expression
    pointer->expression->accept(this);
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
    printList[currentStoredFunctionName] +="push "+ to_string(pointer->expressions.size())+"\nalloc\n";
    int scopeIndex=pointer->expressions.size()-1;
    //Looping through all the expressions
    auto iter = pointer->expressions.end();
    for(iter--; iter >= pointer->expressions.begin(); iter--)
    {
        ((*iter))->accept(this);
        printList[currentStoredFunctionName] +="push "+ to_string(scopeIndex)+"\npush 0\nst\n";
        scopeIndex--;
    }
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

int CodeGeneratorVisitorNode::CalculateJumpAddress(const string& print) {
    int noOfLines=0;
    for(auto &character : print){
        if(character=='\n'){
            noOfLines++;
        }
    }
    return noOfLines+1;
}
//Destructor for class
CodeGeneratorVisitorNode::~CodeGeneratorVisitorNode(){
    functionNames.clear();
    symbolTable.reset();
}

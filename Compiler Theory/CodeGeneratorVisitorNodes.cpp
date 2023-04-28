//Inclusion of the relevant header File
#include "VisitorNodes.h"

//visit methods for the relevant AST Nodes passed as parameters
void CodeGeneratorVisitorNode::visit( ASTNode *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void CodeGeneratorVisitorNode::visit( ASTStatement *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void CodeGeneratorVisitorNode::visit( ASTLiteral *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void CodeGeneratorVisitorNode::visit( ASTType *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void CodeGeneratorVisitorNode::visit( ASTFactor *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void CodeGeneratorVisitorNode::visit( ASTExpr *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void CodeGeneratorVisitorNode::visit( ASTSimpleExpr *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void CodeGeneratorVisitorNode::visit( ASTTerm *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void CodeGeneratorVisitorNode::visit( ASTFormalParam *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
void CodeGeneratorVisitorNode::visit( ASTFormalParams *pointer){/*Empty Method not being used, however kept since it is virtual method in VisitorNode class*/}
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
    //Incrementing frameIndex and adding relevant instructions to the printList
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
    //Creating a new Scope and pushing it onto the symbol Table
    Scope blockScope;
    symbolTable->push(blockScope);
    //Incrementing frameIndex and adding relevant instructions to the printList
    frameIndex++;
    printList[currentStoredFunctionName]+="push 0\noframe\n";

    //Checking whether the variable declaration node is not a nullptr, and if so will proceed to accept the variableDecl Node
    if(pointer->variableDecl!= nullptr){
        pointer->variableDecl->accept(this);
    }
    //Incrementing the Conditional Counter
    conditionalCounter++;
    //Accepting Expression
    pointer->expression->accept(this);
    //Storing the currentStoredFunctionName in a variable, and calculating the new Function key
    string previousFunction=currentStoredFunctionName;
    string newKey="For "+to_string(conditionalCounter);
    //Changing the currentStoredFunctionName to newKey, to make the printList to act as a temporary buffer for storing the output
    currentStoredFunctionName=newKey;
    //Accepting block Node
    //Iterating through all the statements in the block, and resetting the currentStoredFunctionName, for every iteration
    for(auto iter = pointer->block->statements.begin(); iter < pointer->block->statements.end(); iter++)
    {
        ((*iter))->accept(this);
    }
    //Checking whether the assignment node is not a nullptr, and if so will proceed to accept the assignment Node
    if(pointer->assignment!= nullptr) {
        pointer->assignment->accept(this);
    }
    //Accepting Expression
    pointer->expression->accept(this);
    //Calling CalculateJumpAddress function to obtain the number of address lines are in the current Conditional Statement Block
    int noOfLines=CalculateJumpAddress(printList[currentStoredFunctionName]);
    //Adding the respective instructions to the printList for the previousFunction
    printList[previousFunction]+="not\npush #PC+"+ to_string(noOfLines+3)+"\ncjmp2\n"+printList[currentStoredFunctionName];
    printList[previousFunction]+="push #PC-"+ to_string(noOfLines-1)+"\ncjmp2\n";
    //Erasing the newKey from printList, as it is no longer needed
    auto it=printList.find(newKey);
    printList.erase (it);
    //Resetting variables
    currentStoredFunctionName=previousFunction;
    conditionalCounter--;
    frameIndex--;
    //Updating the print key for the currentStoredFunctionName in the symbol table
    printList[currentStoredFunctionName]+="cframe\n";
    //Popping initialScope from the symbol Table
    symbolTable->pop();

}
void CodeGeneratorVisitorNode::visit( ASTWhileStatement *pointer){
    //Incrementing the Conditional Counter
    conditionalCounter++;
    //Accepting Expression
    pointer->expression->accept(this);
    //Storing the currentStoredFunctionName in a variable, and calculating the new Function key
    string previousFunction=currentStoredFunctionName;
    string newKey="While "+to_string(conditionalCounter);
    //Changing the currentStoredFunctionName to newKey, to make the printList to act as a temporary buffer for storing the output
    currentStoredFunctionName=newKey;
    //Accepting block and expression
    pointer->block->accept(this);
    pointer->expression->accept(this);
    //Calling CalculateJumpAddress function to obtain the number of address lines are in the current Conditional Statement Block
    int noOfLines=CalculateJumpAddress(printList[currentStoredFunctionName]);
    //Adding the respective instructions to the printList for the previousFunction
    printList[previousFunction]+="not\npush #PC+"+ to_string(noOfLines+3)+"\ncjmp2\n"+printList[currentStoredFunctionName];
    printList[previousFunction]+="push #PC-"+ to_string(noOfLines-1)+"\ncjmp2\n";
    //Erasing the newKey from printList, as it is no longer needed
    auto it=printList.find(newKey);
    printList.erase (it);
    //Resetting variables
    currentStoredFunctionName=previousFunction;
    conditionalCounter--;
}
void CodeGeneratorVisitorNode::visit( ASTIfStatement *pointer){
    //Incrementing the Conditional Counter
    conditionalCounter++;
    //Accepting Expression
    pointer->expression->accept(this);
    //Storing the currentStoredFunctionName in a variable, and calculating the new Function key
    string previousFunction=currentStoredFunctionName;
    string newKey="If "+to_string(conditionalCounter);
    //Changing the currentStoredFunctionName to newKey, to make the printList to act as a temporary buffer for storing the output
    currentStoredFunctionName=newKey;
    //Checking secondBlock is not nullptr
    if(pointer->secondBlock!= nullptr){
        //Accepting secondBlock
        pointer->secondBlock->accept(this);
        //Calling CalculateJumpAddress function to obtain the number of address lines are in the current Conditional Statement Block
        int noOfLines=CalculateJumpAddress(printList[currentStoredFunctionName])+3;
        //Adding the respective instructions to the printList for the previousFunction, and resetting current buffer
        printList[previousFunction]+="push #PC+"+to_string(noOfLines)+"\ncjmp2\n"+printList[currentStoredFunctionName];
        printList[currentStoredFunctionName]="";
    }
    string currentInstruction;
    //Accepting the firstBlock
    pointer->firstBlock->accept(this);
    //Calling CalculateJumpAddress function to obtain the number of address lines are in the current Conditional Statement Block
    int noOfLines=CalculateJumpAddress(printList[currentStoredFunctionName])+1;
    //Assigning the respective instructions to the currentInstruction, based on whether secondBlock node is a nullptr or not
    if(pointer->secondBlock!= nullptr){
        currentInstruction="push #PC+"+ to_string(noOfLines)+"\njmp\n";
    }
    else{
        currentInstruction="not\npush #PC+"+ to_string(noOfLines)+"\ncjmp2\n";
    }
    //Adding the respective instructions to the printList for the previousFunction
    printList[previousFunction]+=currentInstruction+printList[currentStoredFunctionName];
    //Erasing the newKey from printList, as it is no longer needed
    auto it=printList.find(newKey);
    printList.erase (it);
    //Resetting variables
    currentStoredFunctionName=previousFunction;
    conditionalCounter--;
}
void CodeGeneratorVisitorNode::visit( ASTVariableDecl *pointer){
    //Adding the respective instructions to the printList
    printList[currentStoredFunctionName] += "push 1\nalloc\n";
    //Accepting expression
    pointer->expression->accept(this);
    //Retrieving currentScope in symbol table
    auto iter = symbolTable->scopeStack.end();iter--;
    //Retrieving string Identifier
    string identifier=pointer->identifier->identifier;
    //Calculating and adding the respective identifier storage to the symbol table
    (*iter).scope[identifier]["Address"] ="[" + to_string((*iter).scope.size())  + ":" + to_string(0)+ "]";
    //Adding the relevant instructions to the printList
    printList[currentStoredFunctionName] += "push "+to_string((*iter).scope.size()-1)+"\n";
    printList[currentStoredFunctionName] += "push "+to_string(0)+"\n";
    printList[currentStoredFunctionName] += "st\n";
}
void CodeGeneratorVisitorNode::visit( ASTAssignment *pointer){
    //Retrieving string Identifier
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
    //Incrementing frameIndex
    frameIndex++;
    //Updating the currentStoredFunctionName, to the identifier
    string identifier=pointer->identifier->identifier;
    currentStoredFunctionName=identifier;
    //Adding the function Name instruction
    printList[currentStoredFunctionName]+="\n."+identifier+"\n";
    functionNames.emplace_back(identifier);
    //Creating two new scopes and adding them to the symbol table, one to denote scope of actual params, and the other to denote scope of function
    Scope funScope1;
    symbolTable->push(funScope1);
    Scope funScope2;
    symbolTable->push(funScope2);
    //Retrieving the current scope from the symbol table
    auto scopeIter = symbolTable->scopeStack.end();
    //Checking whether the function has parameters
    if(pointer->formalParams!= nullptr) {
        scopeIter--;
        //Iterating through all the parameters and assigning a memory address for that parameter to the symbol table
        for (auto iter = pointer->formalParams->formalParams.begin();
             iter < pointer->formalParams->formalParams.end(); iter++) {
            string paramIdentifier = ((*iter))->identifier->identifier;
            (*scopeIter).scope[paramIdentifier]["Address"] =
                    "[" + to_string((*scopeIter).scope.size()) + ":" + to_string(frameIndex) + "]";
        }
    }
    //Accepting block
    pointer->block->accept(this);
    //Adding the ret instruction to the printList
    printList[currentStoredFunctionName]+="ret\n";
    //Checking whether the function has parameters
    if(pointer->formalParams!= nullptr) {
        scopeIter = symbolTable->scopeStack.end();
        scopeIter--;
        //Iterating through all the parameters and removing parameters from the symbo table
        for (auto iter = pointer->formalParams->formalParams.begin();
             iter < pointer->formalParams->formalParams.end(); iter++) {
            string paramIdentifier = ((*iter))->identifier->identifier;
            auto it = (*scopeIter).scope.find(paramIdentifier);
            (*scopeIter).scope.erase(it);
        }
    }
    //Popping scopes and decrementing frameIndex
    symbolTable->pop();
    symbolTable->pop();
    frameIndex--;
}
void CodeGeneratorVisitorNode::visit( ASTFunctionCall *pointer){
    //Creating new scope to hold the actual Parameters
    Scope funScope;
    symbolTable->push(funScope);
    //Checking whether actualParams is not a nullptr
    if(pointer->actualParams!= nullptr) {
        //Code Optimization: Adding the number of parameters in the beginning of the frame, to avoid multiple push 1 alloc's
        //Adding the respective instructions to the printList
        printList[currentStoredFunctionName] +="push "+ to_string(pointer->actualParams->expressions.size())+"\n";
        printList[currentStoredFunctionName] +="oframe\n";
        pointer->actualParams->accept(this);
    }
    else{
        printList[currentStoredFunctionName] +="push 0\noframe\n";
    }
    //Adding the respective functions to the printList
    printList[currentStoredFunctionName]+= "push ." +pointer->identifier->identifier + "\ncall\n";
    printList[currentStoredFunctionName] +="cframe\n";
    //Popping scope from symbol table
    symbolTable->pop();
}
void CodeGeneratorVisitorNode::visit( ASTIdentifier *pointer){
    //Retrieving string Identifier
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
    //Accepting expression, and adding the respective instruction to the printList
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
    //Code Optimization: To avoid multiple not instructions in code
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
    //Retrieving the current scopeIndex
    size_t scopeIndex=pointer->expressions.size()-1;
    //Looping through all the expressions
    auto iter = pointer->expressions.end();
    for(iter--; iter >= pointer->expressions.begin(); iter--)
    {
        //Accepting expression, and adding the relevant instructions to the printList, whilst decrementing the scopeIndex
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
    //Looping through the list of FunctionNames in order of first stored
    for(auto & functionName : functionNames){
        //Outputting the respective print for the current function Name
        cout << printList[functionName];
    }
}
//Function which Calculates the Jump Address, given a string print buffer
int CodeGeneratorVisitorNode::CalculateJumpAddress(const string& print) {
    //Declaring the number of lines
    int noOfLines=0;
    //Looping through all the characters in the print buffer
    for(auto &character : print){
        //Checking whether the current character is a '\n', i.e, indicating a new line, thus incrementing the number of lines
        if(character=='\n'){
            noOfLines++;
        }
    }
    //Returning the number of lines, and adding +1, to cater for the first line
    return noOfLines+1;
}
//Destructor for class
CodeGeneratorVisitorNode::~CodeGeneratorVisitorNode(){
    functionNames.clear();
    symbolTable.reset();
}

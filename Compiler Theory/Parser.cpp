#include "HeaderFile.h"


void Parser::XMLPass(fstream &readFilePointer) {
    this->program =ParseProgram(readFilePointer);
    shared_ptr<XMLVisitorNode>node= make_shared<XMLVisitorNode>();

    //node->visit(program);
    for(auto iter = program->program.begin(); iter < program->program.end(); iter++)
    {
        cout<<"Looping";
        (*iter)->accept(node);
    }
    node.reset();
}

shared_ptr<ASTProgram> Parser::ParseProgram(fstream &readFilePointer) {
    file = move(readFilePointer);
    vector<shared_ptr<ASTStatement>> allStatements;
    GetNextToken();
    GetNextToken();
    while (!EOFFlag||(lookaheadToken1->GetTokenName()=="<{>"&&lookaheadToken2->GetTokenName()=="<}>")) {
//        cout<<"\nLooping"<<endl;
        allStatements.push_back(ParseStatement());
        if(EOFFlag){
            cout<<"End of File Reached"<<endl;
            break;
        }
    }
    return shared_ptr<ASTProgram>(make_shared<ASTProgram>(allStatements));
}

shared_ptr<ASTStatement> Parser::ParseStatement() {
//    cout<<"\nStatement: "<<lookaheadToken1->GetTokenName()<<" EOF"<<EOFFlag;
    shared_ptr<ASTStatement> singleStatement;
    bool checkSemiColon=false;

    if(lookaheadToken1->GetTokenName()=="<Identifier>"){
        singleStatement = ParseAssignment();
        checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<{>"){
        singleStatement = ParseBlock();
    }
    else if(lookaheadToken1->GetTokenName()=="<let>"){
        singleStatement = ParseVariableDecl();
        checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<__print>"){
        singleStatement = ParsePrintStatement();
        checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<return>"){
        singleStatement = ParseReturnStatement();
        checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<while>"){
        singleStatement = ParseWhileStatement();
    }
    else if(lookaheadToken1->GetTokenName()=="<if>"){
        singleStatement = ParseIfStatement();
    }
    else if(lookaheadToken1->GetTokenName()=="<for>"){
        singleStatement = ParseForStatement();
    }
    else if(lookaheadToken1->GetTokenName()=="<__pixelr>" ||lookaheadToken1->GetTokenName()=="<__pixel>"){
        singleStatement = ParsePixelStatement();
        checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<fun>"){
        singleStatement = ParseFunctionDecl();
    }
    else if(lookaheadToken1->GetTokenName()=="<__delay>"){
        singleStatement = ParseDelayStatement();
        checkSemiColon= true;
    }
    else{
        cerr<<"\nError: Unexpected: "<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }

    if(checkSemiColon){
//        cout<<"CheckSemi Colon"<<endl;
        if(lookaheadToken1->GetTokenName()!="<;>"){
            cerr<<"\nError: "<<lookaheadToken1->GetTokenAttribute()<<" ; expected "<<endl;
            exit(4);
        }
        GetNextToken();
    }
    return singleStatement;
}

shared_ptr<ASTBlock> Parser::ParseBlock() {
//    cout<<"\nBlock start"<<endl;
    vector<shared_ptr<ASTStatement>> statements;
    shared_ptr<ASTStatement> singleStatement;
    if(lookaheadToken1->GetTokenName()!="<{>"){
        cerr<<"\nError: Expected {, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    while (lookaheadToken1->GetTokenName()!="<}>") {
        singleStatement=ParseStatement();
        statements.push_back(singleStatement);
    }
    //Check for closing brackets
    if(lookaheadToken1->GetTokenName()!="<}>"){
        cerr<<"\nError: Expected }, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();

//    cout<<"\nBlock end"<<endl;
    return make_shared<ASTBlock>(statements);
}

shared_ptr<ASTIfStatement> Parser::ParseIfStatement() {
    shared_ptr<ASTExpr> expression;
    shared_ptr<ASTBlock> firstBlock;
    shared_ptr<ASTBlock> secondBlock=nullptr;
    if(lookaheadToken1->GetTokenName()=="<if>"&&lookaheadToken2->GetTokenName()=="<(>"){
        GetNextToken();
        GetNextToken();
        expression=ParseExpression();
        if(lookaheadToken1->GetTokenName()!="<)>"){
            cerr<<"\nError: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        firstBlock=ParseBlock();

        if(lookaheadToken1->GetTokenName()=="<else>"){
            GetNextToken();
            secondBlock=ParseBlock();
        }
    }else{
        cerr<<"\nError: Expected if, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTIfStatement>(expression,firstBlock,secondBlock);
}

shared_ptr<ASTWhileStatement> Parser::ParseWhileStatement() {
    shared_ptr<ASTExpr> expression;
    shared_ptr<ASTBlock> block;
    if(lookaheadToken1->GetTokenName()=="<while>"&&lookaheadToken2->GetTokenName()=="<(>"){
        GetNextToken();
        GetNextToken();
        expression=ParseExpression();
        if(lookaheadToken1->GetTokenName()!="<)>"){
            cerr<<"\nError: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        block=ParseBlock();
    }else{
        cerr<<"\nError: Expected while, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTWhileStatement>(expression,block);
}

shared_ptr<ASTFormalParam> Parser::ParseFormalParam() {
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTType> type;

    identifier=ParseIdentifier();
    GetNextToken();
    if(lookaheadToken1->GetTokenName()!="<:>"){
        cerr<<"\nError: Expected :, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    type=ParseType();
    GetNextToken();
    return make_shared<ASTFormalParam>(identifier,type);
}

shared_ptr<ASTFormalParams> Parser::ParseFormalParams() {
    vector<shared_ptr<ASTFormalParam>> formalParams;
    formalParams.push_back(ParseFormalParam());

    while(lookaheadToken1->GetTokenName()=="<Comma>" ){
        GetNextToken();
        formalParams.push_back(ParseFormalParam());
    }

    return make_shared<ASTFormalParams>(formalParams);
}

shared_ptr<ASTFunctionDecl> Parser::ParseFunctionDecl() {
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTFormalParams> formalParams=nullptr;
    shared_ptr<ASTType> type;
    shared_ptr<ASTBlock> block;
    if(lookaheadToken1->GetTokenName()=="<fun>"){
        GetNextToken();
        identifier=ParseIdentifier();
        GetNextToken();
        if(lookaheadToken1->GetTokenName()=="<(>"){
            GetNextToken();
            if(lookaheadToken1->GetTokenName()!="<)>"){
                formalParams=ParseFormalParams();
            }
            if(lookaheadToken1->GetTokenName()!="<)>"){
                cerr<<"\nError: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
                exit(4);
            }
            GetNextToken();
            if(lookaheadToken1->GetTokenName()!="<->>"){
                cerr<<"\nError: Expected ->, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
                exit(4);
            }
            GetNextToken();
            type=ParseType();
            GetNextToken();
            block=ParseBlock();
        }
    }else{
        cerr<<"\nError: Expected fun, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTFunctionDecl>(identifier, formalParams, type, block);
}

shared_ptr<ASTForStatement> Parser::ParseForStatement() {
    shared_ptr<ASTExpr> expression;
    shared_ptr<ASTAssignment> assignment=nullptr;
    shared_ptr<ASTVariableDecl> variableDecl=nullptr;
    shared_ptr<ASTBlock> block;
    if(lookaheadToken1->GetTokenName()=="<for>"){
        GetNextToken();
        if(lookaheadToken1->GetTokenName()!="<(>") {
            cerr<<"\nError: Expected (, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        if(lookaheadToken1->GetTokenName()!="<;>") {
            variableDecl = ParseVariableDecl();
        }
        if(lookaheadToken1->GetTokenName()!="<;>") {
            cerr<<"\nError: Expected ;, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        expression=ParseExpression();
        if(lookaheadToken1->GetTokenName()!="<;>") {
            cerr<<"\nError: Expected ;, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        if(lookaheadToken1->GetTokenName()!="<)>") {
            assignment=ParseAssignment();
        }
        if(lookaheadToken1->GetTokenName()!="<)>") {
            cerr<<"\nError: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        block=ParseBlock();
    }else{
        cerr<<"\nError: Expected for, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTForStatement>(expression,assignment,variableDecl,block);
}

shared_ptr<ASTPixelStatement> Parser::ParsePixelStatement() {
    vector<shared_ptr<ASTExpr>> expressions;
    string value;
    if(lookaheadToken1->GetTokenName()=="<__pixelr>"){
        GetNextToken();
        value=lookaheadToken1->GetTokenName();
        for(int c =0; c<5;c++){
            expressions.push_back(ParseExpression());
            if(c<4 && lookaheadToken1->GetTokenName()!="<Comma>"){
                cerr<<"\nError: Expected ',', received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
                exit(4);
            }
            else if(c<4){
                GetNextToken();
            }
        }
    }
    else if(lookaheadToken1->GetTokenName()=="<__pixel>"){
        GetNextToken();
        value=lookaheadToken1->GetTokenName();
        for(int c =0; c<3;c++){
            expressions.push_back(ParseExpression());
            if(c<2 && lookaheadToken1->GetTokenName()!="<Comma>"){
                cerr<<"\nError: Expected ',', received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
                exit(4);
            }
            else if(c<2){
                GetNextToken();
            }
        }
    }
    else{
        cerr<<"\nError: Expected __pixelr or __pixel, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTPixelStatement>(value,expressions);
}

shared_ptr<ASTPrintStatement> Parser::ParsePrintStatement() {
//    cout<<"\nPrint Statement start"<<endl;

    shared_ptr<ASTExpr> expression;
    if(lookaheadToken1->GetTokenName()!="<__print>"){
        cerr<<"\nError: Expected __print, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    expression=ParseExpression();
//    cout<<"\nPrint Statement end"<<endl;
    return make_shared<ASTPrintStatement>(expression);
}

shared_ptr<ASTDelayStatement> Parser::ParseDelayStatement() {
//    cout<<"\nDelay Statement start"<<endl;

    shared_ptr<ASTExpr> expression;
    if(lookaheadToken1->GetTokenName()!="<__delay>"){
        cerr<<"\nError: Expected __delay, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    expression=ParseExpression();
//    cout<<"\nDelay Statement end"<<endl;
    return make_shared<ASTDelayStatement>(expression);
}

shared_ptr<ASTRtrnStatement> Parser::ParseReturnStatement() {
//    cout<<"\nReturn Statement start"<<endl;

    shared_ptr<ASTExpr> expression;
    if(lookaheadToken1->GetTokenName()!="<return>"){
        cerr<<"\nError: Expected return, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    expression=ParseExpression();
//    cout<<"\nReturn Statement end"<<endl;
    return make_shared<ASTRtrnStatement>(expression);
}

shared_ptr<ASTVariableDecl> Parser::ParseVariableDecl() {
//    cout<<"\nVariable Decl start"<<endl;
    GetNextToken();
    shared_ptr<ASTIdentifier> identifier = ParseIdentifier();
    GetNextToken();
    if(lookaheadToken1->GetTokenName()!="<:>"){
        cerr<<"\nError: Expected :, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();

    shared_ptr<ASTType> type;
    if(lookaheadToken1->GetTokenName()!="<Type>"){
        cerr<<"\nError: Expected Type, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    type=ParseType();
    GetNextToken();

    if(lookaheadToken1->GetTokenName()!="<=>"){
        cerr<<"\nError: Expected =, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();

    shared_ptr<ASTExpr> expression=ParseExpression();
//    cout<<"\nVariable Decl end"<<endl;
    return make_shared<ASTVariableDecl>(identifier,type,expression);
}

shared_ptr<ASTAssignment> Parser::ParseAssignment() {
//    cout<<"\nAssignment"<<endl;
    shared_ptr<ASTIdentifier> identifier = ParseIdentifier();
    GetNextToken();
    if(lookaheadToken1->GetTokenName()!="<=>"){
        cerr<<"\nError: Expected =, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    shared_ptr<ASTExpr> expression = ParseExpression();
//    cout<<"\nAssignment end"<<endl;
    return make_shared<ASTAssignment>(identifier,expression);
}

shared_ptr<ASTIdentifier> Parser::ParseIdentifier(){
//    cout<<"\nIdentifier start"<<endl;
    if(lookaheadToken1->GetTokenName()!="<Identifier>"){
        cerr<<"\nError: Expected Identifier, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
//    cout<<"\nIdentifier end"<<endl;
    return make_shared<ASTIdentifier>(lookaheadToken1->GetTokenAttribute());
}

shared_ptr<ASTType> Parser::ParseType() {
//    cout<<"\nType"<<endl;
    return make_shared<ASTType>(lookaheadToken1->GetTokenAttribute());
}

shared_ptr<ASTExpr> Parser::ParseExpression() {
//    cout<<"\nExpression start"<<endl;
    shared_ptr<ASTExpr> expr;
    shared_ptr<ASTExpr> leftExpr;

    leftExpr=ParseSimpleExpression();

    if(lookaheadToken1->GetTokenName()=="<RelationalOp>" ) {
        GetNextToken();
        string relationalOp=lookaheadToken1->GetTokenAttribute();
        shared_ptr<ASTExpr> rightExpr;
        rightExpr=ParseExpression();
        expr=shared_ptr<ASTRelationalOp>(make_shared<ASTRelationalOp>(leftExpr, relationalOp, rightExpr));
    }
    else{
        expr = move(leftExpr);
    }
//    cout<<"\nExpression end"<<endl;
    return expr;
}

shared_ptr<ASTExpr> Parser::ParseSimpleExpression() {
//    cout<<"\nSimple expression start"<<endl;
    shared_ptr<ASTExpr> simpleExpr;
    shared_ptr<ASTExpr> leftSimpleExpr;
    leftSimpleExpr=ParseTerm();

    if(lookaheadToken1->GetTokenName()=="<AdditiveOp>" ) {
        GetNextToken();
        string additiveOp=lookaheadToken1->GetTokenAttribute();
        shared_ptr<ASTExpr> rightSimpleExpr;
        rightSimpleExpr=ParseSimpleExpression();
        simpleExpr=shared_ptr<ASTAdditiveOp>(make_shared<ASTAdditiveOp>(leftSimpleExpr, additiveOp, rightSimpleExpr));
    }
    else{
        simpleExpr = move(leftSimpleExpr);
    }
//    cout<<"\nSimple expression end"<<endl;
    return simpleExpr;
}

shared_ptr<ASTExpr> Parser::ParseTerm() {
//    cout<<"\nTerm start"<<endl;
    shared_ptr<ASTExpr> term;
    shared_ptr<ASTExpr> leftTerm;
    leftTerm=ParseFactor();

    if(lookaheadToken1->GetTokenName()=="<MultiplicativeOp>" ) {
        GetNextToken();
        string multiplicativeOp=lookaheadToken1->GetTokenAttribute();
        shared_ptr<ASTExpr> rightTerm;
        rightTerm=ParseTerm();
        term=shared_ptr<ASTMultiplicativeOp>(make_shared<ASTMultiplicativeOp>(leftTerm, multiplicativeOp, rightTerm));
    }
    else{
        term = move(leftTerm);
    }
//    cout<<"\nTerm end"<<endl;
    return term;
}

shared_ptr<ASTLiteral> Parser::ParseLiteral() {
    auto literal= make_shared<ASTLiteral>();
    if(lookaheadToken1->GetTokenName() == "<IntegerLiteral>"){
//        cout<<"\nInteger Literal "<<lookaheadToken1->GetTokenAttribute();
        literal=make_shared<ASTIntLiteral>(lookaheadToken1->GetTokenAttribute());
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<BooleanLiteral>"){
//        cout<<"\nBoolean Literal "<<lookaheadToken1->GetTokenAttribute();
        literal=make_shared<ASTBoolLiteral>(lookaheadToken1->GetTokenAttribute());
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<FloatLiteral>"){
//        cout<<"\nFloat Literal "<<lookaheadToken1->GetTokenAttribute();
        literal=make_shared<ASTFloatLiteral>(lookaheadToken1->GetTokenAttribute());
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<ColourLiteral>"){
//        cout<<"\nColour Literal "<<lookaheadToken1->GetTokenAttribute();
        literal=make_shared<ASTColourLiteral>(lookaheadToken1->GetTokenAttribute());
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<PadWidth>"){
        literal=make_shared<ASTPadWidth>(lookaheadToken1->GetTokenAttribute());
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<PadHeight>"){
        literal=make_shared<ASTPadHeight>(lookaheadToken1->GetTokenAttribute());
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<__read>"){
        //Need expression
        literal=ParsePadRead();
    }
    else{
        cerr<<"\nError: Expected Literal, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return literal;
}

shared_ptr<ASTPadRead> Parser::ParsePadRead() {
    shared_ptr<ASTExpr> firstExpression;
    shared_ptr<ASTExpr> secondExpression;
    if(lookaheadToken1->GetTokenName()!="<__read>"){
        cerr<<"\nError: Expected __read, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    firstExpression = ParseExpression();

    if(lookaheadToken1->GetTokenName()!="<Comma>"){
        cerr<<"\nError: Expected ',', received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    secondExpression = ParseExpression();

    return make_shared<ASTPadRead>(firstExpression,secondExpression);
}

shared_ptr<ASTExpr> Parser::ParseFactor() {
//    cout<<"\nFactor start"<<endl;

    auto factor= make_shared<ASTExpr>();
    if(lookaheadToken1->GetTokenName() == "<__read>"||lookaheadToken1->GetTokenName() == "<PadHeight>"||lookaheadToken1->GetTokenName() == "<PadWidth>"||lookaheadToken1->GetTokenName() == "<ColourLiteral>"||lookaheadToken1->GetTokenName() == "<IntegerLiteral>" ||lookaheadToken1->GetTokenName() == "<BooleanLiteral>" ||lookaheadToken1->GetTokenName() == "<FloatLiteral>"){
        factor=ParseLiteral();
    }
    else if(lookaheadToken1->GetTokenName() == "<Identifier>" && lookaheadToken2->GetTokenName() == "<(>"){
        factor=ParseFunctionCall();
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<Identifier>"){
        factor=ParseIdentifier();
        GetNextToken();
    }
        //SubExpr
    else if(lookaheadToken1->GetTokenName() == "<(>"){
        factor=ParseSubExpression();
        GetNextToken();
    }
        //Unary - this need to arrange
    else if((lookaheadToken1->GetTokenName() == "<AdditiveOp>" && lookaheadToken1->GetTokenAttribute()=="-")|| lookaheadToken1->GetTokenName() == "<not>"){
        factor=ParseUnary();
    }
        //PadRandI
    else if(lookaheadToken1->GetTokenName() == "<__randi>"){
        factor=ParsePadRandi();
    }
    else{
        cerr<<"\nError: Expected Factor, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }

//    cout<<"\nFactor end"<<endl;
    return factor;
}

shared_ptr<ASTFunctionCall> Parser::ParseFunctionCall() {
//    cout<<"\nFunction Call start"<<endl;
    shared_ptr<ASTIdentifier> identifier=ParseIdentifier();
    GetNextToken();
    if(lookaheadToken1->GetTokenName()!="<(>"){
        cerr<<"\nError: Expected (, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    shared_ptr<ASTActualParams> actualParams;

    //Check if function has parameters
    if(lookaheadToken1->GetTokenName()!="<)>"){
        actualParams = ParseActualParams();
    }
    //Check for closing brackets
    if(lookaheadToken1->GetTokenName()!="<)>"){
        cerr<<"\nError: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }

//    cout<<"\nFunction Call end"<<endl;
    return make_shared<ASTFunctionCall>(identifier,actualParams);
}

shared_ptr<ASTActualParams> Parser::ParseActualParams() {
//    cout<<"\nActual Params start"<<endl;
    vector<shared_ptr<ASTExpr>> expressions;
    expressions.push_back(ParseExpression());

    while (lookaheadToken1->GetTokenName()=="<Comma>" ) {
        GetNextToken();
        expressions.push_back(ParseExpression());
    }
//    cout<<"\nActual Params end"<<endl;
    return make_shared<ASTActualParams>(expressions);
}

shared_ptr<ASTExpr> Parser::ParseUnary() {
    string unaryOperator;
    if((lookaheadToken1->GetTokenName()=="<AdditiveOp>"&& lookaheadToken1->GetTokenAttribute()=="-")||lookaheadToken1->GetTokenName()=="<not>"){
        unaryOperator=lookaheadToken1->GetTokenAttribute();
        GetNextToken();
    }
    else{
        cerr<<"\nError: Expected - or not, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    shared_ptr<ASTExpr> expression=ParseExpression();
    return make_shared<ASTUnary>(expression,unaryOperator);
}

shared_ptr<ASTExpr> Parser::ParsePadRandi() {
    shared_ptr<ASTExpr> expression;
    if(lookaheadToken1->GetTokenName()!="<__randi>"){
        cerr<<"\nError: Expected __randi, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }

    GetNextToken();
    expression=ParseExpression();
    return make_shared<ASTPadRandi>(expression);
}

shared_ptr<ASTExpr> Parser::ParseSubExpression() {
    shared_ptr<ASTExpr> expression;
    if(lookaheadToken1->GetTokenName()!="<(>"){
        cerr<<"\nError: Expected (, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }

    GetNextToken();
    if(lookaheadToken1->GetTokenName()!="<)>"){
        expression = ParseExpression();
    }
    if(lookaheadToken1->GetTokenName()!="<)>"){
        cerr<<"\nError: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }

    return make_shared<ASTSubExpr>(expression);
}


void Parser::GetNextToken() {
    lookaheadToken1=lookaheadToken2;
    if(!file.eof()){
        lookaheadToken2 = lexer->GetNextToken(file);
        CheckValidToken(lookaheadToken2);
        while((lookaheadToken2->GetTokenName()=="<TokenDelimEnter>"||lookaheadToken2->GetTokenName()=="<TokenDelim>"||lookaheadToken2->GetTokenName()=="<Comment>")) {
            lookaheadToken2 = lexer->GetNextToken(file);
            CheckValidToken(lookaheadToken2);
        }
    }
    else{
//        cout<<"End of file"<<endl;
        EOFFlag=true;
    }
//    if(lookaheadToken1!= nullptr){
//        cout<<"token: "<<lookaheadToken1->GetTokenAttribute();
//    }

}

void Parser::CheckValidToken(const shared_ptr<Token>& token) {
    if(token->GetTokenName()=="<ColourLiteral>") {//hex number
        string colourLiteral=token->GetTokenAttribute();
        for(int i=1;i<colourLiteral.length();i++){
            bool validChar=validHexCharacters.find(colourLiteral[i])!= string::npos;
            if(!validChar){
                token->SetTokenName("<Invalid>");
            }
        }
    }
    if(token->GetTokenName()=="<SpecialCase1>") {//special case1 with __
        if(specialCase1[token->GetTokenAttribute()].empty()){
            token->SetTokenName("<Invalid>");
        }
        else{
            token->SetTokenName(specialCase1[token->GetTokenAttribute()]);
        }
    }
    if(token->GetTokenName()=="<SpecialCase2>") {//special case2 with __
        if(specialCase2[token->GetTokenAttribute()].empty()){
            token->SetTokenName("<Invalid>");
        }
        else{
            token->SetTokenName(specialCase2[token->GetTokenAttribute()]);
        }
    }
    if(token->GetTokenName()=="<SpecialCase3>") {//special case3 with __
        if(specialCase3[token->GetTokenAttribute()].empty()){
            token->SetTokenName("<Invalid>");
        }
        else{
            token->SetTokenName(specialCase3[token->GetTokenAttribute()]);
        }
    }

    //Error Checking
    if(token->GetTokenName()=="<Empty>") {//empty file
        cout << token->GetTokenAttribute()<< endl;
        exit(0);
    }
    if(token->GetTokenName()=="<Invalid>") {//invalid token
        cerr<<"\nError: Invalid Token: \n"<<token->GetTokenAttribute()<<endl;
        exit(3);
    }
    if(token->GetTokenName()=="<InvalidComment>") {//invalid comment
        cerr<<"\nError: Invalid Comment: \n"<<token->GetTokenAttribute()<<endl;
        exit(3);
    }
}

//Destructor for class
Parser::~Parser(){
    program.reset();
    lexer.reset();
    lookaheadToken1.reset();
    lookaheadToken2.reset();
}


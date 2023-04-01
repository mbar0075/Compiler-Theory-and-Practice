#include "HeaderFile.h"

shared_ptr<ASTProgram> Parser::ParseProgram(fstream &readFilePointer) {
    file = move(readFilePointer);
    vector<shared_ptr<ASTStatement>> allStatements;
    GetNextToken();
    GetNextToken();
    while (!file.eof()) {
        cout<<"token: "<<lookaheadToken1->GetTokenAttribute();
        allStatements.push_back(ParseStatement());
    }

    return shared_ptr<ASTProgram>(make_shared<ASTProgram>(allStatements));

}

shared_ptr<ASTStatement> Parser::ParseStatement() {
    auto singleStatement = make_shared<ASTStatement>();
    bool checkSemiColon=false;
    if(lookaheadToken1->GetTokenName()=="<Identifier>"){
        singleStatement = ParseAssignment();
        checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<{>"){
//        GetNextToken();
//        singleStatement = ParseBlock();
    }
    else if(lookaheadToken1->GetTokenName()=="<let>"){
//        GetNextToken();
//        singleStatement = ParseVariableDecl();
//        checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<__print>"){
//        GetNextToken();
//        singleStatement = ParsePrintStatement();
//        checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<return>"){
//        GetNextToken();
//        singleStatement = ParseReturnStatement();
//        checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<while>"){
//        GetNextToken();
//        singleStatement = ParseWhileStatement();
    }
    else if(lookaheadToken1->GetTokenName()=="<if>"){
        //GetNextToken();
        //singleStatement = ParseIfStatement();
    }
    else if(lookaheadToken1->GetTokenName()=="<for>"){
        //GetNextToken();
        //singleStatement = ParseForStatement();
    }
    else if(lookaheadToken1->GetTokenName()=="<__pixelr>"){
        //GetNextToken();
        //singleStatement = ParsePixel1Statement();
        //checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<__pixel>"){
        //GetNextToken();
        //singleStatement = ParsePixel2Statement();
        //checkSemiColon= true;
    }
    else if(lookaheadToken1->GetTokenName()=="<fun>"){
        //GetNextToken();
        //singleStatement = ParseFunStatement();
    }
    else if(lookaheadToken1->GetTokenName()=="<__delay>"){
        //GetNextToken();
        //singleStatement = ParseDelayStatement();
        //checkSemiColon= true;
    }
    else{
        cerr<<"\nError: Unexpected: "<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }


    if(checkSemiColon){
        if(lookaheadToken1->GetTokenName()!="<;>"){
            cerr<<"\nError: "<<lookaheadToken1->GetTokenAttribute()<<" ; expected "<<endl;
            exit(4);
        }
    }

    return singleStatement;
}

shared_ptr<ASTAssignment> Parser::ParseAssignment() {
    auto identifier = ParseIdentifier();
    GetNextToken();
    if(lookaheadToken1->GetTokenName()!="<=>"){
        cerr<<"\nError: Expected =, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    auto expression = ParseExpression();
    cout<<identifier->identifier<<" "<<expression->simpleExpressions[0]<<endl;
    return make_shared<ASTAssignment>(identifier,expression);
}

shared_ptr<ASTIdentifier> Parser::ParseIdentifier(){
    if(lookaheadToken1->GetTokenName()!="<Identifier>"){
        cerr<<"\nError: Expected identifier, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTIdentifier>(lookaheadToken1->GetTokenAttribute());
}

shared_ptr<ASTExpr> Parser::ParseExpression() {
    vector<shared_ptr<ASTSimpleExpr>> simpleExpressions;
    simpleExpressions.push_back(ParseSimpleExpression());
    GetNextToken();
    while (lookaheadToken1->GetTokenName()=="<RelationalOp>" ) {
        simpleExpressions.push_back(ParseSimpleExpression());
        GetNextToken();
    }

    return shared_ptr<ASTExpr>(make_shared<ASTExpr>(simpleExpressions));
}

shared_ptr<ASTSimpleExpr> Parser::ParseSimpleExpression() {
    vector<shared_ptr<ASTTerm>> terms;
    terms.push_back(ParseTerm());
    GetNextToken();
    while (lookaheadToken1->GetTokenName()=="<AdditiveOp>" ) {
        terms.push_back(ParseTerm());
        GetNextToken();
    }

    return shared_ptr<ASTSimpleExpr>(make_shared<ASTSimpleExpr>(terms));
}

shared_ptr<ASTTerm> Parser::ParseTerm() {
    vector<shared_ptr<ASTFactor>> factors;
    factors.push_back(ParseFactor());
    GetNextToken();
    while (lookaheadToken1->GetTokenName()=="<MultiplicativeOp>" ) {
        factors.push_back(ParseFactor());
        GetNextToken();
    }

    return shared_ptr<ASTTerm>(make_shared<ASTTerm>(factors));
}

shared_ptr<ASTLiteral> Parser::ParseLiteral() {
    auto literal= make_shared<ASTLiteral>();
    if(lookaheadToken1->GetTokenName() == "<IntegerLiteral>"){
        literal=make_shared<ASTIntLiteral>(lookaheadToken1->GetTokenAttribute());
    }
    else if(lookaheadToken1->GetTokenName() == "<BooleanLiteral>"){
        literal=make_shared<ASTBoolLiteral>(lookaheadToken1->GetTokenAttribute());
    }
    else if(lookaheadToken1->GetTokenName() == "<FloatLiteral>"){
        literal=make_shared<ASTFloatLiteral>(lookaheadToken1->GetTokenAttribute());
    }
    else if(lookaheadToken1->GetTokenName() == "<ColourLiteral>"){
        literal=make_shared<ASTColourLiteral>(lookaheadToken1->GetTokenAttribute());
    }
    else if(lookaheadToken1->GetTokenName() == "<PadWidth>"){
        literal=make_shared<ASTPadWidth>(lookaheadToken1->GetTokenAttribute());
    }
    else if(lookaheadToken1->GetTokenName() == "<PadHeight>"){
        literal=make_shared<ASTPadHeight>(lookaheadToken1->GetTokenAttribute());
    }
    else if(lookaheadToken1->GetTokenName() == "<__read>"){
        //Need expression
        literal=make_shared<ASTPadRead>(lookaheadToken1->GetTokenAttribute());
    }
    else{
        cerr<<"\nError: Expected Literal, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return literal;
}

shared_ptr<ASTFactor> Parser::ParseFactor() {
    auto factor= make_shared<ASTFactor>();
    if(lookaheadToken1->GetTokenName() == "<__read>"||lookaheadToken1->GetTokenName() == "<PadHeight>"||lookaheadToken1->GetTokenName() == "<PadWidth>"||lookaheadToken1->GetTokenName() == "<ColourLiteral>"||lookaheadToken1->GetTokenName() == "<IntegerLiteral>" ||lookaheadToken1->GetTokenName() == "<BooleanLiteral>" ||lookaheadToken1->GetTokenName() == "<FloatLiteral>"){
        factor=ParseLiteral();
    }
    else if(lookaheadToken1->GetTokenName() == "<Identifier>" && lookaheadToken2->GetTokenName() == "<(>"){
        factor=ParseFunctionCall();
    }
    else if(lookaheadToken1->GetTokenName() == "<Identifier>"){
        factor=ParseIdentifier();
    }
        //SubExpr
    else if(lookaheadToken1->GetTokenName() == "<(>"){

    }
        //Unary - this need to arrange
    else if((lookaheadToken1->GetTokenName() == "<AdditiveOp>" && lookaheadToken1->GetTokenAttribute()=="-")|| lookaheadToken1->GetTokenName() == "<not>"){

    }
        //PadRandI
    else if(lookaheadToken1->GetTokenName() == "<__randi>"){

    }
    else{
        cerr<<"\nError: Expected Factor, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return factor;
}

shared_ptr<ASTFunctionCall> Parser::ParseFunctionCall() {
    auto identifier=ParseIdentifier();
    GetNextToken();
    if(lookaheadToken1->GetTokenName()!="<(>"){
        cerr<<"\nError: Expected (, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    auto actualParams = ParseActualParams();
    GetNextToken();
    if(lookaheadToken1->GetTokenName()!="<)>"){
        cerr<<"\nError: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTFunctionCall>(identifier,actualParams);
}

shared_ptr<ASTActualParams> Parser::ParseActualParams() {
    vector<shared_ptr<ASTExpr>> expressions;
    expressions.push_back(ParseExpression());
    GetNextToken();
    while (lookaheadToken1->GetTokenName()=="<Comma>" ) {
        expressions.push_back(ParseExpression());
        GetNextToken();
    }

    return shared_ptr<ASTActualParams>(make_shared<ASTActualParams>(expressions));
}


void Parser::GetNextToken() {
    lookaheadToken1=lookaheadToken2;
    if(!file.eof()){
        lookaheadToken2 = lexer->GetNextToken(file);
        CheckValidToken(lookaheadToken2);
        while(!file.eof() &&lookaheadToken2->GetTokenName()=="<TokenDelim>"||lookaheadToken2->GetTokenName()=="<TokenDelimEnter>") {
            lookaheadToken2 = lexer->GetNextToken(file);
            CheckValidToken(lookaheadToken2);
        }
    }
    else{
        EOFFlag=true;
    }
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


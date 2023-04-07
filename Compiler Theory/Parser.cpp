//Inclusion of relevant header File
#include "HeaderFile.h"
//Method which Initiates XML Pass
void Parser::XMLPass(fstream &readFilePointer) {
    //Parsing Program
    this->program =ParseProgram(readFilePointer);
    //Creating new XML Visitor Node, and initiating accept method from node
    auto* node= new XMLVisitorNode();
    program->accept(node);
}
//Method which Parses a Program given a file pointer, and returns a pointer of ASTProgram
shared_ptr<ASTProgram> Parser::ParseProgram(fstream &readFilePointer) {
    /*Matching Case:〈Program〉 ::= { 〈Statement〉 }*/
    file = move(readFilePointer);
    vector<shared_ptr<ASTStatement>> allStatements;
    GetNextToken();
    GetNextToken();
    //Looping until not reached end of file
    while (!EOFFlag||(lookaheadToken1->GetTokenName()=="<{>"&&lookaheadToken2->GetTokenName()=="<}>")) {
        allStatements.push_back(ParseStatement());
        if(EOFFlag){
            break;
        }
    }
    return shared_ptr<ASTProgram>(make_shared<ASTProgram>(allStatements));
}
//Method which Parses a Statement, and returns a pointer of ASTStatement
shared_ptr<ASTStatement> Parser::ParseStatement() {
    /*Matching Case:〈Statement〉 ::= 〈VariableDecl〉 ‘;’
                                    | 〈Assignment〉 ‘;’
                                    | 〈PrintStatement〉 ‘;’
                                    | 〈DelayStatement〉 ‘;’
                                    | 〈PixelStatement〉 ‘;’
                                    | 〈IfStatement〉
                                    | 〈ForStatement〉
                                    | 〈WhileStatement〉
                                    | 〈RtrnStatement〉 ‘;’
                                    | 〈FunctionDecl〉
                                    | 〈Block〉*/
    shared_ptr<ASTStatement> singleStatement;
    bool checkSemiColon=false;//Variable being used to check whether statement requires a semicolon delimeter
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
        cerr<<"\n Statement Error: Unexpected: "<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    if(checkSemiColon){
        //Error Checking when ; is not matched for the relevant cases
        if(lookaheadToken1->GetTokenName()!="<;>"){
            cerr<<"\n Statement Error: "<<lookaheadToken1->GetTokenAttribute()<<" ; expected "<<endl;
            exit(4);
        }
        GetNextToken();
    }
    return singleStatement;
}
//Method which Parses a Block, and returns a pointer of ASTBlock
shared_ptr<ASTBlock> Parser::ParseBlock() {
    /*Matching Case:〈Block〉 ::= ‘{’ { 〈Statement〉 } ‘}’*/
    vector<shared_ptr<ASTStatement>> statements;
    shared_ptr<ASTStatement> singleStatement;
    //Error Checking when { is not matched
    if(lookaheadToken1->GetTokenName()!="<{>"){
        cerr<<"\nBlock Error: Expected {, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    //Looping until next token is not a }
    while (lookaheadToken1->GetTokenName()!="<}>") {
        singleStatement=ParseStatement();
        statements.push_back(singleStatement);
    }
    //Error Checking when } is not matched
    if(lookaheadToken1->GetTokenName()!="<}>"){
        cerr<<"\nBlock Error: Expected }, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    return make_shared<ASTBlock>(statements);
}
//Method which Parses an If Statement, and returns a pointer of ASTIfStatement
shared_ptr<ASTIfStatement> Parser::ParseIfStatement() {
    /*Matching Case: 〈IfStatement〉 ::= ‘if’ ‘(’ 〈Expr 〉 ‘)’ 〈Block〉 [ ‘else’ 〈Block〉 ]*/
    shared_ptr<ASTExpr> expression;
    shared_ptr<ASTBlock> firstBlock;
    shared_ptr<ASTBlock> secondBlock=nullptr;
    if(lookaheadToken1->GetTokenName()=="<if>"&&lookaheadToken2->GetTokenName()=="<(>"){
        GetNextToken();
        GetNextToken();
        expression=ParseExpression();
        //Error Checking when ) is not matched
        if(lookaheadToken1->GetTokenName()!="<)>"){
            cerr<<"\nIf Statement Error: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        firstBlock=ParseBlock();
        //Checking for else keyword
        if(lookaheadToken1->GetTokenName()=="<else>"){
            GetNextToken();
            secondBlock=ParseBlock();
        }
    }else{//Error Checking when if is not matched
        cerr<<"\nIf Statement Error: Expected if, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTIfStatement>(expression,firstBlock,secondBlock);
}
//Method which Parses a While Statement and returns a pointer of ASTWhileStatement
shared_ptr<ASTWhileStatement> Parser::ParseWhileStatement() {
    /*Matching Case:〈WhileStatement〉 ::= ‘while’ ‘(’ 〈Expr 〉 ‘)’ 〈Block〉*/
    shared_ptr<ASTExpr> expression;
    shared_ptr<ASTBlock> block;
    if(lookaheadToken1->GetTokenName()=="<while>"&&lookaheadToken2->GetTokenName()=="<(>"){
        GetNextToken();
        GetNextToken();
        expression=ParseExpression();
        //Error Checking when ) is not matched
        if(lookaheadToken1->GetTokenName()!="<)>"){
            cerr<<"\nWhile Statement Error: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        block=ParseBlock();
    }else{//Error Checking when while ( is not matched
        cerr<<"\nWhile Statement Error: Expected while, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTWhileStatement>(expression,block);
}
//Method which Parses a Formal Parameter, and returns a pointer of ASTFormalParam
shared_ptr<ASTFormalParam> Parser::ParseFormalParam() {
    /*Matching Case:〈FormalParam〉 ::= 〈Identifier 〉 ‘:’ 〈Type〉*/
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTType> type;
    identifier=ParseIdentifier();
    GetNextToken();
    //Error Checking when : is not matched
    if(lookaheadToken1->GetTokenName()!="<:>"){
        cerr<<"\nFormal Parameter Error: Expected :, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    type=ParseType();
    GetNextToken();
    return make_shared<ASTFormalParam>(identifier,type);
}
//Method which Parses Formal Parameters, and returns a pointer of ASTFormalParams
shared_ptr<ASTFormalParams> Parser::ParseFormalParams() {
    /*Matching Case:〈FormalParams〉 ::= 〈FormalParam〉 { ‘,’ 〈FormalParam〉 }*/
    vector<shared_ptr<ASTFormalParam>> formalParams;
    formalParams.push_back(ParseFormalParam());
    //Looping until next token is a Comma
    while(lookaheadToken1->GetTokenName()=="<Comma>" ){
        GetNextToken();
        formalParams.push_back(ParseFormalParam());
    }
    return make_shared<ASTFormalParams>(formalParams);
}
//Method which Parses a Function Declaration, and returns a pointer of ASTFunctionDecl
shared_ptr<ASTFunctionDecl> Parser::ParseFunctionDecl() {
    /*Matching Case:〈FunctionDecl〉 ::= ‘fun’ 〈Identifier 〉 ‘(’ [ 〈FormalParams〉 ] ‘)’ ‘->’ 〈Type〉 〈Block〉*/
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
            //Error Checking when ) is not matched
            if(lookaheadToken1->GetTokenName()!="<)>"){
                cerr<<"\nFunction Declaration Error: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
                exit(4);
            }
            GetNextToken();
            //Error Checking when -> is not matched
            if(lookaheadToken1->GetTokenName()!="<->>"){
                cerr<<"\nFunction Declaration Error: Expected ->, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
                exit(4);
            }
            GetNextToken();
            type=ParseType();
            GetNextToken();
            block=ParseBlock();
        }
    }else{//Error Checking when fun is not matched
        cerr<<"\nFunction Declaration Error: Expected fun, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTFunctionDecl>(identifier, formalParams, type, block);
}
//Method which Parses a For Statement, and returns a pointer of ASTForStatement
shared_ptr<ASTForStatement> Parser::ParseForStatement() {
    /*Matching Case:〈ForStatement〉 ::= ‘for’ ‘(’ [ 〈VariableDecl〉 ] ’;’ 〈Expr 〉 ’;’ [ 〈Assignment〉 ] ‘)’ 〈Block〉*/
    shared_ptr<ASTExpr> expression;
    shared_ptr<ASTAssignment> assignment=nullptr;
    shared_ptr<ASTVariableDecl> variableDecl=nullptr;
    shared_ptr<ASTBlock> block;
    if(lookaheadToken1->GetTokenName()=="<for>"){
        GetNextToken();
        //Error Checking when ( is not matched
        if(lookaheadToken1->GetTokenName()!="<(>") {
            cerr<<"\nFor Statement Error: Expected (, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        if(lookaheadToken1->GetTokenName()!="<;>") {
            variableDecl = ParseVariableDecl();
        }
        //Error Checking when ; is not matched
        if(lookaheadToken1->GetTokenName()!="<;>") {
            cerr<<"\nFor Statement Error: Expected ;, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        expression=ParseExpression();
        //Error Checking when ; is not matched
        if(lookaheadToken1->GetTokenName()!="<;>") {
            cerr<<"\nExpression Error: Expected ;, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        if(lookaheadToken1->GetTokenName()!="<)>") {
            assignment=ParseAssignment();
        }
        //Error Checking when ) is not matched
        if(lookaheadToken1->GetTokenName()!="<)>") {
            cerr<<"\nExpression Error: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
            exit(4);
        }
        GetNextToken();
        block=ParseBlock();
    }else{//Error Checking when for is not matched
        cerr<<"\nExpression Error: Expected for, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTForStatement>(expression,assignment,variableDecl,block);
}
//Method which Parses a Pixel Statement, and returns a pointer of ASTPixelStatement
shared_ptr<ASTPixelStatement> Parser::ParsePixelStatement() {
    /*Matching Case:〈PixelStatement〉 ::= ‘__pixelr’ 〈Expr 〉‘,’〈Expr 〉‘,’〈Expr 〉‘,’〈Expr 〉‘,’〈Expr 〉
                                        | ‘__pixel’ 〈Expr 〉‘,’〈Expr 〉‘,’〈Expr 〉*/
    vector<shared_ptr<ASTExpr>> expressions;
    string value;
    if(lookaheadToken1->GetTokenName()=="<__pixelr>"){
        value=lookaheadToken1->GetTokenName();
        GetNextToken();
        for(int c =0; c<5;c++){//Looping for 5 times
            expressions.push_back(ParseExpression());
            //Error Checking when Comma is not matched
            if(c<4 && lookaheadToken1->GetTokenName()!="<Comma>"){
                cerr<<"\nPixel Statement Error: Expected ',', received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
                exit(4);
            }
            else if(c<4){
                GetNextToken();
            }
        }
    }
    else if(lookaheadToken1->GetTokenName()=="<__pixel>"){
        value=lookaheadToken1->GetTokenName();
        GetNextToken();
        for(int c =0; c<3;c++){//Looping for 3 times
            expressions.push_back(ParseExpression());
            //Error Checking when Comma is not matched
            if(c<2 && lookaheadToken1->GetTokenName()!="<Comma>"){
                cerr<<"\nPixel Statement Error: Expected ',', received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
                exit(4);
            }
            else if(c<2){
                GetNextToken();
            }
        }
    }
    else{//Error Checking when pixel keywords are not matched
        cerr<<"\nPixel Statement Error: Expected __pixelr or __pixel, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTPixelStatement>(value,expressions);
}
//Method which Parses a Print Statement, and returns pointer of ASTPrintStatement
shared_ptr<ASTPrintStatement> Parser::ParsePrintStatement() {
    /*Matching Case:〈PrintStatement〉 ::= ‘__print’ 〈Expr 〉*/
    shared_ptr<ASTExpr> expression;
    //Error Checking when __print is not matched
    if(lookaheadToken1->GetTokenName()!="<__print>"){
        cerr<<"\nPrint Statement Error: Expected __print, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    expression=ParseExpression();
    return make_shared<ASTPrintStatement>(expression);
}
//Method which Parses a Delay Statement, and returns pointer of ASTDelayStatement
shared_ptr<ASTDelayStatement> Parser::ParseDelayStatement() {
    /*Matching Case:〈DelayStatement〉 ::= ‘__delay’ 〈Expr 〉*/
    shared_ptr<ASTExpr> expression;
    //Error Checking when __delay is not matched
    if(lookaheadToken1->GetTokenName()!="<__delay>"){
        cerr<<"\nDelay Statement Error: Expected __delay, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    expression=ParseExpression();
    return make_shared<ASTDelayStatement>(expression);
}
//Method which Parses a Return Statement, and returns pointer of ASTRtrnStatement
shared_ptr<ASTRtrnStatement> Parser::ParseReturnStatement() {
    /*Matching Case:〈RtrnStatement〉 ::= ‘return’ 〈Expr 〉*/
    shared_ptr<ASTExpr> expression;
    //Error Checking when return is not matched
    if(lookaheadToken1->GetTokenName()!="<return>"){
        cerr<<"\nReturn Statement Error: Expected return, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    expression=ParseExpression();
    return make_shared<ASTRtrnStatement>(expression);
}
//Method which Parses a Variable Declaration, and returns pointer of ASTVariableDecl
shared_ptr<ASTVariableDecl> Parser::ParseVariableDecl() {
    /*Matching Case:〈VariableDecl〉 ::= ‘let’ 〈Identifier 〉 ‘:’ 〈Type〉 ‘=’ 〈Expr 〉*/
    GetNextToken();
    shared_ptr<ASTIdentifier> identifier = ParseIdentifier();
    GetNextToken();
    //Error Checking when : is not matched
    if(lookaheadToken1->GetTokenName()!="<:>"){
        cerr<<"\nVariable Declaration Error: Expected :, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    shared_ptr<ASTType> type;
    //Error Checking when Type token is not matched
    if(lookaheadToken1->GetTokenName()!="<Type>"){
        cerr<<"\nVariable Declaration Error: Expected Type, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    type=ParseType();
    GetNextToken();
    //Error Checking when = is not matched
    if(lookaheadToken1->GetTokenName()!="<=>"){
        cerr<<"\nVariable Declaration Error: Expected =, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    shared_ptr<ASTExpr> expression=ParseExpression();
    return make_shared<ASTVariableDecl>(identifier,type,expression);
}
//Method which Parses an Assignment Statement, and returns pointer of ASTAssignment
shared_ptr<ASTAssignment> Parser::ParseAssignment() {
    /*Matching Case:〈Assignment〉 ::= 〈Identifier 〉 ‘=’ 〈Expr 〉*/
    shared_ptr<ASTIdentifier> identifier = ParseIdentifier();
    GetNextToken();
    //Error Checking when = is not matched
    if(lookaheadToken1->GetTokenName()!="<=>"){
        cerr<<"\nAssignment Statement Error: Expected =, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    shared_ptr<ASTExpr> expression = ParseExpression();
    return make_shared<ASTAssignment>(identifier,expression);
}
//Method which Parses an Identifier, and returns pointer of ASTIdentifier
shared_ptr<ASTIdentifier> Parser::ParseIdentifier(){
    /*(Retrieving Identifier from Lexer Token)
     * Matching Case:〈Identifier 〉 ::= ( 〈Letter 〉 ) { ‘ ’ | 〈Letter 〉 | 〈Digit〉 }*/
    if(lookaheadToken1->GetTokenName()!="<Identifier>"){
        cerr<<"\nIdentifier Error: Expected Identifier, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTIdentifier>(lookaheadToken1->GetTokenAttribute());
}
//Method which Parses a Type, and returns pointer of ASTType
shared_ptr<ASTType> Parser::ParseType() {
    /*(Retrieving Type from Lexer Token)
     * Matching Case:〈Type〉 ::= ‘float’ | ‘int’ | ‘bool’ | ‘colour’*/
    return make_shared<ASTType>(lookaheadToken1->GetTokenAttribute());
}
//Method which Parses an Expression, and returns pointer of ASTExpr
shared_ptr<ASTExpr> Parser::ParseExpression() {
    /*Matching Case:〈Expr 〉 ::= 〈SimpleExpr 〉 { 〈RelationalOp〉 〈SimpleExpr 〉 }*/
    shared_ptr<ASTExpr> expr;
    shared_ptr<ASTExpr> leftExpr;
    leftExpr=ParseSimpleExpression();
    //Checking whether Expression has right Simple Expression
    if(lookaheadToken1->GetTokenName()=="<RelationalOp>" ) {
        string relationalOp=lookaheadToken1->GetTokenAttribute();
        GetNextToken();
        shared_ptr<ASTExpr> rightExpr;
        rightExpr=ParseExpression();
        expr=shared_ptr<ASTRelationalOp>(make_shared<ASTRelationalOp>(leftExpr, relationalOp, rightExpr));
    }
    else{
        expr = move(leftExpr);
    }
    return expr;
}
//Method which Parses a Simple Expression, and returns pointer of ASTExpr
shared_ptr<ASTExpr> Parser::ParseSimpleExpression() {
    /*Matching Case:〈SimpleExpr 〉 ::= 〈Term〉 { 〈AdditiveOp〉 〈Term〉 }*/
    shared_ptr<ASTExpr> simpleExpr;
    shared_ptr<ASTExpr> leftSimpleExpr;
    leftSimpleExpr=ParseTerm();
    //Checking whether Simple Expression has right Term
    if(lookaheadToken1->GetTokenName()=="<AdditiveOp>" ) {
        string additiveOp=lookaheadToken1->GetTokenAttribute();
        GetNextToken();
        shared_ptr<ASTExpr> rightSimpleExpr;
        rightSimpleExpr=ParseSimpleExpression();
        simpleExpr=shared_ptr<ASTAdditiveOp>(make_shared<ASTAdditiveOp>(leftSimpleExpr, additiveOp, rightSimpleExpr));
    }
    else{
        simpleExpr = move(leftSimpleExpr);
    }
    return simpleExpr;
}
//Method which Parses a Term, and returns pointer of ASTTerm
shared_ptr<ASTExpr> Parser::ParseTerm() {
    /*Matching Case:〈Term〉 ::= 〈Factor 〉 { 〈MultiplicativeOp〉 〈Factor 〉 }*/
    shared_ptr<ASTExpr> term;
    shared_ptr<ASTExpr> leftTerm;
    leftTerm=ParseFactor();
    //Checking whether Term has right Factor
    if(lookaheadToken1->GetTokenName()=="<MultiplicativeOp>" ) {
        string multiplicativeOp=lookaheadToken1->GetTokenAttribute();
        GetNextToken();
        shared_ptr<ASTExpr> rightTerm;
        rightTerm=ParseTerm();
        term=shared_ptr<ASTMultiplicativeOp>(make_shared<ASTMultiplicativeOp>(leftTerm, multiplicativeOp, rightTerm));
    }
    else{
        term = move(leftTerm);
    }
    return term;
}
//Method which Parses a Literal, and returns pointer of ASTLiteral
shared_ptr<ASTLiteral> Parser::ParseLiteral() {
    /*Matching Case:〈Literal〉 ::= 〈BooleanLiteral〉
                                | 〈IntegerLiteral〉
                                | 〈FloatLiteral〉
                                | 〈ColourLiteral〉
                                | 〈PadWidth〉
                                | 〈PadHeight〉
                                | 〈PadRead〉*/
    auto literal= make_shared<ASTLiteral>();
    if(lookaheadToken1->GetTokenName() == "<IntegerLiteral>"){
        literal=make_shared<ASTIntLiteral>(lookaheadToken1->GetTokenAttribute());
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<BooleanLiteral>"){
        literal=make_shared<ASTBoolLiteral>(lookaheadToken1->GetTokenAttribute());
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<FloatLiteral>"){
        literal=make_shared<ASTFloatLiteral>(lookaheadToken1->GetTokenAttribute());
        GetNextToken();
    }
    else if(lookaheadToken1->GetTokenName() == "<ColourLiteral>"){
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
        literal=ParsePadRead();
    }
    else{//Error Checking when literal is not matched
        cerr<<"\nLiteral Error: Expected Literal, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return literal;
}
//Method which Parses Pad Read, and returns pointer of ASTPadRead
shared_ptr<ASTPadRead> Parser::ParsePadRead() {
    /*Matching Case:〈PadRead〉 :: = ‘__read’ 〈Expr 〉‘,’〈Expr 〉*/
    shared_ptr<ASTExpr> firstExpression;
    shared_ptr<ASTExpr> secondExpression;
    //Error Checking when __read is not matched
    if(lookaheadToken1->GetTokenName()!="<__read>"){
        cerr<<"\nPad Read Error: Expected __read, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    firstExpression = ParseExpression();
    //Error Checking when Comma is not matched
    if(lookaheadToken1->GetTokenName()!="<Comma>"){
        cerr<<"\nPad Read Error: Expected ',', received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    secondExpression = ParseExpression();
    return make_shared<ASTPadRead>(firstExpression,secondExpression);
}
//Method which Parses a Factor, and returns pointer of ASTExpr
shared_ptr<ASTExpr> Parser::ParseFactor() {
    /*Matching Case:〈Factor 〉 ::= 〈Literal〉
                                | 〈Identifier 〉
                                | 〈FunctionCall〉
                                | 〈SubExpr 〉
                                | 〈Unary〉
                                | 〈PadRandI〉
                                | 〈PadWidth〉
                                | 〈PadHeight〉
                                | 〈PadRead〉*/
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
    else if(lookaheadToken1->GetTokenName() == "<(>"){
        factor=ParseSubExpression();
        GetNextToken();
    }
    else if((lookaheadToken1->GetTokenName() == "<AdditiveOp>" && lookaheadToken1->GetTokenAttribute()=="-")|| lookaheadToken1->GetTokenName() == "<not>"){
        factor=ParseUnary();
    }
    else if(lookaheadToken1->GetTokenName() == "<__randi>"){
        factor=ParsePadRandi();
    }
    else{//Error Checking when not factor is matched
        cerr<<"\nFactor Error: Expected Factor, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return factor;
}
//Method which Parses a Function Call, and returns pointer of ASTFunctionCall
shared_ptr<ASTFunctionCall> Parser::ParseFunctionCall() {
    /*Matching Case:〈FunctionCall〉 ::= 〈Identifier 〉 ‘(’ [ 〈ActualParams〉 ] ‘)’*/
    shared_ptr<ASTIdentifier> identifier=ParseIdentifier();
    GetNextToken();
    //Error Checking when ( is not matched
    if(lookaheadToken1->GetTokenName()!="<(>"){
        cerr<<"\nFunction Call Error: Expected (, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    shared_ptr<ASTActualParams> actualParams;
    //Checkint whether function has parameters
    if(lookaheadToken1->GetTokenName()!="<)>"){
        actualParams = ParseActualParams();
    }
    //Error Checking when ) is not matched
    if(lookaheadToken1->GetTokenName()!="<)>"){
        cerr<<"\nFunction Call Error: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTFunctionCall>(identifier,actualParams);
}
//Method which Parses Actual Parameters, and returns pointer of ASTActualParams
shared_ptr<ASTActualParams> Parser::ParseActualParams() {
    /*Matching Case:〈ActualParams〉 ::= 〈Expr 〉 { ‘,’ 〈Expr 〉 }*/
    vector<shared_ptr<ASTExpr>> expressions;
    expressions.push_back(ParseExpression());
    //Looping until next token is a Comma
    while (lookaheadToken1->GetTokenName()=="<Comma>" ) {
        GetNextToken();
        expressions.push_back(ParseExpression());
    }
    return make_shared<ASTActualParams>(expressions);
}
//Method which Parses a Unary Expression, and returns pointer of ASTExpr
shared_ptr<ASTExpr> Parser::ParseUnary() {
    /*Matching Case:〈Unary〉 ::= ( ‘-’ | ‘not’ ) 〈Expr 〉*/
    string unaryOperator;
    if((lookaheadToken1->GetTokenName()=="<AdditiveOp>"&& lookaheadToken1->GetTokenAttribute()=="-")||lookaheadToken1->GetTokenName()=="<not>"){
        unaryOperator=lookaheadToken1->GetTokenAttribute();
        GetNextToken();
    }
    else{//Error Checking when Unary Operator is not matched
        cerr<<"\nUnary Expression Error: Expected - or not, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    shared_ptr<ASTExpr> expression=ParseExpression();
    return make_shared<ASTUnary>(expression,unaryOperator);
}
//Method which Parses PadRandi, and returns pointer of ASTPadRandi
shared_ptr<ASTExpr> Parser::ParsePadRandi() {
    /*Matching Case:〈PadRandI〉 :: = ‘__randi’ 〈Expr 〉*/
    shared_ptr<ASTExpr> expression;
    //Error Checking when __randi is not matched
    if(lookaheadToken1->GetTokenName()!="<__randi>"){
        cerr<<"\nPad Randi Error: Expected __randi, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    expression=ParseExpression();
    return make_shared<ASTPadRandi>(expression);
}
//Method which Parses a Sub Expression, and returns pointer of ASTExpr
shared_ptr<ASTExpr> Parser::ParseSubExpression() {
    /*Matching Case:〈SubExpr 〉 ::= ‘(’ 〈Expr 〉 ‘)’*/
    shared_ptr<ASTExpr> expression;
    //Error Checking when ( is not matched
    if(lookaheadToken1->GetTokenName()!="<(>"){
        cerr<<"\nSub Expression Error: Expected (, received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    GetNextToken();
    //Error Checking when ) is not matched
    if(lookaheadToken1->GetTokenName()!="<)>"){
        expression = ParseExpression();
    }
    if(lookaheadToken1->GetTokenName()!="<)>"){
        cerr<<"\nSub Expression Error: Expected ), received:"<<lookaheadToken1->GetTokenAttribute()<<endl;
        exit(4);
    }
    return make_shared<ASTSubExpr>(expression);
}
//Method which communicates with the Lexer, and updates the two lookahead tokens
void Parser::GetNextToken() {
    //Setting lookaheadtoken1 to lookaheadtoken2
    lookaheadToken1=lookaheadToken2;
    //Checking whether end of file is reached, if so, setting the relevant flag
    if(!file.eof()){
        //Retrieving token from lexer, and storing in lookaheadtoken2, and checking when token is valid
        lookaheadToken2 = lexer->GetNextToken(file);
        CheckValidToken(lookaheadToken2);
        //Continue to loop is token is one of the special cases, which will be ignored during parsing
        while((lookaheadToken2->GetTokenName()=="<TokenDelimEnter>"||lookaheadToken2->GetTokenName()=="<TokenDelim>"||lookaheadToken2->GetTokenName()=="<Comment>")) {
            lookaheadToken2 = lexer->GetNextToken(file);
            CheckValidToken(lookaheadToken2);
        }
    }
    else{
        EOFFlag=true;
    }
    if(lookaheadToken2->GetTokenName()=="<Empty>"){
        EOFFlag=true;
    }
}
//Method which Checks whether Token is Valid, given a token
void Parser::CheckValidToken(const shared_ptr<Token>& token) {
    //Checking Valid Colour Literal (Hex Number)
    if(token->GetTokenName()=="<ColourLiteral>") {
        string colourLiteral=token->GetTokenAttribute();
        for(int i=1;i<colourLiteral.length();i++){
            bool validChar=validHexCharacters.find(colourLiteral[i])!= string::npos;
            if(!validChar){
                token->SetTokenName("<Invalid>");
            }
        }
    }
    //Checking Special Case 1 (tokens which start with __)
    if(token->GetTokenName()=="<SpecialCase1>") {
        if(specialCase1[token->GetTokenAttribute()].empty()){
            token->SetTokenName("<Invalid>");
        }
        else{
            token->SetTokenName(specialCase1[token->GetTokenAttribute()]);
        }
    }
    //Checking Special Case 2 (tokens which start with __)
    if(token->GetTokenName()=="<SpecialCase2>") {
        if(specialCase2[token->GetTokenAttribute()].empty()){
            token->SetTokenName("<Invalid>");
        }
        else{
            token->SetTokenName(specialCase2[token->GetTokenAttribute()]);
        }
    }
    //Checking Special Case 3 (tokens which start with __)
    if(token->GetTokenName()=="<SpecialCase3>") {
        if(specialCase3[token->GetTokenAttribute()].empty()){
            token->SetTokenName("<Invalid>");
        }
        else{
            token->SetTokenName(specialCase3[token->GetTokenAttribute()]);
        }
    }
    //Relevant Error Printing if token is Invalid
    if(token->GetTokenName()=="<Invalid>") {//Invalid Token
        cerr<<"\nError: Invalid Token: \n"<<token->GetTokenAttribute()<<endl;
        exit(3);
    }
    if(token->GetTokenName()=="<InvalidComment>") {//Invalid Comment
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
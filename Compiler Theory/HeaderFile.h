//
// Created by Matthias on 24/03/2023.
//
//Inclusion of relevant header File
#include "VisitorNodes.h"

//Token Class with its relevant variables and methods
class Token{
private:
    string name;
    string attribute;
public:
    Token(string name,string attribute);
    string GetTokenName();
    string GetTokenAttribute();
    void SetTokenName(string newName);
    Token()=default;
    ~Token()=default;
};

//Lexer Class with its relevant variables and methods
class Lexer{
private:
    stack<string> stack;//Stack
    map<string,string> catTable;
    map<string,string> tokenTable;
    map<string,string> identifierTable;
    map<string,map<string,string>> transitionTable;
    void ClearStack();
    void LoadTables();
public:
    shared_ptr<Token> GetNextToken(fstream &readFilePointer);
    Lexer();
    ~Lexer()=default;
};

//Parser Class with its relevant variables and methods
class Parser{
private:
    string validHexCharacters="ABCDEFabcdef0123456789";
    map<string,string> specialCase1={{ "__read", "<__read>" }};
    map<string,string> specialCase2={{ "__clear", "<__clear>" },{ "__randi", "<__randi>" },{ "__print", "<__print>" },{ "__delay", "<__delay>" },{ "__pixel", "<__pixel>" },{ "__width", "<PadWidth>" }};
    map<string,string> specialCase3={{ "__pixelr", "<__pixelr>" },{ "__height", "<PadHeight>" }};
    unique_ptr<Lexer> lexer= make_unique<Lexer>();
    shared_ptr<Token> lookaheadToken1;
    shared_ptr<Token> lookaheadToken2;
    shared_ptr<ASTProgram> program;
    fstream file;
    bool EOFFlag=false;
    void CheckValidToken( const shared_ptr<Token>& token);
public:
    shared_ptr<ASTProgram> ParseProgram(fstream &readFilePointer);
    shared_ptr<ASTStatement> ParseStatement();
    shared_ptr<ASTAssignment> ParseAssignment();
    shared_ptr<ASTIdentifier> ParseIdentifier();
    shared_ptr<ASTExpr> ParseExpression();
    shared_ptr<ASTExpr> ParseFactor();
    shared_ptr<ASTLiteral> ParseLiteral();
    shared_ptr<ASTBlock> ParseBlock();
    shared_ptr<ASTExpr> ParseSimpleExpression();
    shared_ptr<ASTExpr> ParseTerm();
    shared_ptr<ASTActualParams> ParseActualParams();
    shared_ptr<ASTFunctionCall> ParseFunctionCall();
    shared_ptr<ASTVariableDecl> ParseVariableDecl();
    shared_ptr<ASTExpr> ParsePadRandi();
    shared_ptr<ASTExpr> ParseSubExpression();
    shared_ptr<ASTExpr> ParseUnary();
    shared_ptr<ASTPadRead> ParsePadRead();
    shared_ptr<ASTPrintStatement> ParsePrintStatement();
    shared_ptr<ASTClearStatement> ParseClearStatement();
    shared_ptr<ASTDelayStatement> ParseDelayStatement();
    shared_ptr<ASTRtrnStatement> ParseReturnStatement();
    shared_ptr<ASTPixelStatement> ParsePixelStatement();
    shared_ptr<ASTIfStatement> ParseIfStatement();
    shared_ptr<ASTForStatement> ParseForStatement();
    shared_ptr<ASTWhileStatement> ParseWhileStatement();
    shared_ptr<ASTFormalParam> ParseFormalParam();
    shared_ptr<ASTFormalParams> ParseFormalParams();
    shared_ptr<ASTFunctionDecl> ParseFunctionDecl();
    void XMLPass();
    void SemanticPass();
    void CodeGenerationPass();
    void Compile(fstream &readFilePointer);
    shared_ptr<ASTType> ParseType();
    void GetNextToken();
    Parser()=default;
    ~Parser();
};
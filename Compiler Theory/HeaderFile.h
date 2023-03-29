//
// Created by Matthias on 24/03/2023.
//
//Inclusion of Libraries
#include <iostream>
#include <fstream>//read from file
#include <sstream>//read from string
#include <string>//string
#include <memory>//unique pointers
#include <map>//Dictionary/tables
#include <stack>//stack
#include <list>//For AST dynamic list
#include <utility>//For shared ptr

using namespace std;

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

class ASTNode{
private:
    string ASTType;
    string nodeName;
    shared_ptr<ASTNode> parent;
    list< shared_ptr<ASTNode>> children;
public:
    ASTNode(string ASTType,string nodeName,shared_ptr<ASTNode> newparent);
    string GetASTType();
    string GetNodeName();
    shared_ptr<ASTNode> GetNodeParent();
    list< shared_ptr<ASTNode>> GetChildren();
    void CreateChild(const shared_ptr<ASTNode>& child);
    void ShowChildren();
    void SetNodeParent(shared_ptr<ASTNode> newparent);
    ASTNode()=default;
    ~ASTNode();
};

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

class Parser{
private:
    string validHexCharacters="ABCDEFabcdef0123456789";
    map<string,string> specialCase1={{ "__read", "<__read>" }};
    map<string,string> specialCase2={{ "__randi", "<__randi>" },{ "__print", "<__print>" },{ "__delay", "<__delay>" },{ "__pixel", "<__pixel>" },{ "__width", "<PadWidth>" }};
    map<string,string> specialCase3={{ "__pixelr", "<__pixelr>" },{ "__height", "<PadHeight>" }};
    unique_ptr<Lexer> lexer= make_unique<Lexer>();
    shared_ptr<Token> currentToken;
    shared_ptr<Token> lookaheadToken1;
    stack<string> stack;//Stack
    map<string,string> tmpTable;
    map<string,string> terminals;
    map<string,map<string,string>> parseTable;
    //shared_ptr<ASTNode> ASTROOT;
    void CheckValidToken(const shared_ptr<Token>& token);
    void LoadTables();
public:
    void LLKParse(fstream &readFilePointer);
    Parser();
    ~Parser();
};
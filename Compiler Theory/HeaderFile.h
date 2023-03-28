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

class Lexer{
private:
    stack<string> stack;//Stack
    map<string,string> catTable;
    map<string,string> tokenTable;
    map<string,string> identifierTable;
    map<string,map<string,string>> transitionTable;
    void ClearStack();
public:
    void LoadTables();
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
    void CheckValidToken(const shared_ptr<Token>& token);
    stack<string> stack;//Stack
    //map<string,map<string,string>> parseTable;
public:
    //void LoadTables();
    void LLKParse(fstream &readFilePointer);
    Parser()=default;
    ~Parser();
};
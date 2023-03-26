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
    void LoadData();
    shared_ptr<Token> GetNextToken(fstream &readFilePointer);
    Lexer()=default;
    ~Lexer()=default;
};

class Parser{
private:
    stack<string> stack;//Stack
    map<string,map<string,string>> parseTable;
public:
    Parser()=default;
    ~Parser()=default;
};
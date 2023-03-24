//
// Created by Matthias on 24/03/2023.
//
//Inclusion of Libraries
#include <iostream>
#include <fstream>//read from file
#include <sstream>//read from string
#include <string>
#include <memory>//unique pointers
#include <map>//Dictionary/tables
#include <stack>//stack

using namespace std;

class Lexer{
private:
    stack<string> stack;//Stack
    map<string,string> catTable;
    map<string,string> tokenTable;
    map<string,map<string,string>> transitionTable;
    void ClearStack();
public:
    void LoadData();
    string NextWord(string filename);
    Lexer()=default;
    ~Lexer()=default;
};
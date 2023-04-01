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
#include <vector>//For AST dynamic list
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

class ASTFactor;
class ASTIdentifier;
class ASTExpr;
class ASTStatement;
class ASTSimpleExpr;
class ASTTerm;
class  VisitorNode{};

class ASTNode{
public:
    ASTNode()=default;
    //virtual void accept(shared_ptr<VisitorNode> v)=0;
    ~ASTNode()=default;
};

class ASTProgram : public ASTNode{
public:
    ASTProgram()=default;
    explicit ASTProgram(const vector<shared_ptr<ASTStatement>>& program);
    vector<shared_ptr<ASTStatement>> program{};
    //void accept(shared_ptr<VisitorNode> v) override;
    ~ASTProgram();
};

class ASTStatement: public ASTProgram{
public:
    ASTStatement()=default;
    //void accept(shared_ptr<VisitorNode> v) override;
    ~ASTStatement()=default;
};



class ASTBlock: public ASTStatement{};

class ASTWhileStatement: public ASTStatement{};

class ASTForStatement: public ASTStatement{};

class ASTIfStatement: public ASTStatement{};

class ASTRtrnStatement: public ASTStatement{};

class ASTPixelStatement: public ASTStatement{};

class ASTDelayStatement: public ASTStatement{};

class ASTPrintStatement: public ASTStatement{};

class ASTVariableDecl: public ASTStatement{};


class ASTAssignment: public ASTStatement{
public:
    explicit ASTAssignment(const shared_ptr<ASTIdentifier>& identifier, const shared_ptr<ASTExpr>& expression);
    ASTAssignment()=default;
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTExpr> expression;
    //void accept(shared_ptr<VisitorNode> v) override;
    ~ASTAssignment();
};

class ASTFunctionDecl: public ASTStatement{};

class ASTFormalParams: public ASTNode{};

class ASTActualParams: public ASTNode{
public:
    ASTActualParams()=default;
    explicit ASTActualParams(const vector<shared_ptr<ASTExpr>>& expressions);
    vector<shared_ptr<ASTExpr>> expressions{};
    ~ASTActualParams();
};

class ASTFormalParam: public ASTFormalParams{};

class ASTExpr: public ASTActualParams{
public:
    ASTExpr()=default;
    explicit ASTExpr(const vector<shared_ptr<ASTSimpleExpr>>& simpleExpressions);
    vector<shared_ptr<ASTSimpleExpr>> simpleExpressions{};
    ~ASTExpr();
};

class ASTSimpleExpr: public ASTExpr{
public:
    ASTSimpleExpr()=default;
    explicit ASTSimpleExpr(const vector<shared_ptr<ASTTerm>>& terms);
    vector<shared_ptr<ASTTerm>> terms{};
    ~ASTSimpleExpr();
};

class ASTTerm: public ASTSimpleExpr{
public:
    ASTTerm()=default;
    explicit ASTTerm(const vector<shared_ptr<ASTFactor>>& factors);
    vector<shared_ptr<ASTFactor>> factors{};
    ~ASTTerm();
};

class ASTFactor: public ASTTerm{
public:
    ASTFactor()=default;
    ~ASTFactor()=default;
};

class ASTUnary: public ASTNode{};

class ASTSubExpr: public ASTNode{};

class ASTFunctionCall: public ASTFactor{
public:
    string value;
    ASTFunctionCall(shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTActualParams> actualParams);
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTActualParams> actualParams;
    ASTFunctionCall()=default;
    ~ASTFunctionCall();
};

class ASTLiteral: public ASTFactor{};

class ASTIntLiteral: public ASTLiteral{
public:
    int value;
    explicit ASTIntLiteral(const string& value);
    ASTIntLiteral()=default;
    ~ASTIntLiteral()=default;
};
class ASTColourLiteral: public ASTLiteral{
public:
    string value;
    explicit ASTColourLiteral(const string& value);
    ASTColourLiteral()=default;
    ~ASTColourLiteral()=default;
};
class ASTFloatLiteral: public ASTLiteral{
public:
    float value;
    explicit ASTFloatLiteral(const string& value);
    ASTFloatLiteral()=default;
    ~ASTFloatLiteral()=default;
};
class ASTBoolLiteral: public ASTLiteral{
public:
    bool value;
    explicit ASTBoolLiteral(const string& value);
    ASTBoolLiteral()=default;
    ~ASTBoolLiteral()=default;
};
class ASTPadWidth: public ASTLiteral{
public:
    string value;
    explicit ASTPadWidth(const string& value);
    ASTPadWidth()=default;
    ~ASTPadWidth()=default;
};
class ASTPadHeight: public ASTLiteral{
public:
    string value;
    explicit ASTPadHeight(const string& value);
    ASTPadHeight()=default;
    ~ASTPadHeight()=default;
};
class ASTPadRead: public ASTLiteral{
public:
    string value;
    explicit ASTPadRead(const string& value);
    ASTPadRead()=default;
    ~ASTPadRead()=default;
};

class ASTPadRandi: public ASTFactor{};

class ASTIdentifier: public ASTFactor{
public:
    explicit ASTIdentifier(string identifier);
    string identifier;
    ASTIdentifier()=default;
    //void accept(shared_ptr<VisitorNode> v) override;
    ~ASTIdentifier()=default;
};





class Parser{
private:
    string validHexCharacters="ABCDEFabcdef0123456789";
    map<string,string> specialCase1={{ "__read", "<__read>" }};
    map<string,string> specialCase2={{ "__randi", "<__randi>" },{ "__print", "<__print>" },{ "__delay", "<__delay>" },{ "__pixel", "<__pixel>" },{ "__width", "<PadWidth>" }};
    map<string,string> specialCase3={{ "__pixelr", "<__pixelr>" },{ "__height", "<PadHeight>" }};
    unique_ptr<Lexer> lexer= make_unique<Lexer>();
    shared_ptr<Token> lookaheadToken1;
    shared_ptr<Token> lookaheadToken2;
    shared_ptr<ASTProgram> program;
    fstream file;
    bool EOFFlag=false;
    void CheckValidToken(const shared_ptr<Token>& token);
public:
    //void LLKParse(fstream &readFilePointer);
    shared_ptr<ASTProgram> ParseProgram(fstream &readFilePointer);
    shared_ptr<ASTStatement> ParseStatement();
    shared_ptr<ASTAssignment> ParseAssignment();
    shared_ptr<ASTIdentifier> ParseIdentifier();
    shared_ptr<ASTExpr> ParseExpression();
    shared_ptr<ASTFactor> ParseFactor();
    shared_ptr<ASTLiteral> ParseLiteral();
    shared_ptr<ASTBlock> ParseBlock();
    shared_ptr<ASTSimpleExpr> ParseSimpleExpression();
    shared_ptr<ASTTerm> ParseTerm();
    shared_ptr<ASTActualParams> ParseActualParams();
    shared_ptr<ASTFunctionCall> ParseFunctionCall();
    void GetNextToken();
    Parser()=default;
    ~Parser();


};
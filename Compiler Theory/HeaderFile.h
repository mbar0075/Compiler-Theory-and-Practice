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
class ASTAssignment;
class ASTVariableDecl;
class ASTFormalParam;
class ASTFormalParams;
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



class ASTBlock: public ASTStatement{
public:
    ASTBlock()=default;
    explicit ASTBlock(const vector<shared_ptr<ASTStatement>>& statements);
    vector<shared_ptr<ASTStatement>> statements;
    ~ASTBlock();
};

class ASTWhileStatement: public ASTStatement{
public:
    ASTWhileStatement()=default;
    explicit ASTWhileStatement(const shared_ptr<ASTExpr>& expression,const shared_ptr<ASTBlock>& block);
    shared_ptr<ASTBlock> block;
    shared_ptr<ASTExpr> expression;
    ~ASTWhileStatement();
};

class ASTForStatement: public ASTStatement{
public:
    ASTForStatement()=default;
    explicit ASTForStatement(const shared_ptr<ASTExpr>& expression,const shared_ptr<ASTAssignment>& assignment,const shared_ptr<ASTVariableDecl>& variableDecl ,const shared_ptr<ASTBlock>& block);
    shared_ptr<ASTExpr> expression;
    shared_ptr<ASTBlock> block;
    shared_ptr<ASTAssignment> assignment;
    shared_ptr<ASTVariableDecl> variableDecl;
    ~ASTForStatement();
};

class ASTIfStatement: public ASTStatement{
public:
    ASTIfStatement()=default;
    explicit ASTIfStatement(const shared_ptr<ASTExpr>& expression,const shared_ptr<ASTBlock>& firstBlock,const shared_ptr<ASTBlock>& secondBlock);
    shared_ptr<ASTBlock> firstBlock;
    shared_ptr<ASTBlock> secondBlock;
    shared_ptr<ASTExpr> expression;
    ~ASTIfStatement();
};

class ASTRtrnStatement: public ASTStatement{
public:
    ASTRtrnStatement()=default;
    explicit ASTRtrnStatement(const shared_ptr<ASTExpr>& expression);
    shared_ptr<ASTExpr> expression;
    ~ASTRtrnStatement();
};

class ASTPixelStatement: public ASTStatement{
public:
    ASTPixelStatement()=default;
    explicit ASTPixelStatement(const string& value,const vector<shared_ptr<ASTExpr>>& expressions);
    string value;
    vector<shared_ptr<ASTExpr>> expressions;
    ~ASTPixelStatement();

};

class ASTDelayStatement: public ASTStatement{
public:
    ASTDelayStatement()=default;
    explicit ASTDelayStatement(const shared_ptr<ASTExpr>& expression);
    shared_ptr<ASTExpr> expression;
    ~ASTDelayStatement();
};

class ASTPrintStatement: public ASTStatement{
public:
    ASTPrintStatement()=default;
    explicit ASTPrintStatement(const shared_ptr<ASTExpr>& expression);
    shared_ptr<ASTExpr> expression;
    ~ASTPrintStatement();
};

class ASTType: public ASTNode{
public:
    ASTType()=default;
    explicit ASTType(const string& value);
    string value;
    ~ASTType()=default;
};

class ASTVariableDecl: public ASTStatement{
public:
    ASTVariableDecl()=default;
    explicit ASTVariableDecl(const shared_ptr<ASTIdentifier>& identifier,const shared_ptr<ASTType>& type,const shared_ptr<ASTExpr>& expression);
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTType> type;
    shared_ptr<ASTExpr> expression;
    ~ASTVariableDecl();
};


class ASTAssignment: public ASTStatement{
public:
    explicit ASTAssignment(const shared_ptr<ASTIdentifier>& identifier, const shared_ptr<ASTExpr>& expression);
    ASTAssignment()=default;
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTExpr> expression;
    //void accept(shared_ptr<VisitorNode> v) override;
    ~ASTAssignment();
};

class ASTFunctionDecl: public ASTStatement{
public:
    ASTFunctionDecl()=default;
    explicit ASTFunctionDecl(const shared_ptr<ASTIdentifier>& identifier,const shared_ptr<ASTFormalParams>& formalParams,const shared_ptr<ASTType>& type ,const shared_ptr<ASTBlock>& block);
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTBlock> block;
    shared_ptr<ASTType> type;
    shared_ptr<ASTFormalParams> formalParams;
    ~ASTFunctionDecl();
};

class ASTFormalParams: public ASTNode{
public:
    ASTFormalParams()=default;
    explicit ASTFormalParams(const vector<shared_ptr<ASTFormalParam>>& formalParams);
    vector<shared_ptr<ASTFormalParam>> formalParams;
    ~ASTFormalParams();
};

class ASTActualParams: public ASTNode{
public:
    ASTActualParams()=default;
    explicit ASTActualParams(const vector<shared_ptr<ASTExpr>>& expressions);
    vector<shared_ptr<ASTExpr>> expressions;
    ~ASTActualParams();
};

class ASTFormalParam: public ASTFormalParams{
public:
    ASTFormalParam()=default;
    explicit ASTFormalParam(const shared_ptr<ASTIdentifier>& identifier,const shared_ptr<ASTType>& type);
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTType> type;
    ~ASTFormalParam();
};

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

class ASTTerm: public ASTExpr{
public:
    ASTTerm()=default;
    explicit ASTTerm(const vector<shared_ptr<ASTFactor>>& factors);
    vector<shared_ptr<ASTFactor>> factors{};
    ~ASTTerm();
};

class ASTFactor: public ASTExpr{
public:
    ASTFactor()=default;
    ~ASTFactor()=default;
};

class ASTUnary: public ASTExpr{
public:
    explicit ASTUnary(const shared_ptr<ASTExpr>& expression, const string& UnaryOperator);
    ASTUnary()=default;
    string UnaryOperator;
    shared_ptr<ASTExpr> expression;
    ~ASTUnary();
};

class ASTSubExpr: public ASTExpr{
public:
    explicit ASTSubExpr(const shared_ptr<ASTExpr>& expression);
    ASTSubExpr()=default;
    shared_ptr<ASTExpr> expression;
    ~ASTSubExpr();
};

class ASTFunctionCall: public ASTExpr{
public:
    string value;
    explicit ASTFunctionCall(const shared_ptr<ASTIdentifier>& identifier,const shared_ptr<ASTActualParams>& actualParams);
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTActualParams> actualParams;
    ASTFunctionCall()=default;
    ~ASTFunctionCall();
};

class ASTLiteral: public ASTExpr{
public:
    ASTLiteral()=default;
    ~ASTLiteral()=default;
};

class ASTIntLiteral: public ASTLiteral{
public:
    int value{};
    explicit ASTIntLiteral(const string& val);
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
    float value{};
    explicit ASTFloatLiteral(const string& val);
    ASTFloatLiteral()=default;
    ~ASTFloatLiteral()=default;
};
class ASTBoolLiteral: public ASTLiteral{
public:
    bool value{};
    explicit ASTBoolLiteral(const string& val);
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
    ASTPadRead()=default;
    shared_ptr<ASTExpr> firstExpression;
    shared_ptr<ASTExpr> secondExpression;
    explicit ASTPadRead(const shared_ptr<ASTExpr>& firstExpression,const shared_ptr<ASTExpr>& secondExpression);
    ~ASTPadRead();
};

class ASTPadRandi: public ASTExpr{
public:
    ASTPadRandi()=default;
    explicit ASTPadRandi(const shared_ptr<ASTExpr>& expression);
    shared_ptr<ASTExpr> expression;
    ~ASTPadRandi();
};

class ASTIdentifier: public ASTExpr{
public:
    explicit ASTIdentifier(const string& identifier);
    string identifier;
    ASTIdentifier()=default;
    //void accept(shared_ptr<VisitorNode> v) override;
    ~ASTIdentifier()=default;
};

class ASTMultiplicativeOp: public ASTExpr{
public:
    ASTMultiplicativeOp()=default;
    explicit ASTMultiplicativeOp(const shared_ptr<ASTExpr>& leftFactor,const string& multiplicativeOp ,const shared_ptr<ASTExpr>& rightExpression);
    shared_ptr<ASTExpr> leftFactor;
    string multiplicativeOp ;
    shared_ptr<ASTExpr> rightExpression;
    ~ASTMultiplicativeOp();
};
class ASTAdditiveOp: public ASTExpr{
public:
    ASTAdditiveOp()=default;
    explicit ASTAdditiveOp(const shared_ptr<ASTExpr>& leftTerm,const string& additiveOp ,const shared_ptr<ASTExpr>& rightTerm);
    shared_ptr<ASTExpr> leftTerm;
    string additiveOp ;
    shared_ptr<ASTExpr> rightTerm;
    ~ASTAdditiveOp();
};
class ASTRelationalOp: public ASTExpr{
public:
    ASTRelationalOp()=default;
    explicit ASTRelationalOp(const shared_ptr<ASTExpr>& leftSimpleExpr,const string& relationalOp ,const shared_ptr<ASTExpr>& rightSimpleExpr);
    shared_ptr<ASTExpr> leftSimpleExpr;
    string relationalOp ;
    shared_ptr<ASTExpr> rightSimpleExpr;
    ~ASTRelationalOp();
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
    shared_ptr<ASTDelayStatement> ParseDelayStatement();
    shared_ptr<ASTRtrnStatement> ParseReturnStatement();
    shared_ptr<ASTPixelStatement> ParsePixelStatement();
    shared_ptr<ASTIfStatement> ParseIfStatement();
    shared_ptr<ASTForStatement> ParseForStatement();
    shared_ptr<ASTWhileStatement> ParseWhileStatement();
    shared_ptr<ASTFormalParam> ParseFormalParam();
    shared_ptr<ASTFormalParams> ParseFormalParams();
    shared_ptr<ASTFunctionDecl> ParseFunctionDecl();

    shared_ptr<ASTType> ParseType();
    void GetNextToken();
    Parser()=default;
    ~Parser();


};
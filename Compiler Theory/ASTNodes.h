//
// Created by Matthias on 06/04/2023.
//
//Inclusion of Libraries
#include <iostream>
#include <fstream>//Utilising to read from File
#include <sstream>//Utilising to read from String
#include <string>//Utilising for strings
#include <memory>//Utilising for smart pointers
#include <map>//Utilising for Dictionaries/Tables
#include <stack>//Utilising for Stack
#include <vector>//Utilising for Dynamic List
#include <utility>//Utilising for shared pointers
#include <windows.h>//Utilising for Colour print
using namespace std;

//Relevant AST Nodes classes with their variables and methods
class VisitorNode;
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

class ASTNode{
public:
    ASTNode()=default;
    virtual void accept(VisitorNode * visitor);
    ~ASTNode()=default;
};
class ASTProgram : public ASTNode{
public:
    ASTProgram()=default;
    explicit ASTProgram( vector<shared_ptr<ASTStatement>> program);
    void accept(VisitorNode * visitor) override;
    vector<shared_ptr<ASTStatement>> program{};
     ~ASTProgram();
};
class ASTStatement: public ASTProgram{
public:
    ASTStatement()=default;
    void accept(VisitorNode * visitor) override;
    ~ASTStatement()=default;
};
class ASTBlock: public ASTStatement{
public:
    ASTBlock()=default;
    explicit ASTBlock( vector<shared_ptr<ASTStatement>> statements);
    void accept(VisitorNode * visitor) override;
    vector<shared_ptr<ASTStatement>> statements;
    ~ASTBlock();
};
class ASTWhileStatement: public ASTStatement{
public:
    ASTWhileStatement()=default;
    ASTWhileStatement( shared_ptr<ASTExpr> expression, shared_ptr<ASTBlock> block);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTBlock> block;
    shared_ptr<ASTExpr> expression;
    ~ASTWhileStatement();
};
class ASTForStatement: public ASTStatement{
public:
    ASTForStatement()=default;
    ASTForStatement( shared_ptr<ASTExpr> expression, shared_ptr<ASTAssignment> assignment, shared_ptr<ASTVariableDecl> variableDecl , shared_ptr<ASTBlock> block);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTExpr> expression;
    shared_ptr<ASTBlock> block;
    shared_ptr<ASTAssignment> assignment;
    shared_ptr<ASTVariableDecl> variableDecl;
    ~ASTForStatement();
};
class ASTIfStatement: public ASTStatement{
public:
    ASTIfStatement()=default;
    ASTIfStatement( shared_ptr<ASTExpr> expression, shared_ptr<ASTBlock> firstBlock, shared_ptr<ASTBlock> secondBlock);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTBlock> firstBlock;
    shared_ptr<ASTBlock> secondBlock;
    shared_ptr<ASTExpr> expression;
    ~ASTIfStatement();
};
class ASTRtrnStatement: public ASTStatement{
public:
    ASTRtrnStatement()=default;
    void accept(VisitorNode * visitor) override;
    explicit ASTRtrnStatement( shared_ptr<ASTExpr> expression);
    shared_ptr<ASTExpr> expression;
    ~ASTRtrnStatement();
};
class ASTPixelStatement: public ASTStatement{
public:
    ASTPixelStatement()=default;
    void accept(VisitorNode * visitor) override;
    explicit ASTPixelStatement( string value, vector<shared_ptr<ASTExpr>> expressions);
    string value;
    vector<shared_ptr<ASTExpr>> expressions;
    ~ASTPixelStatement();

};
class ASTDelayStatement: public ASTStatement{
public:
    ASTDelayStatement()=default;
    void accept(VisitorNode * visitor) override;
    explicit ASTDelayStatement( shared_ptr<ASTExpr> expression);
    shared_ptr<ASTExpr> expression;
    ~ASTDelayStatement();
};
class ASTPrintStatement: public ASTStatement{
public:
    ASTPrintStatement()=default;
    void accept(VisitorNode * visitor) override;
    explicit ASTPrintStatement( shared_ptr<ASTExpr> expression);
    shared_ptr<ASTExpr> expression;
    ~ASTPrintStatement();
};
class ASTType: public ASTNode{
public:
    ASTType()=default;
    explicit ASTType( string value);
    void accept(VisitorNode * visitor) override;
    string value;
    ~ASTType()=default;
};
class ASTVariableDecl: public ASTStatement{
public:
    ASTVariableDecl()=default;
    void accept(VisitorNode * visitor) override;
    ASTVariableDecl( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTType> type, shared_ptr<ASTExpr> expression);
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTType> type;
    shared_ptr<ASTExpr> expression;
    ~ASTVariableDecl();
};
class ASTAssignment: public ASTStatement{
public:
    ASTAssignment( shared_ptr<ASTIdentifier> identifier,  shared_ptr<ASTExpr> expression);
    void accept(VisitorNode * visitor) override;
    ASTAssignment()=default;
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTExpr> expression;
    ~ASTAssignment();
};
class ASTFunctionDecl: public ASTStatement{
public:
    ASTFunctionDecl()=default;
    ASTFunctionDecl( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTFormalParams> formalParams, shared_ptr<ASTType> type , shared_ptr<ASTBlock> block);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTBlock> block;
    shared_ptr<ASTType> type;
    shared_ptr<ASTFormalParams> formalParams;
    ~ASTFunctionDecl();
};
class ASTFormalParams: public ASTStatement{
public:
    ASTFormalParams()=default;
    explicit ASTFormalParams( vector<shared_ptr<ASTFormalParam>> formalParams);
    void accept(VisitorNode * visitor) override;
    vector<shared_ptr<ASTFormalParam>> formalParams;
    ~ASTFormalParams();
};
class ASTActualParams: public ASTNode{
public:
    ASTActualParams()=default;
    explicit ASTActualParams( vector<shared_ptr<ASTExpr>> expressions);
    void accept(VisitorNode * visitor) override;
    vector<shared_ptr<ASTExpr>> expressions;
    ~ASTActualParams();
};

class ASTFormalParam: public ASTFormalParams{
public:
    ASTFormalParam()=default;
    ASTFormalParam( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTType> type);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTType> type;
    ~ASTFormalParam();
};
class ASTExpr: public ASTActualParams{
public:
    ASTExpr()=default;
    explicit ASTExpr( vector<shared_ptr<ASTSimpleExpr>> simpleExpressions);
    void accept(VisitorNode * visitor) override;
    vector<shared_ptr<ASTSimpleExpr>> simpleExpressions{};
    ~ASTExpr();
};
class ASTSimpleExpr: public ASTExpr{
public:
    ASTSimpleExpr()=default;
    explicit ASTSimpleExpr( vector<shared_ptr<ASTTerm>> terms);
    void accept(VisitorNode * visitor) override;
    vector<shared_ptr<ASTTerm>> terms{};
    ~ASTSimpleExpr();
};
class ASTTerm: public ASTSimpleExpr{
public:
    ASTTerm()=default;
    explicit ASTTerm( vector<shared_ptr<ASTFactor>> factors);
    void accept(VisitorNode * visitor) override;
    vector<shared_ptr<ASTFactor>> factors{};
    ~ASTTerm();
};
class ASTFactor: public ASTTerm{
public:
    ASTFactor()=default;
    void accept(VisitorNode * visitor) override;
    ~ASTFactor()=default;
};
class ASTUnary: public ASTExpr{
public:
    ASTUnary( shared_ptr<ASTExpr> expression,  string UnaryOperator);
    ASTUnary()=default;
    void accept(VisitorNode * visitor) override;
    string UnaryOperator;
    shared_ptr<ASTExpr> expression;
    ~ASTUnary();
};
class ASTSubExpr: public ASTExpr{
public:
    explicit ASTSubExpr( shared_ptr<ASTExpr> expression);
    void accept(VisitorNode * visitor) override;
    ASTSubExpr()=default;
    shared_ptr<ASTExpr> expression;
    ~ASTSubExpr();
};
class ASTFunctionCall: public ASTExpr{
public:
    string value;
    ASTFunctionCall( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTActualParams> actualParams);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTIdentifier> identifier;
    shared_ptr<ASTActualParams> actualParams;
    ASTFunctionCall()=default;
    ~ASTFunctionCall();
};
class ASTLiteral: public ASTExpr{
public:
    ASTLiteral()=default;
    void accept(VisitorNode * visitor) override;
    ~ASTLiteral()=default;
};
class ASTIntLiteral: public ASTLiteral{
public:
    int value{};
    explicit ASTIntLiteral( const string& val);
    void accept(VisitorNode * visitor) override;
    ASTIntLiteral()=default;
    ~ASTIntLiteral()=default;
};
class ASTColourLiteral: public ASTLiteral{
public:
    string value;
    explicit ASTColourLiteral( string value);
    void accept(VisitorNode * visitor) override;
    ASTColourLiteral()=default;
    ~ASTColourLiteral()=default;
};
class ASTFloatLiteral: public ASTLiteral{
public:
    float value{};
    explicit ASTFloatLiteral( const string& val);
    void accept(VisitorNode * visitor) override;
    ASTFloatLiteral()=default;
    ~ASTFloatLiteral()=default;
};
class ASTBoolLiteral: public ASTLiteral{
public:
    bool value{};
    explicit ASTBoolLiteral( const string& val);
    void accept(VisitorNode * visitor) override;
    ASTBoolLiteral()=default;
    ~ASTBoolLiteral()=default;
};
class ASTPadWidth: public ASTLiteral{
public:
    string value;
    explicit ASTPadWidth( string value);
    void accept(VisitorNode * visitor) override;
    ASTPadWidth()=default;
    ~ASTPadWidth()=default;
};
class ASTPadHeight: public ASTLiteral{
public:
    string value;
    explicit ASTPadHeight( string value);
    void accept(VisitorNode * visitor) override;
    ASTPadHeight()=default;
    ~ASTPadHeight()=default;
};
class ASTPadRead: public ASTLiteral{
public:
    ASTPadRead()=default;
    shared_ptr<ASTExpr> firstExpression;
    shared_ptr<ASTExpr> secondExpression;
    void accept(VisitorNode * visitor) override;
    ASTPadRead( shared_ptr<ASTExpr> firstExpression, shared_ptr<ASTExpr> secondExpression);
    ~ASTPadRead();
};
class ASTPadRandi: public ASTExpr{
public:
    ASTPadRandi()=default;
    explicit ASTPadRandi( shared_ptr<ASTExpr> expression);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTExpr> expression;
    ~ASTPadRandi();
};
class ASTIdentifier: public ASTExpr{
public:
    explicit ASTIdentifier( string identifier);
    void accept(VisitorNode * visitor) override;
    string identifier;
    ASTIdentifier()=default;
    ~ASTIdentifier()=default;
};
class ASTMultiplicativeOp: public ASTExpr{
public:
    ASTMultiplicativeOp()=default;
    ASTMultiplicativeOp( shared_ptr<ASTExpr> leftFactor, string multiplicativeOp , shared_ptr<ASTExpr> rightFactor);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTExpr> leftFactor;
    string multiplicativeOp ;
    shared_ptr<ASTExpr> rightFactor;
    ~ASTMultiplicativeOp();
};
class ASTAdditiveOp: public ASTExpr{
public:
    ASTAdditiveOp()=default;
    ASTAdditiveOp( shared_ptr<ASTExpr> leftTerm, string additiveOp , shared_ptr<ASTExpr> rightTerm);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTExpr> leftTerm;
    string additiveOp ;
    shared_ptr<ASTExpr> rightTerm;
    ~ASTAdditiveOp();
};
class ASTRelationalOp: public ASTExpr{
public:
    ASTRelationalOp()=default;
    ASTRelationalOp( shared_ptr<ASTExpr> leftSimpleExpr, string relationalOp , shared_ptr<ASTExpr> rightSimpleExpr);
    void accept(VisitorNode * visitor) override;
    shared_ptr<ASTExpr> leftSimpleExpr;
    string relationalOp ;
    shared_ptr<ASTExpr> rightSimpleExpr;
    ~ASTRelationalOp();
};
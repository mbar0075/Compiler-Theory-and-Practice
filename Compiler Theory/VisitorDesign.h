//
// Created by Matthias on 06/04/2023.
//
#include "ASTNodes.h"

class  VisitorNode : public enable_shared_from_this<VisitorNode>{
public:
    int visited=0;
    VisitorNode()=default;
    virtual void visit(shared_ptr<ASTNode> pointer)=0;
    virtual void visit(shared_ptr<ASTProgram> pointer)=0;
    virtual void visit(shared_ptr<ASTStatement>pointer)=0;
    virtual void visit(shared_ptr<ASTPrintStatement>pointer)=0;
    virtual void visit(shared_ptr<ASTDelayStatement>pointer)=0;
    virtual void visit(shared_ptr<ASTForStatement>pointer)=0;
    virtual void visit(shared_ptr<ASTWhileStatement>pointer)=0;
    virtual void visit(shared_ptr<ASTIfStatement>pointer)=0;
    virtual void visit(shared_ptr<ASTVariableDecl>pointer)=0;
    virtual void visit(shared_ptr<ASTAssignment>pointer)=0;
    virtual void visit(shared_ptr<ASTBlock>pointer)=0;
    virtual void visit(shared_ptr<ASTFunctionDecl>pointer)=0;
    virtual void visit(shared_ptr<ASTFunctionCall>pointer)=0;
    virtual void visit(shared_ptr<ASTFormalParams>pointer)=0;
    virtual void visit(shared_ptr<ASTFormalParam>pointer)=0;
    virtual void visit(shared_ptr<ASTType>pointer)=0;
    virtual void visit(shared_ptr<ASTIdentifier>pointer)=0;
    virtual void visit(shared_ptr<ASTMultiplicativeOp>pointer)=0;
    virtual void visit(shared_ptr<ASTAdditiveOp>pointer)=0;
    virtual void visit(shared_ptr<ASTRelationalOp>pointer)=0;
    virtual void visit(shared_ptr<ASTRtrnStatement>pointer)=0;
    virtual void visit(shared_ptr<ASTFactor>pointer)=0;
    virtual void visit(shared_ptr<ASTLiteral>pointer)=0;
    virtual void visit(shared_ptr<ASTIntLiteral>pointer)=0;
    virtual void visit(shared_ptr<ASTFloatLiteral>pointer)=0;
    virtual void visit(shared_ptr<ASTBoolLiteral>pointer)=0;
    virtual void visit(shared_ptr<ASTColourLiteral>pointer)=0;
    virtual void visit(shared_ptr<ASTPadRead>pointer)=0;
    virtual void visit(shared_ptr<ASTPadRandi>pointer)=0;
    virtual void visit(shared_ptr<ASTPadWidth>pointer)=0;
    virtual void visit(shared_ptr<ASTPadHeight>pointer)=0;
    virtual void visit(shared_ptr<ASTTerm>pointer)=0;
    virtual void visit(shared_ptr<ASTExpr>pointer)=0;
    virtual void visit(shared_ptr<ASTSubExpr>pointer)=0;
    virtual void visit(shared_ptr<ASTSimpleExpr>pointer)=0;
    virtual void visit(shared_ptr<ASTUnary>pointer)=0;
    virtual void visit(shared_ptr<ASTActualParams>pointer)=0;
    virtual void visit(shared_ptr<ASTPixelStatement>pointer)=0;
    ~VisitorNode()=default;
};

class XMLVisitorNode : public VisitorNode{
public:
    XMLVisitorNode()=default;
    void printIndent();
    void visit(shared_ptr<ASTNode>pointer)override;
    void visit(shared_ptr<ASTProgram>pointer)override;
    void visit(shared_ptr<ASTStatement>pointer)override;
    void visit(shared_ptr<ASTPrintStatement>pointer)override;
    void visit(shared_ptr<ASTDelayStatement>pointer)override;
    void visit(shared_ptr<ASTForStatement>pointer)override;
    void visit(shared_ptr<ASTWhileStatement>pointer)override;
    void visit(shared_ptr<ASTIfStatement>pointer)override;
    void visit(shared_ptr<ASTVariableDecl>pointer)override;
    void visit(shared_ptr<ASTAssignment>pointer)override;
    void visit(shared_ptr<ASTBlock>pointer)override;
    void visit(shared_ptr<ASTFunctionDecl>pointer)override;
    void visit(shared_ptr<ASTFunctionCall>pointer)override;
    void visit(shared_ptr<ASTFormalParams>pointer)override;
    void visit(shared_ptr<ASTFormalParam>pointer)override;
    void visit(shared_ptr<ASTType>pointer)override;
    void visit(shared_ptr<ASTIdentifier>pointer)override;
    void visit(shared_ptr<ASTMultiplicativeOp>pointer)override;
    void visit(shared_ptr<ASTAdditiveOp>pointer)override;
    void visit(shared_ptr<ASTRelationalOp>pointer)override;
    void visit(shared_ptr<ASTRtrnStatement>pointer)override;
    void visit(shared_ptr<ASTFactor>pointer)override;
    void visit(shared_ptr<ASTLiteral>pointer)override;
    void visit(shared_ptr<ASTIntLiteral>pointer)override;
    void visit(shared_ptr<ASTFloatLiteral>pointer)override;
    void visit(shared_ptr<ASTBoolLiteral>pointer)override;
    void visit(shared_ptr<ASTColourLiteral>pointer)override;
    void visit(shared_ptr<ASTPadRead>pointer)override;
    void visit(shared_ptr<ASTPadRandi>pointer)override;
    void visit(shared_ptr<ASTPadWidth>pointer)override;
    void visit(shared_ptr<ASTPadHeight>pointer)override;
    void visit(shared_ptr<ASTTerm>pointer)override;
    void visit(shared_ptr<ASTExpr>pointer)override;
    void visit(shared_ptr<ASTSubExpr>pointer)override;
    void visit(shared_ptr<ASTSimpleExpr>pointer)override;
    void visit(shared_ptr<ASTUnary>pointer)override;
    void visit(shared_ptr<ASTActualParams>pointer)override;
    void visit(shared_ptr<ASTPixelStatement>pointer)override;
    ~XMLVisitorNode()=default;
};

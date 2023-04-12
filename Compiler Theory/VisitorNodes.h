//
// Created by Matthias on 06/04/2023.
//
//Inclusion of relevant header File
#include "SymbolTable.h"

//Visitor Node class with its relevant variables and methods
class  VisitorNode{
public:
    int tabCounter=0;
    VisitorNode()=default;
    virtual void visit( ASTNode * pointer)=0;
    virtual void visit( ASTProgram * pointer)=0;
    virtual void visit( ASTStatement *pointer)=0;
    virtual void visit( ASTPrintStatement *pointer)=0;
    virtual void visit( ASTDelayStatement *pointer)=0;
    virtual void visit( ASTForStatement *pointer)=0;
    virtual void visit( ASTWhileStatement *pointer)=0;
    virtual void visit( ASTIfStatement *pointer)=0;
    virtual void visit( ASTVariableDecl *pointer)=0;
    virtual void visit( ASTAssignment *pointer)=0;
    virtual void visit( ASTBlock *pointer)=0;
    virtual void visit( ASTFunctionDecl *pointer)=0;
    virtual void visit( ASTFunctionCall *pointer)=0;
    virtual void visit( ASTFormalParams *pointer)=0;
    virtual void visit( ASTFormalParam *pointer)=0;
    virtual void visit( ASTType *pointer)=0;
    virtual void visit( ASTIdentifier *pointer)=0;
    virtual void visit( ASTMultiplicativeOp *pointer)=0;
    virtual void visit( ASTAdditiveOp *pointer)=0;
    virtual void visit( ASTRelationalOp *pointer)=0;
    virtual void visit( ASTRtrnStatement *pointer)=0;
    virtual void visit( ASTFactor *pointer)=0;
    virtual void visit( ASTLiteral *pointer)=0;
    virtual void visit( ASTIntLiteral *pointer)=0;
    virtual void visit( ASTFloatLiteral *pointer)=0;
    virtual void visit( ASTBoolLiteral *pointer)=0;
    virtual void visit( ASTColourLiteral *pointer)=0;
    virtual void visit( ASTPadRead *pointer)=0;
    virtual void visit( ASTPadRandi *pointer)=0;
    virtual void visit( ASTPadWidth *pointer)=0;
    virtual void visit( ASTPadHeight *pointer)=0;
    virtual void visit( ASTTerm *pointer)=0;
    virtual void visit( ASTExpr *pointer)=0;
    virtual void visit( ASTSubExpr *pointer)=0;
    virtual void visit( ASTSimpleExpr *pointer)=0;
    virtual void visit( ASTUnary *pointer)=0;
    virtual void visit( ASTActualParams *pointer)=0;
    virtual void visit( ASTPixelStatement *pointer)=0;
    ~VisitorNode()=default;
};

//XML Visitor Node class with its relevant variables and methods
class XMLVisitorNode : public VisitorNode{
public:
    XMLVisitorNode()=default;
    void printIndent();
    void visit( ASTNode *pointer)override;
    void visit( ASTProgram *pointer)override;
    void visit( ASTStatement *pointer)override;
    void visit( ASTPrintStatement *pointer)override;
    void visit( ASTDelayStatement *pointer)override;
    void visit( ASTForStatement *pointer)override;
    void visit( ASTWhileStatement *pointer)override;
    void visit( ASTIfStatement *pointer)override;
    void visit( ASTVariableDecl *pointer)override;
    void visit( ASTAssignment *pointer)override;
    void visit( ASTBlock *pointer)override;
    void visit( ASTFunctionDecl *pointer)override;
    void visit( ASTFunctionCall *pointer)override;
    void visit( ASTFormalParams *pointer)override;
    void visit( ASTFormalParam *pointer)override;
    void visit( ASTType *pointer)override;
    void visit( ASTIdentifier *pointer)override;
    void visit( ASTMultiplicativeOp *pointer)override;
    void visit( ASTAdditiveOp *pointer)override;
    void visit( ASTRelationalOp *pointer)override;
    void visit( ASTRtrnStatement *pointer)override;
    void visit( ASTFactor *pointer)override;
    void visit( ASTLiteral *pointer)override;
    void visit( ASTIntLiteral *pointer)override;
    void visit( ASTFloatLiteral *pointer)override;
    void visit( ASTBoolLiteral *pointer)override;
    void visit( ASTColourLiteral *pointer)override;
    void visit( ASTPadRead *pointer)override;
    void visit( ASTPadRandi *pointer)override;
    void visit( ASTPadWidth *pointer)override;
    void visit( ASTPadHeight *pointer)override;
    void visit( ASTTerm *pointer)override;
    void visit( ASTExpr *pointer)override;
    void visit( ASTSubExpr *pointer)override;
    void visit( ASTSimpleExpr *pointer)override;
    void visit( ASTUnary *pointer)override;
    void visit( ASTActualParams *pointer)override;
    void visit( ASTPixelStatement *pointer)override;
    ~XMLVisitorNode()=default;
};

class SemanticVisitorNode: public VisitorNode{
public:
    unique_ptr<SymbolTable> symbolTable= make_unique<SymbolTable>();
    string currentStoredType;
    string currentStoredFunctionName;
    bool zeroFlag=false;
    bool assignmentFlag=false;
    bool returnFlag=false;
    void RemoveFunctionParameters(const shared_ptr<ASTFormalParams>& pointer) const;
    SemanticVisitorNode()=default;
    void visit( ASTNode *pointer)override;
    void visit( ASTProgram *pointer)override;
    void visit( ASTStatement *pointer)override;
    void visit( ASTPrintStatement *pointer)override;
    void visit( ASTDelayStatement *pointer)override;
    void visit( ASTForStatement *pointer)override;
    void visit( ASTWhileStatement *pointer)override;
    void visit( ASTIfStatement *pointer)override;
    void visit( ASTVariableDecl *pointer)override;
    void visit( ASTAssignment *pointer)override;
    void visit( ASTBlock *pointer)override;
    void visit( ASTFunctionDecl *pointer)override;
    void visit( ASTFunctionCall *pointer)override;
    void visit( ASTFormalParams *pointer)override;
    void visit( ASTFormalParam *pointer)override;
    void visit( ASTType *pointer)override;
    void visit( ASTIdentifier *pointer)override;
    void visit( ASTMultiplicativeOp *pointer)override;
    void visit( ASTAdditiveOp *pointer)override;
    void visit( ASTRelationalOp *pointer)override;
    void visit( ASTRtrnStatement *pointer)override;
    void visit( ASTFactor *pointer)override;
    void visit( ASTLiteral *pointer)override;
    void visit( ASTIntLiteral *pointer)override;
    void visit( ASTFloatLiteral *pointer)override;
    void visit( ASTBoolLiteral *pointer)override;
    void visit( ASTColourLiteral *pointer)override;
    void visit( ASTPadRead *pointer)override;
    void visit( ASTPadRandi *pointer)override;
    void visit( ASTPadWidth *pointer)override;
    void visit( ASTPadHeight *pointer)override;
    void visit( ASTTerm *pointer)override;
    void visit( ASTExpr *pointer)override;
    void visit( ASTSubExpr *pointer)override;
    void visit( ASTSimpleExpr *pointer)override;
    void visit( ASTUnary *pointer)override;
    void visit( ASTActualParams *pointer)override;
    void visit( ASTPixelStatement *pointer)override;
    ~SemanticVisitorNode();
};
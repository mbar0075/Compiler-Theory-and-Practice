#include "VisitorNodes.h"

void SemanticVisitorNode::visit(ASTNode * pointer){}

void SemanticVisitorNode::visit(ASTProgram * pointer) {
    Scope initialScope;
    symbolTable->push(initialScope);
    for(auto iter = pointer->program.begin(); iter < pointer->program.end(); iter++)
    {
        ((*iter))->accept(this);
    }
    symbolTable->pop();
}
void SemanticVisitorNode::visit(ASTBlock * pointer) {
    Scope blockScope;
    symbolTable->push(blockScope);
    for(auto iter = pointer->statements.begin(); iter < pointer->statements.end(); iter++)
    {
        ((*iter))->accept(this);
    }
    symbolTable->pop();
}
void SemanticVisitorNode::visit(ASTVariableDecl * pointer) {
    Scope poppedScope=symbolTable->pop();
    string identifier=pointer->identifier->identifier;
    string type=pointer->type->value;
    if(!poppedScope.scope[identifier].empty()){
        cerr<<"\nSemantic Error: Identifier already exists:"<<identifier<<endl;
        exit(5);
    }
    poppedScope.scope[identifier]["type"]=type;
//    poppedScope.scope[pointer->identifier->identifier]["expression"]=pointer->expression->;
    symbolTable->push(poppedScope);
}

void SemanticVisitorNode::visit( ASTStatement *pointer){}
void SemanticVisitorNode::visit( ASTPrintStatement *pointer){}
void SemanticVisitorNode::visit( ASTDelayStatement *pointer){}
void SemanticVisitorNode::visit( ASTForStatement *pointer){}
void SemanticVisitorNode::visit( ASTWhileStatement *pointer){}
void SemanticVisitorNode::visit( ASTIfStatement *pointer){}
void SemanticVisitorNode::visit( ASTAssignment *pointer){}
void SemanticVisitorNode::visit( ASTFunctionDecl *pointer){}
void SemanticVisitorNode::visit( ASTFunctionCall *pointer){}
void SemanticVisitorNode::visit( ASTFormalParams *pointer){}
void SemanticVisitorNode::visit( ASTFormalParam *pointer){}
void SemanticVisitorNode::visit( ASTType *pointer){}
void SemanticVisitorNode::visit( ASTIdentifier *pointer){}
void SemanticVisitorNode::visit( ASTMultiplicativeOp *pointer){}
void SemanticVisitorNode::visit( ASTAdditiveOp *pointer){}
void SemanticVisitorNode::visit( ASTRelationalOp *pointer){}
void SemanticVisitorNode::visit( ASTRtrnStatement *pointer){}
void SemanticVisitorNode::visit( ASTFactor *pointer){}
void SemanticVisitorNode::visit( ASTLiteral *pointer){}
void SemanticVisitorNode::visit( ASTIntLiteral *pointer){}
void SemanticVisitorNode::visit( ASTFloatLiteral *pointer){}
void SemanticVisitorNode::visit( ASTBoolLiteral *pointer){}
void SemanticVisitorNode::visit( ASTColourLiteral *pointer){}
void SemanticVisitorNode::visit( ASTPadRead *pointer){}
void SemanticVisitorNode::visit( ASTPadRandi *pointer){}
void SemanticVisitorNode::visit( ASTPadWidth *pointer){}
void SemanticVisitorNode::visit( ASTPadHeight *pointer){}
void SemanticVisitorNode::visit( ASTTerm *pointer){}
void SemanticVisitorNode::visit( ASTExpr *pointer){}
void SemanticVisitorNode::visit( ASTSubExpr *pointer){}
void SemanticVisitorNode::visit( ASTSimpleExpr *pointer){}
void SemanticVisitorNode::visit( ASTUnary *pointer){}
void SemanticVisitorNode::visit( ASTActualParams *pointer){}
void SemanticVisitorNode::visit( ASTPixelStatement *pointer){}


SemanticVisitorNode::~SemanticVisitorNode() {
    symbolTable.reset();
}
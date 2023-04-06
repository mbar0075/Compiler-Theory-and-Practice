#include "HeaderFile.h"

void XMLVisitorNode::printIndent() {
    for(int v=0;v<visited;v++){
        cout<<"  ";
    }
}

void XMLVisitorNode::visit(shared_ptr<ASTNode> pointer) {
    cout<<pointer.get();
//    cout<<"In here node";
//pointer->accept(enable_shared_from_this<>);
}
void XMLVisitorNode::visit(shared_ptr<ASTStatement> pointer) {

//    if (dynamic_cast<ASTAssignment*>(pointer.get())){
//        visit(static_pointer_cast<ASTAssignment>(pointer));
//    }
//    else if (dynamic_cast<ASTVariableDecl*>(pointer.get())){
//        visit(static_pointer_cast<ASTVariableDecl>(pointer));
//    }
}
void XMLVisitorNode::visit(shared_ptr<ASTFactor> pointer) {}
void XMLVisitorNode::visit(shared_ptr<ASTLiteral> pointer) {}

void XMLVisitorNode::visit(shared_ptr<ASTProgram> pointer) {
    for(auto iter = pointer->program.begin(); iter < pointer->program.end(); iter++)
    {
        visit((*iter));
    }
}

void XMLVisitorNode::visit(shared_ptr<ASTVariableDecl> pointer) {
    printIndent();
    cout<<"<Decl>"<<endl;
    visited++;
    visit(pointer->type);
    visit(pointer->identifier);
    cout<<"</Var>"<<endl;
    visit(pointer->expression);
    visited--;
    printIndent();
    cout<<"</Decl>"<<endl;
}

void XMLVisitorNode::visit(shared_ptr<ASTType> pointer) {
    printIndent();
    cout<<"<Var Type= \""<<pointer->value<<"\">";
}

void XMLVisitorNode::visit(shared_ptr<ASTIdentifier> pointer) {
    cout<<"<Id>"<<pointer->identifier<<"</Id>";
}

void XMLVisitorNode::visit(shared_ptr<ASTExpr> pointer) {
    for(auto iter = pointer->simpleExpressions.begin(); iter <pointer->simpleExpressions.end(); iter++)
    {
        visit((*iter));
    }
}

void XMLVisitorNode::visit(shared_ptr<ASTSimpleExpr> pointer) {
    for(auto iter = pointer->terms.begin(); iter <pointer->terms.end(); iter++)
    {
        visit((*iter));
    }
}

void XMLVisitorNode::visit(shared_ptr<ASTTerm> pointer) {
    for(auto iter = pointer->factors.begin(); iter <pointer->factors.end(); iter++)
    {
        visit((*iter));
    }
}

void XMLVisitorNode::visit(shared_ptr<ASTAdditiveOp> pointer) {
    cout<<"<BinExprNode Op=\""<<pointer->additiveOp<<"\">";
    visit(pointer->leftTerm);
    visit(pointer->rightTerm);
    cout<<"</BinExprNode>";
}

void XMLVisitorNode::visit(shared_ptr<ASTRelationalOp> pointer) {
    cout<<"<BinExprNode Op=\""<<pointer->relationalOp<<"\">";
    visit(pointer->leftSimpleExpr);
    visit(pointer->rightSimpleExpr);
    cout<<"</BinExprNode>";
}

void XMLVisitorNode::visit(shared_ptr<ASTMultiplicativeOp> pointer) {
    cout<<"<BinExprNode Op=\""<<pointer->multiplicativeOp<<"\">";
    visit(pointer->leftFactor);
    visit(pointer->rightFactor);
    cout<<"</BinExprNode>";
}

void XMLVisitorNode::visit(shared_ptr<ASTFloatLiteral> pointer) {
    cout<<"<FloatConst>"<<pointer->value<<"</FloatConst>";
}

void XMLVisitorNode::visit(shared_ptr<ASTIntLiteral> pointer) {
    cout<<"<IntConst>"<<pointer->value<<"</IntConst>";
}

void XMLVisitorNode::visit(shared_ptr<ASTBoolLiteral> pointer) {
    cout<<"<BoolConst>"<<pointer->value<<"</BoolConst>";
}

void XMLVisitorNode::visit(shared_ptr<ASTColourLiteral> pointer) {
    cout<<"<ColourConst>"<<pointer->value<<"</ColourConst>";
}

void XMLVisitorNode::visit(shared_ptr<ASTPadRandi> pointer) {
    cout<<"<PadRandi> __randi";
    visit(pointer->expression);
    cout<<"</PadRandi>";
}

void XMLVisitorNode::visit(shared_ptr<ASTPadRead> pointer) {
    cout<<"<PadRead> __read";
    visit(pointer->firstExpression);
    visit(pointer->secondExpression);
    cout<<"</PadRead>";
}

void XMLVisitorNode::visit(shared_ptr<ASTPadWidth> pointer) {
    cout<<"<PadWidth>"<<pointer->value<<"</PadWidth>";
}

void XMLVisitorNode::visit(shared_ptr<ASTPadHeight> pointer) {
    cout<<"<PadHeight>"<<pointer->value<<"</PadHeight>";
}

void XMLVisitorNode::visit(shared_ptr<ASTBlock> pointer) {
    for(auto iter = pointer->statements.begin(); iter < pointer->statements.end(); iter++)
    {
        visit((*iter));
    }
}

void XMLVisitorNode::visit(shared_ptr<ASTPrintStatement> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTDelayStatement> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTForStatement> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTWhileStatement> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTIfStatement> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTAssignment> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTFunctionCall> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTFunctionDecl> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTFormalParams> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTFormalParam> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTRtrnStatement> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTSubExpr> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTUnary> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTActualParams> pointer) {
}

void XMLVisitorNode::visit(shared_ptr<ASTPixelStatement> pointer) {
}
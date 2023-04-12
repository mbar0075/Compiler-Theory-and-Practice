//Inclusion of relevant header File
#include "VisitorNodes.h"

//Method which prints relevant indent based on tabCounter
void XMLVisitorNode::printIndent() {
    for(int v=0;v<tabCounter;v++){
        cout<<"  ";
    }
}

//visit methods for the relevant AST Nodes passed as parameters
void XMLVisitorNode::visit(ASTNode * pointer){}
void XMLVisitorNode::visit(ASTStatement * pointer) {}
void XMLVisitorNode::visit(ASTFactor * pointer) {}
void XMLVisitorNode::visit(ASTLiteral * pointer) {}
void XMLVisitorNode::visit(ASTProgram * pointer) {
    for(auto iter = pointer->program.begin(); iter < pointer->program.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}
void XMLVisitorNode::visit(ASTVariableDecl * pointer) {
    printIndent();
    cout<<"<Decl>"<<endl;
    tabCounter++;
    pointer->type->accept(this);
    tabCounter++;
    pointer->identifier->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</Var>"<<endl;
    pointer->expression->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</Decl>"<<endl;
}
void XMLVisitorNode::visit(ASTType * pointer) {
    printIndent();
    cout<<"<Var Type= \""<<pointer->value<<"\">"<<endl;
}
void XMLVisitorNode::visit(ASTIdentifier * pointer) {
    printIndent();
    cout<<"<Id>"<<pointer->identifier<<"</Id>"<<endl;
}
void XMLVisitorNode::visit(ASTExpr * pointer) {
    for(auto iter = pointer->simpleExpressions.begin(); iter <pointer->simpleExpressions.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}
void XMLVisitorNode::visit(ASTSimpleExpr * pointer) {
    for(auto iter = pointer->terms.begin(); iter <pointer->terms.end(); iter++)
    {
        ((*iter))->accept(this);
    }

}
void XMLVisitorNode::visit(ASTTerm * pointer) {
    for(auto iter = pointer->factors.begin(); iter <pointer->factors.end(); iter++)
    {
        ((*iter))->accept(this);
    }
}
void XMLVisitorNode::visit(ASTAdditiveOp * pointer) {
    printIndent();
    cout<<"<BinExprNode Op=\""<<pointer->additiveOp<<"\">"<<endl;
    tabCounter++;
    pointer->leftTerm->accept(this);
    pointer->rightTerm->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</BinExprNode>"<<endl;
}
void XMLVisitorNode::visit(ASTRelationalOp * pointer) {
    printIndent();
    cout<<"<BinExprNode Op=\""<<pointer->relationalOp<<"\">"<<endl;
    tabCounter++;
    pointer->leftSimpleExpr->accept(this);
    pointer->rightSimpleExpr->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</BinExprNode>"<<endl;
}
void XMLVisitorNode::visit(ASTMultiplicativeOp * pointer) {
    printIndent();
    cout<<"<BinExprNode Op=\""<<pointer->multiplicativeOp<<"\">"<<endl;
    tabCounter++;
    pointer->leftFactor->accept(this);
    pointer->rightFactor->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</BinExprNode>"<<endl;
}
void XMLVisitorNode::visit(ASTFloatLiteral * pointer) {
    printIndent();
    cout<<"<FloatConst>"<<pointer->value<<"</FloatConst>"<<endl;
}
void XMLVisitorNode::visit(ASTIntLiteral * pointer) {
    printIndent();
    cout<<"<IntConst>"<<pointer->value<<"</IntConst>"<<endl;
}
void XMLVisitorNode::visit(ASTBoolLiteral * pointer) {
    printIndent();
    string boolOutput = pointer->value ? "true" : "false";
    cout<<"<BoolConst>"<<boolOutput<<"</BoolConst>"<<endl;
}
void XMLVisitorNode::visit(ASTColourLiteral * pointer) {
    printIndent();
    cout<<"<ColourConst>"<<pointer->value<<"</ColourConst>"<<endl;
}
void XMLVisitorNode::visit(ASTPadRandi * pointer) {
    printIndent();
    cout<<"<PadRandi Type=\"<__randi>\">"<<endl;
    tabCounter++;
    pointer->expression->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</PadRandi>"<<endl;
}
void XMLVisitorNode::visit(ASTPadRead * pointer) {
    printIndent();
    cout<<"<PadRead Type=\"<__read>\">"<<endl;
    tabCounter++;
    pointer->firstExpression->accept(this);
    pointer->secondExpression->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</PadRead>"<<endl;
}
void XMLVisitorNode::visit(ASTPadWidth * pointer) {
    printIndent();
    cout<<"<PadWidth>"<<pointer->value<<"</PadWidth>"<<endl;
}
void XMLVisitorNode::visit(ASTPadHeight * pointer) {
    printIndent();
    cout<<"<PadHeight>"<<pointer->value<<"</PadHeight>"<<endl;
}
void XMLVisitorNode::visit(ASTBlock * pointer) {
    printIndent();
    cout<<"<Block>"<<endl;
    tabCounter++;
    for(auto iter = pointer->statements.begin(); iter < pointer->statements.end(); iter++)
    {
        ((*iter))->accept(this);
    }
    tabCounter--;
    printIndent();
    cout<<"</Block>"<<endl;
}
void XMLVisitorNode::visit(ASTPrintStatement * pointer) {
    printIndent();
    cout<<"<Print>"<<endl;
    tabCounter++;
    pointer->expression->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</Print>"<<endl;
}
void XMLVisitorNode::visit(ASTDelayStatement * pointer) {
    printIndent();
    cout<<"<Delay>"<<endl;
    tabCounter++;
    pointer->expression->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</Delay>"<<endl;
}
void XMLVisitorNode::visit(ASTForStatement * pointer) {
    printIndent();
    cout<<"<For>"<<endl;
    tabCounter++;
    if(pointer->variableDecl!= nullptr){
        pointer->variableDecl->accept(this);
    }
    pointer->expression->accept(this);
    if(pointer->assignment!= nullptr) {
        pointer->assignment->accept(this);
    }
    pointer->block->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</For>"<<endl;
}
void XMLVisitorNode::visit(ASTWhileStatement * pointer) {
    printIndent();
    cout<<"<While>"<<endl;
    tabCounter++;
    pointer->expression->accept(this);
    pointer->block->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</While>"<<endl;
}
void XMLVisitorNode::visit(ASTIfStatement * pointer) {
    printIndent();
    cout<<"<If>"<<endl;
    tabCounter++;
    pointer->expression->accept(this);
    pointer->firstBlock->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</If>"<<endl;

    if(pointer->secondBlock!= nullptr){
        printIndent();
        cout<<"<Else>"<<endl;
        tabCounter++;
        pointer->secondBlock->accept(this);
        tabCounter--;
        printIndent();
        cout<<"</Else>"<<endl;
    }
}
void XMLVisitorNode::visit(ASTAssignment * pointer) {
    printIndent();
    cout<<"<Assign>"<<endl;
    tabCounter++;
    pointer->identifier->accept(this);
    pointer->expression->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</Assign>"<<endl;
}
void XMLVisitorNode::visit(ASTFunctionCall * pointer) {
    printIndent();
    cout<<"<FunCall>"<<endl;
    tabCounter++;
    pointer->identifier->accept(this);
    if(pointer->actualParams!= nullptr){
        pointer->actualParams->accept(this);
    }
    tabCounter--;
    printIndent();
    cout<<"</FunCall>"<<endl;
}
void XMLVisitorNode::visit(ASTFunctionDecl * pointer) {
    printIndent();
    cout<<"<FunDecl>"<<endl;
    tabCounter++;
    pointer->identifier->accept(this);
    if(pointer->formalParams!= nullptr){
        pointer->formalParams->accept(this);
    }
    pointer->type->accept(this);
    printIndent();
    cout<<"</Var>"<<endl;
    pointer->block->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</FunDecl>"<<endl;
}
void XMLVisitorNode::visit(ASTFormalParams * pointer) {
    printIndent();
    cout<<"<FormalParams>"<<endl;
    tabCounter++;
    for(auto iter = pointer->formalParams.begin(); iter < pointer->formalParams.end(); iter++)
    {
        ((*iter))->accept(this);
    }
    tabCounter--;
    printIndent();
    cout<<"</FormalParams>"<<endl;
}
void XMLVisitorNode::visit(ASTFormalParam * pointer) {
    printIndent();
    cout<<"<FormalParam>"<<endl;
    tabCounter++;
    pointer->identifier->accept(this);
    pointer->type->accept(this);
    printIndent();
    cout<<"</Var>"<<endl;
    tabCounter--;
    printIndent();
    cout<<"</FormalParam>"<<endl;
}
void XMLVisitorNode::visit(ASTRtrnStatement * pointer) {
    printIndent();
    cout<<"<Return>"<<endl;
    tabCounter++;
    pointer->expression->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</Return>"<<endl;
}
void XMLVisitorNode::visit(ASTSubExpr * pointer) {
    printIndent();
    cout<<"<SubExpr>"<<endl;
    tabCounter++;
    pointer->expression->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</SubExpr>"<<endl;
}
void XMLVisitorNode::visit(ASTUnary * pointer) {
    printIndent();
    cout<<"<Unary Op=\""<<pointer->UnaryOperator<<"\">"<<endl;
    tabCounter++;
    pointer->expression->accept(this);
    tabCounter--;
    printIndent();
    cout<<"</Unary>"<<endl;
}
void XMLVisitorNode::visit(ASTActualParams * pointer) {
    printIndent();
    cout<<"<ActualParams>"<<endl;
    tabCounter++;
    for(auto iter = pointer->expressions.begin(); iter < pointer->expressions.end(); iter++)
    {
        ((*iter))->accept(this);
    }
    tabCounter--;
    printIndent();
    cout<<"</ActualParams>"<<endl;
}
void XMLVisitorNode::visit(ASTPixelStatement * pointer) {
    printIndent();
    cout<<"<Pixel Type=\""<<pointer->value<<"\">"<<endl;
    tabCounter++;
    for(auto iter = pointer->expressions.begin(); iter < pointer->expressions.end(); iter++)
    {
        ((*iter))->accept(this);
    }
    tabCounter--;
    printIndent();
    cout<<"</Pixel>"<<endl;
}
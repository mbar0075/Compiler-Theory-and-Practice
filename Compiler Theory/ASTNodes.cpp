//Inclusion of relevant header File
#include <utility>

#include "HeaderFile.h"

//Constructors for the relevant AST Nodes
ASTProgram::ASTProgram( vector<shared_ptr<ASTStatement>> program) {
    this->program=move(program);
}
ASTIdentifier::ASTIdentifier( string identifier) {
    this->identifier=move(identifier);
}
ASTAssignment::ASTAssignment( shared_ptr<ASTIdentifier> identifier,  shared_ptr<ASTExpr> expression) {
    this->identifier=move(identifier);
    this->expression=move(expression);
}
ASTIntLiteral::ASTIntLiteral( const string& val) {
    this->value=stoi(val);
}
ASTColourLiteral::ASTColourLiteral( string value) {
    this->value=move(move(value));
}
ASTFloatLiteral::ASTFloatLiteral( const string& val) {
    this->value =stof(val);
}
ASTBoolLiteral::ASTBoolLiteral( const string& val) {
    if(val=="true"){
        this->value=true;
    }
    else if(val=="false"){
        this->value=false;
    }
}
ASTPadHeight::ASTPadHeight( string value) {
    this->value=move(value);
}
ASTPadRead::ASTPadRead( shared_ptr<ASTExpr> firstExpression,  shared_ptr<ASTExpr> secondExpression) {
    this->firstExpression=move(firstExpression);
    this->secondExpression=move(secondExpression);
}
ASTPadWidth::ASTPadWidth( string value) {
    this->value=move(value);
}
ASTFunctionCall::ASTFunctionCall( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTActualParams> actualParams) {
    this->identifier=move(identifier);
    this->actualParams=move(actualParams);
}
ASTActualParams::ASTActualParams( vector<shared_ptr<ASTExpr>> expressions) {
    this->expressions=move(expressions);
}
ASTExpr::ASTExpr( vector<shared_ptr<ASTSimpleExpr>> simpleExpressions) {
    this->simpleExpressions=move(simpleExpressions);
}
ASTSimpleExpr::ASTSimpleExpr( vector<shared_ptr<ASTTerm>> terms) {
    this->terms=move(terms);
}
ASTTerm::ASTTerm( vector<shared_ptr<ASTFactor>> factors) {
    this->factors=move(factors);
}
ASTMultiplicativeOp::ASTMultiplicativeOp( shared_ptr<ASTExpr> leftFactor, string multiplicativeOp , shared_ptr<ASTExpr> rightFactor){
    this->leftFactor = move(leftFactor);
    this->multiplicativeOp = move(multiplicativeOp);
    this->rightFactor = move(rightFactor);
}
ASTRelationalOp::ASTRelationalOp( shared_ptr<ASTExpr> leftSimpleExpr, string relationalOp , shared_ptr<ASTExpr> rightSimpleExpr){
    this->leftSimpleExpr = move(leftSimpleExpr);
    this->relationalOp = move(relationalOp);
    this->rightSimpleExpr = move(rightSimpleExpr);
}
ASTAdditiveOp::ASTAdditiveOp( shared_ptr<ASTExpr> leftTerm, string additiveOp , shared_ptr<ASTExpr> rightTerm){
    this->leftTerm = move(leftTerm);
    this->additiveOp = move(additiveOp);
    this->rightTerm = move(rightTerm);
}
ASTVariableDecl::ASTVariableDecl( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTType> type, shared_ptr<ASTExpr> expression) {
    this->identifier=move(identifier);
    this->type=move(type);
    this->expression=move(expression);
}
ASTType::ASTType( string value) {
    this->value=move(value);
}
ASTSubExpr::ASTSubExpr( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}
ASTUnary::ASTUnary( shared_ptr<ASTExpr> expression,  string UnaryOperator) {
    this->expression=move(expression);
    this->UnaryOperator=move(UnaryOperator);
}
ASTPadRandi::ASTPadRandi( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}
ASTBlock::ASTBlock( vector<shared_ptr<ASTStatement>> statements) {
    this->statements=move(statements);
}
ASTPrintStatement::ASTPrintStatement( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}
ASTDelayStatement::ASTDelayStatement( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}
ASTRtrnStatement::ASTRtrnStatement( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}
ASTPixelStatement::ASTPixelStatement( string value, vector<shared_ptr<ASTExpr>> expressions) {
    this->value=move(value);
    this->expressions=move(expressions);
}
ASTIfStatement::ASTIfStatement( shared_ptr<ASTExpr> expression,  shared_ptr<ASTBlock> firstBlock, shared_ptr<ASTBlock> secondBlock) {
    this->expression=move(expression);
    this->firstBlock=move(firstBlock);
    this->secondBlock=move(secondBlock);
}
ASTForStatement::ASTForStatement( shared_ptr<ASTExpr> expression,  shared_ptr<ASTAssignment> assignment, shared_ptr<ASTVariableDecl> variableDecl,  shared_ptr<ASTBlock> block) {
    this->expression=move(expression);
    this->assignment=move(assignment);
    this->variableDecl=move(variableDecl);
    this->block=move(block);
}
ASTWhileStatement::ASTWhileStatement( shared_ptr<ASTExpr> expression,  shared_ptr<ASTBlock> block) {
    this->expression=move(expression);
    this->block=move(block);
}
ASTFormalParam::ASTFormalParam( shared_ptr<ASTIdentifier> identifier,  shared_ptr<ASTType> type) {
    this->identifier=move(identifier);
    this->type=move(type);
}
ASTFormalParams::ASTFormalParams( vector<shared_ptr<ASTFormalParam>> formalParams) {
    this->formalParams=move(formalParams);
}
ASTFunctionDecl::ASTFunctionDecl( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTFormalParams> formalParams,  shared_ptr<ASTType> type, shared_ptr<ASTBlock> block) {
    this->identifier=move(identifier);
    this->formalParams=move(formalParams);
    this->type=move(type);
    this->block=move(block);
}
ASTClearStatement::ASTClearStatement(string value) {
    this->value=std::move(value);
}

//Destructors for the relevant AST Nodes
ASTProgram::~ASTProgram() {
    program.clear();
}
ASTAssignment::~ASTAssignment() {
    identifier.reset();
    expression.reset();
}
ASTPadRead::~ASTPadRead() {
    firstExpression.reset();
    secondExpression.reset();
}
ASTFunctionCall::~ASTFunctionCall() {
    identifier.reset();
    actualParams.reset();
}
ASTActualParams::~ASTActualParams(){
    expressions.clear();
}
ASTExpr::~ASTExpr() {
    simpleExpressions.clear();
}
ASTSimpleExpr::~ASTSimpleExpr(){
    this->terms.clear();
}
ASTTerm::~ASTTerm(){
    factors.clear();
}
ASTMultiplicativeOp::~ASTMultiplicativeOp(){
    leftFactor.reset();
    rightFactor.reset();
}
ASTAdditiveOp::~ASTAdditiveOp(){
    leftTerm.reset();
    rightTerm.reset();
}
ASTRelationalOp::~ASTRelationalOp(){
    leftSimpleExpr.reset();
    rightSimpleExpr.reset();
}
ASTVariableDecl::~ASTVariableDecl() {
    identifier.reset();
    type.reset();
    expression.reset();
}
ASTSubExpr::~ASTSubExpr() {
    expression.reset();
}
ASTUnary::~ASTUnary() {
    expression.reset();
}
ASTPadRandi::~ASTPadRandi() {
    expression.reset();
}
ASTBlock::~ASTBlock() {
    statements.clear();
}
ASTPrintStatement::~ASTPrintStatement() {
    expression.reset();
}
ASTDelayStatement::~ASTDelayStatement() {
    expression.reset();
}
ASTRtrnStatement::~ASTRtrnStatement() {
    expression.reset();
}
ASTPixelStatement::~ASTPixelStatement() {
    expressions.clear();
}
ASTIfStatement::~ASTIfStatement() {
    expression.reset();
    firstBlock.reset();
    secondBlock.reset();
}
ASTForStatement::~ASTForStatement() {
    expression.reset();
    assignment.reset();
    variableDecl.reset();
    block.reset();
}
ASTWhileStatement::~ASTWhileStatement() {
    expression.reset();
    block.reset();
}
ASTFormalParam::~ASTFormalParam() {
    identifier.reset();
    type.reset();
}
ASTFormalParams::~ASTFormalParams(){
    formalParams.clear();
}
ASTFunctionDecl::~ASTFunctionDecl() {
    identifier.reset();
    formalParams.reset();
    type.reset();
    block.reset();
}

//accept method for the relevant AST Nodes
void ASTNode::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTProgram::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTStatement::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTType::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTBlock::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTExpr::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTIdentifier::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTActualParams::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTVariableDecl::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTFormalParam::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTFormalParams::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTSimpleExpr::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTTerm::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTDelayStatement::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTAssignment::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTAdditiveOp::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTSubExpr::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTFunctionCall::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTFunctionDecl::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTPadRandi::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTPrintStatement::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTPadRead::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTPadWidth::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTForStatement::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTWhileStatement::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTIfStatement::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTUnary::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTLiteral::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTColourLiteral::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTFloatLiteral::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTIntLiteral::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTBoolLiteral::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTPixelStatement::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTFactor::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTPadHeight::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTRtrnStatement::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTRelationalOp::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTMultiplicativeOp::accept(VisitorNode * visitor) {
    visitor->visit(this);
}
void ASTClearStatement::accept(VisitorNode *visitor) {
    visitor->visit(this);
}
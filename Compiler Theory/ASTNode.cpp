#include "HeaderFile.h"


ASTProgram::ASTProgram( vector<shared_ptr<ASTStatement>> program) {
    this->program=move(program);
}

ASTProgram::~ASTProgram() {
    program.clear();
}

ASTIdentifier::ASTIdentifier( string identifier) {
    this->identifier=move(identifier);
}

ASTAssignment::ASTAssignment( shared_ptr<ASTIdentifier> identifier,  shared_ptr<ASTExpr> expression) {
    this->identifier=move(identifier);
    this->expression=move(expression);
}

ASTAssignment::~ASTAssignment() {
    identifier.reset();
    expression.reset();
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

ASTPadRead::~ASTPadRead() {
    firstExpression.reset();
    secondExpression.reset();
}

ASTPadWidth::ASTPadWidth( string value) {
    this->value=move(value);
}

ASTFunctionCall::ASTFunctionCall( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTActualParams> actualParams) {
    this->identifier=move(identifier);
    this->actualParams=move(actualParams);
}

ASTFunctionCall::~ASTFunctionCall() {
    identifier.reset();
    actualParams.reset();
}

ASTActualParams::ASTActualParams( vector<shared_ptr<ASTExpr>> expressions) {
    this->expressions=move(expressions);
}

ASTActualParams::~ASTActualParams(){
    expressions.clear();
}

ASTExpr::ASTExpr( vector<shared_ptr<ASTSimpleExpr>> simpleExpressions) {
    this->simpleExpressions=move(simpleExpressions);
}

ASTExpr::~ASTExpr() {
    simpleExpressions.clear();
}

ASTSimpleExpr::ASTSimpleExpr( vector<shared_ptr<ASTTerm>> terms) {
    this->terms=move(terms);
}

ASTSimpleExpr::~ASTSimpleExpr(){
    this->terms.clear();
}

ASTTerm::ASTTerm( vector<shared_ptr<ASTFactor>> factors) {
    this->factors=move(factors);
}

ASTTerm::~ASTTerm(){
    factors.clear();
}

ASTMultiplicativeOp::ASTMultiplicativeOp( shared_ptr<ASTExpr> leftFactor, string multiplicativeOp , shared_ptr<ASTExpr> rightFactor){
    this->leftFactor = move(leftFactor);
    this->multiplicativeOp = move(multiplicativeOp);
    this->rightFactor = move(rightFactor);
}

ASTMultiplicativeOp::~ASTMultiplicativeOp(){
    leftFactor.reset();
    rightFactor.reset();
}

ASTAdditiveOp::ASTAdditiveOp( shared_ptr<ASTExpr> leftTerm, string additiveOp , shared_ptr<ASTExpr> rightTerm){
    this->leftTerm = move(leftTerm);
    this->additiveOp = move(additiveOp);
    this->rightTerm = move(rightTerm);
}

ASTAdditiveOp::~ASTAdditiveOp(){
    leftTerm.reset();
    rightTerm.reset();
}

ASTRelationalOp::ASTRelationalOp( shared_ptr<ASTExpr> leftSimpleExpr, string relationalOp , shared_ptr<ASTExpr> rightSimpleExpr){
    this->leftSimpleExpr = move(leftSimpleExpr);
    this->relationalOp = move(relationalOp);
    this->rightSimpleExpr = move(rightSimpleExpr);
}

ASTRelationalOp::~ASTRelationalOp(){
    leftSimpleExpr.reset();
    rightSimpleExpr.reset();
}

ASTVariableDecl::ASTVariableDecl( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTType> type, shared_ptr<ASTExpr> expression) {
    this->identifier=move(identifier);
    this->type=move(type);
    this->expression=move(expression);
}

ASTVariableDecl::~ASTVariableDecl() {
    identifier.reset();
    type.reset();
    expression.reset();
}

ASTType::ASTType( string value) {
    this->value=move(value);
}

ASTSubExpr::ASTSubExpr( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}

ASTSubExpr::~ASTSubExpr() {
    expression.reset();
}

ASTUnary::ASTUnary( shared_ptr<ASTExpr> expression,  string UnaryOperator) {
    this->expression=move(expression);
    this->UnaryOperator=move(UnaryOperator);
}

ASTUnary::~ASTUnary() {
    expression.reset();
}

ASTPadRandi::ASTPadRandi( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}

ASTPadRandi::~ASTPadRandi() {
    expression.reset();
}

ASTBlock::ASTBlock( vector<shared_ptr<ASTStatement>> statements) {
    this->statements=move(statements);
}

ASTBlock::~ASTBlock() {
    statements.clear();
}

ASTPrintStatement::ASTPrintStatement( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}

ASTPrintStatement::~ASTPrintStatement() {
    expression.reset();
}

ASTDelayStatement::ASTDelayStatement( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}

ASTDelayStatement::~ASTDelayStatement() {
    expression.reset();
}

ASTRtrnStatement::ASTRtrnStatement( shared_ptr<ASTExpr> expression) {
    this->expression=move(expression);
}

ASTRtrnStatement::~ASTRtrnStatement() {
    expression.reset();
}

ASTPixelStatement::ASTPixelStatement( string value, vector<shared_ptr<ASTExpr>> expressions) {
    this->value=move(value);
    this->expressions=move(expressions);
}

ASTPixelStatement::~ASTPixelStatement() {
    expressions.clear();
}

ASTIfStatement::ASTIfStatement( shared_ptr<ASTExpr> expression,  shared_ptr<ASTBlock> firstBlock, shared_ptr<ASTBlock> secondBlock) {
    this->expression=move(expression);
    this->firstBlock=move(firstBlock);
    this->secondBlock=move(secondBlock);
}

ASTIfStatement::~ASTIfStatement() {
    expression.reset();
    firstBlock.reset();
    secondBlock.reset();
}

ASTForStatement::ASTForStatement( shared_ptr<ASTExpr> expression,  shared_ptr<ASTAssignment> assignment, shared_ptr<ASTVariableDecl> variableDecl,  shared_ptr<ASTBlock> block) {
    this->expression=move(expression);
    this->assignment=move(assignment);
    this->variableDecl=move(variableDecl);
    this->block=move(block);
}

ASTForStatement::~ASTForStatement() {
    expression.reset();
    assignment.reset();
    variableDecl.reset();
    block.reset();
}

ASTWhileStatement::ASTWhileStatement( shared_ptr<ASTExpr> expression,  shared_ptr<ASTBlock> block) {
    this->expression=move(expression);
    this->block=move(block);
}

ASTWhileStatement::~ASTWhileStatement() {
    expression.reset();
    block.reset();
}

ASTFormalParam::ASTFormalParam( shared_ptr<ASTIdentifier> identifier,  shared_ptr<ASTType> type) {
    this->identifier=move(identifier);
    this->type=move(type);
}

ASTFormalParam::~ASTFormalParam() {
    identifier.reset();
    type.reset();
}

ASTFormalParams::ASTFormalParams( vector<shared_ptr<ASTFormalParam>> formalParams) {
    this->formalParams=move(formalParams);
}

ASTFormalParams::~ASTFormalParams(){
    formalParams.clear();
}

ASTFunctionDecl::ASTFunctionDecl( shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTFormalParams> formalParams,  shared_ptr<ASTType> type, shared_ptr<ASTBlock> block) {
    this->identifier=move(identifier);
    this->formalParams=move(formalParams);
    this->type=move(type);
    this->block=move(block);
}

ASTFunctionDecl::~ASTFunctionDecl() {
    identifier.reset();
    formalParams.reset();
    type.reset();
    block.reset();
}

void ASTNode::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTProgram::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTStatement::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTBlock::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTExpr::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTIdentifier::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTActualParams::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTVariableDecl::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
    cout<<this;
}

void ASTFormalParam::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTFormalParams::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTSimpleExpr::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTTerm::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTDelayStatement::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTAssignment::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTAdditiveOp::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTSubExpr::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTFunctionCall::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTFunctionDecl::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTPadRandi::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTPrintStatement::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTPadRead::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTPadWidth::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTForStatement::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTWhileStatement::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTIfStatement::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTUnary::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTLiteral::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTColourLiteral::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTFloatLiteral::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTIntLiteral::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTBoolLiteral::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTPixelStatement::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTFactor::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTPadHeight::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTRtrnStatement::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTRelationalOp::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}

void ASTMultiplicativeOp::accept(shared_ptr<VisitorNode> visitor) {
    visitor->visit(shared_from_this());
}
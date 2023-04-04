#include "HeaderFile.h"


ASTProgram::ASTProgram(const vector<shared_ptr<ASTStatement>>& program) {
    this->program=program;
}

ASTProgram::~ASTProgram() {
    program.clear();
}

ASTIdentifier::ASTIdentifier(const string& identifier) {
    this->identifier=identifier;
}

ASTAssignment::ASTAssignment(const shared_ptr<ASTIdentifier>& identifier, const shared_ptr<ASTExpr>& expression) {
    this->identifier=identifier;
    this->expression=expression;
}

ASTAssignment::~ASTAssignment() {
    identifier.reset();
    expression.reset();
}

ASTIntLiteral::ASTIntLiteral(const string& val) {
    this->value=stoi(val);
}

ASTColourLiteral::ASTColourLiteral(const string &value) {
    this->value=value;
}

ASTFloatLiteral::ASTFloatLiteral(const string &val) {
    this->value =stof(val);
}

ASTBoolLiteral::ASTBoolLiteral(const string &val) {
    if(val=="true"){
        this->value=true;
    }
    else if(val=="false"){
        this->value=false;
    }
}

ASTPadHeight::ASTPadHeight(const string &value) {
    this->value=value;
}

ASTPadRead::ASTPadRead(const shared_ptr<ASTExpr> &firstExpression, const shared_ptr<ASTExpr> &secondExpression) {
    this->firstExpression=firstExpression;
    this->secondExpression=secondExpression;
}

ASTPadRead::~ASTPadRead() {
    firstExpression.reset();
    secondExpression.reset();
}

ASTPadWidth::ASTPadWidth(const string &value) {
    this->value=value;
}

ASTFunctionCall::ASTFunctionCall(const shared_ptr<ASTIdentifier>& identifier,const shared_ptr<ASTActualParams>& actualParams) {
    this->identifier=identifier;
    this->actualParams=actualParams;
}

ASTFunctionCall::~ASTFunctionCall() {
    identifier.reset();
    actualParams.reset();
}

ASTActualParams::ASTActualParams(const vector<shared_ptr<ASTExpr>> &expressions) {
    this->expressions=expressions;
}

ASTActualParams::~ASTActualParams(){
    expressions.clear();
}

ASTExpr::ASTExpr(const vector<shared_ptr<ASTSimpleExpr>> &simpleExpressions) {
    this->simpleExpressions=simpleExpressions;
}

ASTExpr::~ASTExpr() {
    simpleExpressions.clear();
}

ASTSimpleExpr::ASTSimpleExpr(const vector<shared_ptr<ASTTerm>> &terms) {
    this->terms=terms;
}

ASTSimpleExpr::~ASTSimpleExpr(){
    this->terms.clear();
}

ASTTerm::ASTTerm(const vector<shared_ptr<ASTFactor>> &factors) {
    this->factors=factors;
}

ASTTerm::~ASTTerm(){
    factors.clear();
}

ASTMultiplicativeOp::ASTMultiplicativeOp(const shared_ptr<ASTExpr>& leftFactor,const string& multiplicativeOp ,const shared_ptr<ASTExpr>& rightExpression){
    this->leftFactor = leftFactor;
    this->multiplicativeOp = multiplicativeOp;
    this->rightExpression = rightExpression;
}

ASTMultiplicativeOp::~ASTMultiplicativeOp(){
    leftFactor.reset();
    rightExpression.reset();
}

ASTAdditiveOp::ASTAdditiveOp(const shared_ptr<ASTExpr>& leftTerm,const string& additiveOp ,const shared_ptr<ASTExpr>& rightTerm){
    this->leftTerm = leftTerm;
    this->additiveOp = additiveOp;
    this->rightTerm = rightTerm;
}

ASTAdditiveOp::~ASTAdditiveOp(){
    leftTerm.reset();
    rightTerm.reset();
}

ASTRelationalOp::ASTRelationalOp(const shared_ptr<ASTExpr>& leftSimpleExpr,const string& relationalOp ,const shared_ptr<ASTExpr>& rightSimpleExpr){
    this->leftSimpleExpr = leftSimpleExpr;
    this->relationalOp = relationalOp;
    this->rightSimpleExpr = rightSimpleExpr;
}

ASTRelationalOp::~ASTRelationalOp(){
    leftSimpleExpr.reset();
    rightSimpleExpr.reset();
}

ASTVariableDecl::ASTVariableDecl(const shared_ptr<ASTIdentifier>& identifier,const shared_ptr<ASTType>& type,const shared_ptr<ASTExpr>& expression) {
    this->identifier=identifier;
    this->type=type;
    this->expression=expression;
}

ASTVariableDecl::~ASTVariableDecl() {
    identifier.reset();
    type.reset();
    expression.reset();
}

ASTType::ASTType(const string& value) {
    this->value=value;
}

ASTSubExpr::ASTSubExpr(const shared_ptr<ASTExpr> &expression) {
    this->expression=expression;
}

ASTSubExpr::~ASTSubExpr() {
    expression.reset();
}

ASTUnary::ASTUnary(const shared_ptr<ASTExpr> &expression, const string& UnaryOperator) {
    this->expression=expression;
    this->UnaryOperator=UnaryOperator;
}

ASTUnary::~ASTUnary() {
    expression.reset();
}

ASTPadRandi::ASTPadRandi(const shared_ptr<ASTExpr>& expression) {
    this->expression=expression;
}

ASTPadRandi::~ASTPadRandi() {
    expression.reset();
}

ASTBlock::ASTBlock(const vector<shared_ptr<ASTStatement>> &statements) {
    this->statements=statements;
}

ASTBlock::~ASTBlock() {
    statements.clear();
}

ASTPrintStatement::ASTPrintStatement(const shared_ptr<ASTExpr> &expression) {
    this->expression=expression;
}

ASTPrintStatement::~ASTPrintStatement() {
    expression.reset();
}

ASTDelayStatement::ASTDelayStatement(const shared_ptr<ASTExpr> &expression) {
    this->expression=expression;
}

ASTDelayStatement::~ASTDelayStatement() {
    expression.reset();
}

ASTRtrnStatement::ASTRtrnStatement(const shared_ptr<ASTExpr> &expression) {
    this->expression=expression;
}

ASTRtrnStatement::~ASTRtrnStatement() {
    expression.reset();
}

ASTPixelStatement::ASTPixelStatement(const string& value,const vector<shared_ptr<ASTExpr>>& expressions) {
    this->value=value;
    this->expressions=expressions;
}

ASTPixelStatement::~ASTPixelStatement() {
    expressions.clear();
}

ASTIfStatement::ASTIfStatement(const shared_ptr<ASTExpr> &expression, const shared_ptr<ASTBlock> &firstBlock,const shared_ptr<ASTBlock> &secondBlock) {
    this->expression=expression;
    this->firstBlock=firstBlock;
    this->secondBlock=secondBlock;
}

ASTIfStatement::~ASTIfStatement() {
    expression.reset();
    firstBlock.reset();
    secondBlock.reset();
}

ASTForStatement::ASTForStatement(const shared_ptr<ASTExpr> &expression, const shared_ptr<ASTAssignment> &assignment,const shared_ptr<ASTVariableDecl> &variableDecl, const shared_ptr<ASTBlock> &block) {
    this->expression=expression;
    this->assignment=assignment;
    this->variableDecl=variableDecl;
    this->block=block;
}

ASTForStatement::~ASTForStatement() {
    expression.reset();
    assignment.reset();
    variableDecl.reset();
    block.reset();
}

ASTWhileStatement::ASTWhileStatement(const shared_ptr<ASTExpr> &expression, const shared_ptr<ASTBlock> &block) {
    this->expression=expression;
    this->block=block;
}

ASTWhileStatement::~ASTWhileStatement() {
    expression.reset();
    block.reset();
}

ASTFormalParam::ASTFormalParam(const shared_ptr<ASTIdentifier> &identifier, const shared_ptr<ASTType> &type) {
    this->identifier=identifier;
    this->type=type;
}

ASTFormalParam::~ASTFormalParam() {
    identifier.reset();
    type.reset();
}

ASTFormalParams::ASTFormalParams(const vector<shared_ptr<ASTFormalParam>> &formalParams) {
    this->formalParams=formalParams;
}

ASTFormalParams::~ASTFormalParams(){
    formalParams.clear();
}

ASTFunctionDecl::ASTFunctionDecl(const shared_ptr<ASTIdentifier> &identifier,const shared_ptr<ASTFormalParams> &formalParams, const shared_ptr<ASTType> &type,const shared_ptr<ASTBlock> &block) {
    this->identifier=identifier;
    this->formalParams=formalParams;
    this->type=type;
    this->block=block;
}

ASTFunctionDecl::~ASTFunctionDecl() {
    identifier.reset();
    formalParams.reset();
    type.reset();
    block.reset();
}
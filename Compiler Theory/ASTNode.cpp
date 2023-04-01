#include <utility>

#include "HeaderFile.h"


ASTProgram::ASTProgram(const vector<shared_ptr<ASTStatement>>& program) {
    this->program=program;
}

ASTProgram::~ASTProgram() {
    program.clear();
}

ASTIdentifier::ASTIdentifier(string identifier) {
    this->identifier=move(identifier);
}

ASTAssignment::ASTAssignment(const shared_ptr<ASTIdentifier>& identifier, const shared_ptr<ASTExpr>& expression) {
    this->identifier=identifier;
    this->expression=expression;
}

ASTAssignment::~ASTAssignment() {
    identifier.reset();
    expression.reset();
}

ASTIntLiteral::ASTIntLiteral(const string& value) {
    this->value=stoi(value);
}

ASTColourLiteral::ASTColourLiteral(const string &value) {
    this->value=value;
}

ASTFloatLiteral::ASTFloatLiteral(const string &value) {
    this->value =stof(value);
}

ASTBoolLiteral::ASTBoolLiteral(const string &value) {
    if(value=="true"){
        this->value=true;
    }
    else if(value=="false"){
        this->value=false;
    }
}

ASTPadHeight::ASTPadHeight(const string &value) {
    this->value;
}

ASTPadRead::ASTPadRead(const string &value) {
    this->value;
}

ASTPadWidth::ASTPadWidth(const string &value) {
    this->value;
}

ASTFunctionCall::ASTFunctionCall(shared_ptr<ASTIdentifier> identifier, shared_ptr<ASTActualParams> actualParams) {
    this->identifier=move(identifier);
    this->actualParams=move(actualParams);
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
#include "HeaderFile.h"

Token::Token(string name,string attribute) {
    this->name=name;
    this->attribute=attribute;
}

string Token::GetTokenAttribute() {
    return attribute;
}

string Token::GetTokenName() {
    return name;
}
#include <utility>

#include "HeaderFile.h"

//Constructor for class
Token::Token(string name,string attribute) {
    this->name=move(name);
    this->attribute=move(attribute);
}
//Getter for Token Attribute
string Token::GetTokenAttribute() {
    return attribute;
}
//Getter for Token Name
string Token::GetTokenName() {
    return name;
}
//Setter for Token Name
void Token::SetTokenName(string newName) {
    name=move(newName);
}

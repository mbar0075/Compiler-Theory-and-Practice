#include "HeaderFile.h"

int main() {
    unique_ptr<Lexer> lexer= make_unique<Lexer>();
    lexer->LoadData();


    string filename="test.txt";
    //Loading File
    fstream readFilePointer;
    readFilePointer.open("../"+filename, ios::in);
    while(!readFilePointer.eof()){
        shared_ptr<Token> token = lexer->GetNextToken(readFilePointer);
        if(token->GetTokenName()!="<TokenDelim>") {//Do not need Token Delim token
            cout << "Token: <" << token->GetTokenName() << " , " << token->GetTokenAttribute() << ">" << endl;
        }
        else{
            token.reset();
        }
        token.reset();//Resetting pointer
    }


    return 0;
}

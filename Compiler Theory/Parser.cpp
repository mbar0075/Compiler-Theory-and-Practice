#include "HeaderFile.h"

void Parser::LLKParse(fstream &readFilePointer) {
    stack.push("<Program>$");
    while(!readFilePointer.eof()){
        shared_ptr<Token> token = lexer->GetNextToken(readFilePointer);
        CheckValidToken(token);
        if(token->GetTokenName()!="<TokenDelim>"&&token->GetTokenName()!="<Comment>") {//Do not need Token Delim token

//            while(!stack.empty()) {
//                if (stack.top() == "Terminal") {
//                    string r = stack.top();
//                    stack.pop();
//                    //if (r̸=t), report an error
//                } else if (stack.top() != "Terminal") {
//                    string A = stack.top();
//                    stack.pop();
//                    /*
//                     * if T[A,t] is undefined, report an error
//                     * otherwise push(W, T[A,t])
//                     */
//                }
//            }

            cout << "Token: <" << token->GetTokenName() << " , " << token->GetTokenAttribute() << ">" << endl;
        }
        else{
            token.reset();
        }
        token.reset();//Resetting pointer
    }
}

void Parser::CheckValidToken(const shared_ptr<Token>& token) {
    if(token->GetTokenName()=="<ColourLiteral>") {//hex number
        string colourLiteral=token->GetTokenAttribute();
        for(int i=1;i<colourLiteral.length();i++){
            bool validChar=validHexCharacters.find(colourLiteral[i])!= string::npos;
            if(!validChar){
                token->SetTokenName("<Invalid>");
            }
        }
    }
    if(token->GetTokenName()=="<SpecialCase1>") {//special case1 with __
        if(specialCase1[token->GetTokenAttribute()].empty()){
            token->SetTokenName("<Invalid>");
        }
        else{
            token->SetTokenName(specialCase1[token->GetTokenAttribute()]);
        }
    }
    if(token->GetTokenName()=="<SpecialCase2>") {//special case2 with __
        if(specialCase2[token->GetTokenAttribute()].empty()){
            token->SetTokenName("<Invalid>");
        }
        else{
            token->SetTokenName(specialCase2[token->GetTokenAttribute()]);
        }
    }
    if(token->GetTokenName()=="<SpecialCase3>") {//special case3 with __
        if(specialCase3[token->GetTokenAttribute()].empty()){
            token->SetTokenName("<Invalid>");
        }
        else{
            token->SetTokenName(specialCase3[token->GetTokenAttribute()]);
        }
    }

    //Error Checking
    if(token->GetTokenName()=="<Empty>") {//empty file
        cout << token->GetTokenAttribute()<< endl;
        exit(0);
    }
    if(token->GetTokenName()=="<Invalid>") {//invalid token
        cerr<<"\nError: Invalid Token: \n"<<token->GetTokenAttribute()<<endl;
        exit(3);
    }
    if(token->GetTokenName()=="<InvalidComment>") {//invalid comment
        cerr<<"\nError: Invalid Comment: \n"<<token->GetTokenAttribute()<<endl;
        exit(3);
    }
}

//Destructor for class
Parser::~Parser(){
    lexer.reset();
}

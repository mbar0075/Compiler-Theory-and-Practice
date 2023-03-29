#include "HeaderFile.h"

Parser::Parser() {
    LoadTables();
}

//Function which Loads the relevant tables, which will be used as look-up for the Parser
void Parser::LoadTables() {
    //Opening CAT Table File
    fstream catFilePointer;
    catFilePointer.open("../ParseTransTable.csv", ios::in);
    //Checking whether file is open
    if(catFilePointer.is_open()){
        string key,value;
        //Retrieving the dictionary key
        while(getline(catFilePointer,key,',')){
            //Retrieving value
            getline(catFilePointer, value);
            //Storing in table dictionary data structure
            tmpTable[key]=value;
        }
    }
    else{
        //Printing respective error message
        cerr<<"\nError: CAT Table not found"<<endl;
        exit(1);
    }
    //Closing file
    catFilePointer.close();

    //Opening Transition Table File
    fstream parseTableFilePointer;
    parseTableFilePointer.open("../ParseTable.csv", ios::in);
    //Checking whether file is open
    if(parseTableFilePointer.is_open()){
        string secondaryKeys,data;
        //Retrieving the secondary Keys
        getline(parseTableFilePointer,secondaryKeys);

        string key;
        //Retrieving the dictionary key
        while(getline(parseTableFilePointer,key,',')){
            //Removing first , from secondary Keys
            stringstream str2(secondaryKeys);
            string value,secondaryKey;
            getline(parseTableFilePointer,value);
            stringstream str(value);
            getline(str2,secondaryKey,',');
            //Retrieving the dictionary secondary key and value
            while(getline(str2,secondaryKey,',')) {
                getline(str,value,',');
                //Storing in table dictionary data structure whereby (first index is state, secondary index is the token)
                parseTable[key][secondaryKey]=value;
                terminals[secondaryKey]=secondaryKey;//terminals
            }
        }
    }else{
        //Printing respective error message
        cerr<<"\nError: Transition Table not found"<<endl;
        exit(1);
    }
    //Closing file
    parseTableFilePointer.close();
}

void Parser::LLKParse(fstream &readFilePointer) {
    stack.push("$");
    stack.push("<Program>");
    currentToken= lexer->GetNextToken(readFilePointer);//Lookahead first token
    CheckValidToken(currentToken);
    while(!stack.empty()){
        if(stack.top()=="$"){
            if(!readFilePointer.eof()){
                currentToken=lookaheadToken1;
                lookaheadToken1=lexer->GetNextToken(readFilePointer);
                CheckValidToken(lookaheadToken1);
                stack.push("<Statement>");
            }
            else{
                break;
            }

        }


        if(currentToken->GetTokenName()!="<TokenDelim>"&&currentToken->GetTokenName()!="<Comment>") {//Do not need Token Delim token
            cout << "Token: <" << currentToken->GetTokenName() << " , " << currentToken->GetTokenAttribute() << ">" << endl;

            //Parsing Algorithm
            if(!terminals[stack.top()].empty()){
                string r = stack.top();
                stack.pop();
                cout<<"Terminal: "<<currentToken->GetTokenName()<<" "<<r<<endl;
                if(r!=currentToken->GetTokenName()){
                    cerr<<"\nError: "<<currentToken->GetTokenAttribute()<<" is not a "<<r<<endl;
                    exit(-1);
                }
                currentToken=lookaheadToken1;
                if(!readFilePointer.eof()) {
                    lookaheadToken1 = lexer->GetNextToken(readFilePointer);
                    CheckValidToken(lookaheadToken1);
                }
            }
            else if(terminals[stack.top()].empty()){

                string A = stack.top();
                stack.pop();
                cout<<"Non-Terminal: "<<currentToken->GetTokenName()<<" "<<A<<endl;
                if(parseTable[A][currentToken->GetTokenName()].empty()){
                    cerr<<"\nError: "<<currentToken->GetTokenAttribute()<<" is not a "<<A<<endl;
                    exit(-1);
                }
                else{
                    //Here you need to add to the ast tree
                    stack.push(tmpTable[parseTable[A][currentToken->GetTokenName()]]);
                }
            }
        }
        else{
            currentToken=lookaheadToken1;
            if(!readFilePointer.eof()) {
                lookaheadToken1 = lexer->GetNextToken(readFilePointer);
                CheckValidToken(lookaheadToken1);
            }
        }
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
    currentToken.reset();
    lookaheadToken1.reset();
}

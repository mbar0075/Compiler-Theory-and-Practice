#include "HeaderFile.h"

void Lexer::LoadData() {
    //Opening CAT Table File
    fstream catFilePointer;
    catFilePointer.open("../CAT.csv", ios::in);
    //Checking whether file is open
    if(catFilePointer.is_open()){
        string key,value;
        //Retrieving the dictionary key
        while(getline(catFilePointer,key,',')){
            //Retrieving value
            getline(catFilePointer, value);
            //Storing in dictionary
            catTable[key]=value;
        }
    }
    else{
        //Printing respective error message
        cerr<<"Error: CAT Table not found"<<endl;
        exit(1);
    }
    //Closing file
    catFilePointer.close();

    //Opening identifier Table File
    fstream identifierTableFilePointer;
    identifierTableFilePointer.open("../IdentifierTable.csv", ios::in);
    //Checking whether file is open
    if(identifierTableFilePointer.is_open()){
        string key,value;
        //Retrieving the dictionary key
        while(getline(identifierTableFilePointer,key,',')){
            //Retrieving value
            getline(identifierTableFilePointer, value);
            //Storing in dictionary
            identifierTable[key]=value;
        }
    }
    else{
        //Printing respective error message
        cerr<<"Error: Identifier Table not found"<<endl;
        exit(1);
    }

    //Opening Token Table File
    fstream tokenTableFilePointer;
    tokenTableFilePointer.open("../TokenTable.csv", ios::in);
    //Checking whether file is open
    if(tokenTableFilePointer.is_open()){
        string key,value;
        //Retrieving the dictionary key
        while(getline(tokenTableFilePointer,key,',')){
            //Retrieving value
            getline(tokenTableFilePointer, value);
            //Storing in dictionary
            tokenTable[key]=value;
        }
    }
    else{
        //Printing respective error message
        cerr<<"Error: Token Table not found"<<endl;
        exit(1);
    }
    //Closing file
    tokenTableFilePointer.close();

    //Opening Transition Table File(first index is state, secondary index is the token)
    fstream transitionTableFilePointer;
    transitionTableFilePointer.open("../TransTable.csv", ios::in);
    //Checking whether file is open
    if(transitionTableFilePointer.is_open()){
        string secondaryKeys,data;
        //Retrieving the secondary Keys
        getline(transitionTableFilePointer,secondaryKeys);

        string key;
        //Retrieving the dictionary key
        while(getline(transitionTableFilePointer,key,',')){
            //Removing first , from secondary Keys
            stringstream str2(secondaryKeys);
            string value,secondaryKey;
            getline(transitionTableFilePointer,value);
            stringstream str(value);
            getline(str2,secondaryKey,',');
            //Retrieving the dictionary secondary key and value
            while(getline(str2,secondaryKey,',')) {
                getline(str,value,',');
                //Storing in dictionary
                transitionTable[key][secondaryKey]=value;
            }
        }
    }else{
        //Printing respective error message
        cerr<<"Error: Transition Table not found"<<endl;
        exit(1);
    }
    //Closing file
    transitionTableFilePointer.close();
}

shared_ptr<Token> Lexer::GetNextToken(fstream &readFilePointer) {
    //Initialisation
    string state="S0";
    string lexeme;
    ClearStack();
    stack.push("Bad");

    //Checking whether File is open
    if(readFilePointer.is_open()) {
        //Scanning Loop
        while(state!="Serr"&&!readFilePointer.eof()){
            //cout<<"Scanning Loop: "<<endl;
            //cout<<"Lexeme: "<<lexeme<<endl;
            //cout<<"State: "<<state<<endl;

            //Reading nextChar
            char nextChar;
            //noskipws to not skip spaces when reading from file
            readFilePointer >> noskipws >> nextChar;
            //Checking whether end of file is reached // Error Checking
            if(readFilePointer.eof()){
                break;
            }
            lexeme+=nextChar;
            if(tokenTable[state]!="<Invalid>"){
                ClearStack();
            }
            stack.push(state);
            string token;
            token=nextChar;
            string tokenType=catTable[token];
            //cout<<"Token : \""<<token<<"\""<<endl;
            //cout<<"Token Type"<<tokenType<<endl;
            //cout<<"Transition Table "<<transitionTable[state][tokenType]<<endl;
            //Checking for token delimeters
            if(token ==","){/*Hard coded since csv files do not work*/
                //cout<<"In here"<<endl;
                state=transitionTable[state]["<Comma>"];
                //cout<<"Transition Table "<<transitionTable[state]["<TokenDelim>"]<<endl;
            }
            else if(token ==" "){
                //cout<<"In here"<<endl;
                state=transitionTable[state]["<TokenDelim>"];
                //cout<<"Transition Table "<<transitionTable[state]["<TokenDelim>"]<<endl;
            }
            else if(token == "\n"){
                state=transitionTable[state]["<TokenDelimEnter>"];
            }
            else{
                state=transitionTable[state][tokenType];
            }

        }

        //RollBack Loop
        while(tokenTable[state]=="<Invalid>"&&state!="Bad"){
            //cout<<"Lexeme: "<<lexeme<<endl;
            //cout<<"State: "<<state<<endl;
            state=stack.top();
            stack.pop();
            //cout<<"NewState: "<<state<<endl;
            readFilePointer.putback(lexeme[lexeme.length()-1]);
            lexeme = lexeme.substr(0, lexeme.length() - 1);//Truncating lexeme
        }

        //Report Result
        if(state!="Bad"&&tokenTable[state]!="<Invalid>"){
            string tokenName=tokenTable[state];
            //Checking for other keywords
            if(tokenTable[state]=="<Identifier>"){
                if(identifierTable.find(lexeme)!=identifierTable.end()){//Checking whether lexeme is one of the keys in the identifierTable
                    tokenName=identifierTable[lexeme];
                }

                //cout<<"Token Name: "<<tokenName<<endl;
            }
            shared_ptr<Token> token = make_shared<Token>(tokenName,lexeme);
            //cout<<"Token: <"<<token->GetTokenName()<<" , "<<token->GetTokenAttribute()<<">"<<endl;
            return token;
        }
        else{
            cerr<<"Error: Invalid Token: "<<lexeme<<endl;
            exit(3);
        }
    }
    else{
        //Printing respective error message
        cerr<<"Error: File not Found"<<endl;
        exit(2);
    }
}

void Lexer::ClearStack(){
    while(!stack.empty()){
        stack.pop();
    }
}

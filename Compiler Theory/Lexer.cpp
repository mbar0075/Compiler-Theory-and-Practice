//Inclusion of relevant header File
#include "HeaderFile.h"

//Constructor for class
Lexer::Lexer() {
    LoadTables();
}
//Function which Loads the relevant tables, which will be used as look-up for the Lexer
void Lexer::LoadTables() {
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
            //Storing in table dictionary data structure
            catTable[key]=value;
        }
    }
    else{
        //Printing respective error message
        cerr<<"\nError: CAT Table not found"<<endl;
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
            //Storing in table dictionary data structure
            identifierTable[key]=value;
        }
    }
    else{
        //Printing respective error message
        cerr<<"\nError: Identifier Table not found"<<endl;
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
            //Storing in table dictionary data structure
            tokenTable[key]=value;
        }
    }
    else{
        //Printing respective error message
        cerr<<"\nError: Token Table not found"<<endl;
        exit(1);
    }
    //Closing file
    tokenTableFilePointer.close();

    //Opening Transition Table File
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
                //Storing in table dictionary data structure whereby (first index is state, secondary index is the token)
                transitionTable[key][secondaryKey]=value;
            }
        }
    }else{
        //Printing respective error message
        cerr<<"\nError: Transition Table not found"<<endl;
        exit(1);
    }
    //Closing file
    transitionTableFilePointer.close();
}

//Function which retrieves the Next Token given a file stream
shared_ptr<Token> Lexer::GetNextToken(fstream &readFilePointer) {
    //Initialisation
    //Setting state to starting state, Clearing the Stack, and appending Bad to the state
    string state="S0";
    string lexeme;
    ClearStack();
    stack.push("Bad");

    //Checking whether File is open
    if(readFilePointer.is_open()) {
        //Checking for empty file and producing Empty Token
        char tempChar;
        //Reading first Character from the file
        readFilePointer >> noskipws >> tempChar;
        //Checking whether file pointer is pointing to end of file, if so returning empty token
        if(readFilePointer.eof()){
            shared_ptr<Token> token = make_shared<Token>("<Empty>","Empty File");
            return token;
        }
        //Putting back first character to file stream
        readFilePointer.putback(tempChar);
        //Variable which will be used as a place-holder to display the type of invalid token
        string invalidToken;
        //Scanning Loop (Looping until state is not error state, and file pointer has not reached end of file)
        while(state!="Serr"&&!readFilePointer.eof()){
            //Reading next Character from the file stream
            char nextChar;
            readFilePointer >> noskipws >> nextChar;
            //ErrorChecking: Checking whether end of file is reached
            if(readFilePointer.eof()){
                break;
            }
            //Appending nextChar to invalidToken and lexeme
            invalidToken+=nextChar;
            lexeme+=nextChar;
            //Checking whether state is not Invalid, through tokenTable look-up, and if so clearing stack
            if(tokenTable[state]!="<Invalid>"){
                ClearStack();
            }
            //Pushing state on the stack, and retrieving tokenType through catTable look-up
            stack.push(state);
            string token;
            token=nextChar;
            string tokenType=catTable[token];

            //Static Checks for Token Delimiters and Commas, as was proven to being difficult through the use of csv file
            //Updating the states respectively through transitionTable look-up, based on the state and tokenType
            if(token ==","){
                state=transitionTable[state]["<Comma>"];
            }
            else if(token ==" "){
                state=transitionTable[state]["<TokenDelim>"];
            }
            else if(token == "\n"){
                state=transitionTable[state]["<TokenDelimEnter>"];
            }
            else{
                state=transitionTable[state][tokenType];
            }
        }

        //RollBack Loop (Looping until state is Invalid and state is not Bad)
        while(tokenTable[state]=="<Invalid>"&&state!="Bad"){
            //Popping state from the stack
            state=stack.top();
            stack.pop();
            //Truncating Lexeme
            readFilePointer.putback(lexeme[lexeme.length()-1]);
            lexeme = lexeme.substr(0, lexeme.length() - 1);
        }

        //Report Result
        //Checking whether state is not Bad, and state is not Invalid
        if(state!="Bad"&&tokenTable[state]!="<Invalid>"){
            string tokenName=tokenTable[state];
            //Checking for specific keywords, in the case the token is an identifier
            if(tokenTable[state]=="<Identifier>"){
                //Checking whether lexeme is one of the keys in the identifierTable
                if(identifierTable.find(lexeme)!=identifierTable.end()){
                    tokenName=identifierTable[lexeme];
                }
            }
            //Checking whether tokenNam is empty, if so setting tokenName to Invalid
            if(tokenName.empty()){
                tokenName="<Invalid>";
            }
            //Returning Token
            shared_ptr<Token> token = make_shared<Token>(tokenName,lexeme);
            return token;
        }
        else{
            //Printing respective error message
            cerr<<"\nError: Invalid Token: "<<invalidToken<<endl;
            exit(3);
        }
    }
    else{
        //Printing respective error message
        cerr<<"\nError: File not Found"<<endl;
        exit(2);
    }
}

//Function which Clears the Stack, until stack is empty
void Lexer::ClearStack(){
    while(!stack.empty()){
        stack.pop();
    }
}

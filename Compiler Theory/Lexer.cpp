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



//    map<string, string>::iterator it;
//    for (it = catTable.begin(); it != catTable.end(); it++)
//    {
//        cout<<it->first<<" : "<<it->second<<endl;
//    }
//    cout<<endl;
//    for (it = tokenTable.begin(); it != tokenTable.end(); it++)
//    {
//        cout<<it->first<<" : "<<it->second<<endl;
//    }
//    cout<<tokenTable["S0"]<<endl;
//    cout << "Hello, World!" << endl;
//cout<<transitionTable["S0"]["<letter>"]<<endl;
}

string Lexer::NextWord(string filename) {
    //Initialisation
    string state="S0";
    string lexeme="";
    ClearStack();
    stack.push("bad");

    //Loading File
    fstream readFilePointer;
    readFilePointer.open("../"+filename, ios::in);

    //Checking whether File is open
    if(readFilePointer.is_open()) {
        //Scanning Loop
        while(state!="Serr"&&!readFilePointer.eof()){
            //Reading nextChar
            char nextChar;
            readFilePointer >> nextChar;
            //Checking whether end of file is reached // Error Checking
            if(readFilePointer.eof()){
                break;
            }
            lexeme+=nextChar;
            if(tokenTable[state]!="invalid"){
                ClearStack();
            }
            stack.push(state);
            string token="";
            token=nextChar;
            string tokenType=catTable[token];
            state=transitionTable[state][tokenType];
        }

        //RollBack Loop
        while(tokenTable[state]=="invalid"&&state!="bad"){
            state=stack.top();
            stack.pop();
            lexeme= lexeme.substr (0,lexeme.length()-1);//Truncating lexeme
        }
        cout<<"Lexeme: "<<lexeme<<endl;

        //Report Result
        if(state!="bad"&&tokenTable[state]!="invalid"){
            cout<<tokenTable[state]<<endl;
            return tokenTable[state];
        }
        else{
            cerr<<"Error: Invalid Token"<<endl;
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

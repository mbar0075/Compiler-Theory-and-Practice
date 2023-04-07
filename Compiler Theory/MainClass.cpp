//Inclusion of relevant header File
#include "HeaderFile.h"

int main() {
    unique_ptr<Parser> parser= make_unique<Parser>();

    string filename="PixIR.txt";
    //Loading File
    fstream readFilePointer;
    readFilePointer.open("../"+filename, ios::in);
    //parser->LLKParse(readFilePointer);
    parser->XMLPass(readFilePointer);
    parser.reset();

    return 0;
}

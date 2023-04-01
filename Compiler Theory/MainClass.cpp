#include "HeaderFile.h"

int main() {
    unique_ptr<Parser> parser= make_unique<Parser>();

    string filename="test.txt";
    //Loading File
    fstream readFilePointer;
    readFilePointer.open("../"+filename, ios::in);
    //parser->LLKParse(readFilePointer);
    shared_ptr<ASTProgram> program =parser->ParseProgram(readFilePointer);
    parser.reset();

    return 0;
}

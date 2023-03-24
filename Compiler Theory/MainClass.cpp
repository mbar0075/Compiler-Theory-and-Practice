#include "HeaderFile.h"

int main() {
    unique_ptr<Lexer> lexer= make_unique<Lexer>();
    lexer->LoadData();
    lexer->NextWord("test.txt");

    return 0;
}

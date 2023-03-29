#include "HeaderFile.h"

int main() {
    unique_ptr<Parser> parser= make_unique<Parser>();

    string filename="test.txt";
    //Loading File
    fstream readFilePointer;
    readFilePointer.open("../"+filename, ios::in);
    parser->LLKParse(readFilePointer);
    parser.reset();

//    //list< shared_ptr<ASTNode>> tree;
//    shared_ptr<ASTNode> root;
//    shared_ptr<ASTNode> node = make_shared<ASTNode>("Program","test", nullptr);
//    node->ShowChildren();
//    node->CreateChild(make_shared<ASTNode>("Program1","test1", node));
//    node->ShowChildren();
//    node->CreateChild(make_shared<ASTNode>("Program2","test2", node));
//    node->ShowChildren();
//    root=node;
//    //tree.push_back(node);
//    //tree.splice(tree.end(),node->GetChildren());
//    node.reset();//not needed

    return 0;
}

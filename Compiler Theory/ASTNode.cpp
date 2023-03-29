#include "HeaderFile.h"

ASTNode::ASTNode(string ASTType, string nodeName,shared_ptr<ASTNode> newparent) {
    this->ASTType=move(ASTType);
    this->nodeName=move(nodeName);
    this->parent=move(newparent);
}

string ASTNode::GetASTType() {
    return ASTType;
}

string ASTNode::GetNodeName() {
    return nodeName;
}

shared_ptr<ASTNode> ASTNode::GetNodeParent() {
    return parent;
}

void ASTNode::SetNodeParent(shared_ptr<ASTNode> newparent) {
    this->parent=move(newparent);
}

void ASTNode::CreateChild(const shared_ptr<ASTNode>& child) {
    children.push_back(child);
}

list< shared_ptr<ASTNode>> ASTNode::GetChildren() {
    return children;
}

void ASTNode::ShowChildren() {
    list<shared_ptr<ASTNode>>:: iterator it;

    cout<<"\nASTType: "<<ASTType<<" Node: "<<nodeName<<" Children:"<<endl;
    for(it=children.begin();it!=children.end();it++){
        cout<<"ASTType: "<<(*it)->GetASTType()<<" Node: "<<(*it)->GetNodeName()<<endl;
    }
}

ASTNode::~ASTNode() {
    parent.reset();
    children.clear();
}
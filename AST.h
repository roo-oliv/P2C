#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <vector>


struct Node {
    int regra;
    Token *tk;
    vector<Node*> children;
    Node *parent;
};

class AST{
public:
    AST(){
        Node* root = new Node;
    }
    ~AST() {}
    
    Node* createNode(int regra){
        Node *newNode = new Node;
        newNode->regra = regra;
        newNode->tk = NULL;
        newNode->parent = root;
        return newNode;
    }
    
    Node* createLeaf(Token* tk){
        Node *newNode = new Node;
        newNode->tk = tk;
        newNode->regra = -1;
        newNode->parent = root;
        return newNode;
    }
    
    Node* addChild(Node* parent, Node* child){
        parent->children.push_back(child);
        child->parent = parent;
        return parent;
    }
    
    Node* getRoot(){
        return root;
    }
    
    void setRoot(Node* nroot){
        root = nroot;
    }
    
private:
    Node* root;
};
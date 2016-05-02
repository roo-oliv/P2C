#include "datastructures.hpp"

std::string compiler::Node::getKindName() {
    switch (kind) {
        case 0 :
            return "unary operator";
        case 1 :
            return "binary operator";
        case 2 :
            return "assignment operator";
        case 3 :
            return "scope operator";
        case 4 :
            return "flow control";
        case 5 :
            return "delimiter";
        case 6 :
            return "parameter";
        case 7 :
            return "variable";
        case 8 :
            return "function";
        case 9 :
            return "Number";
        case 10 :
            return "BooleanType";
        case 11 :
            return "PyTupleObject";
        case 12 :
            return "PyListObject";
        case 13 :
            return "indentation";
        case 14 :
            return "NoneType";
        case 15 :
            return "conditional";
        case 16 :
            return "else";
        case 17 :
            return "for";
        case 18 :
            return "def";
        case 19 :
            return "pass";
        case 20 :
            return "pending";
        case 21 :
            return "whole expression";
        default :
            return "invalid kind";
    }
}

compiler::TokenList::TokenList() {
    heading = new Token;
    trailing = heading;
    size = 0;
}

compiler::TokenList::~TokenList() { }

//Inserir novos tokens na lista
int compiler::TokenList::push(Token* t) {
    t->next = NULL;
    trailing->next = t;
    t->parent = trailing;
    trailing = t;
    size++;
    return 0;
}

//Retirar o último token da lista
compiler::Token* compiler::TokenList::pop() {
    if(size>0){
        Token* t = trailing;
        trailing = trailing->parent;
        trailing->next = NULL;
        t->parent = NULL;
        size--;
        return t;
    }else{
        std::cout<<"lista vazia"<<std::endl;
        return NULL;
    }
}

//Verificar o ultimo token da lista
compiler::Token* compiler::TokenList::readTrailing() { return trailing; }

//Verificar o primeiro token da lista
compiler::Token* compiler::TokenList::readHeading() { return heading; }

//Quantidade de elementos da lista
int compiler::TokenList::getSize() { return size; }

compiler::IndentStack::IndentStack() {
    top = NULL;
    nElem = size = 0;
}

compiler::IndentStack::~IndentStack() {}

//Inserir elementos na pilha
int compiler::IndentStack::push(Indent *i) {
    i->next = top;
    top = i;
    nElem++;
    size += i->spaces;
    return 0;
}

//Retirar o elemento do topo da pilha
compiler::Indent* compiler::IndentStack::pop() {
    if(nElem > 0) {
        Indent* i = top;
        top = top->next;
        nElem--;
        size = size - i->spaces;
        return i;
    }else {
        std::cout << "ERROR: empty stack!" << std::endl;
        return NULL;
    }
}

//Verificar o elemento do topo da pilha
compiler::Indent* compiler::IndentStack::getTop() { return top; }

//Verificar a quantidade de elementos da pilha
int compiler::IndentStack::getNElem() { return nElem; }

//Verificar a quantidade de espaços que foram inseridos no total
int compiler::IndentStack::getSize() { return size; }

compiler::AST::AST(){
    Node* root = new Node;
}
compiler::AST::~AST() {}

compiler::Node* compiler::AST::createNode(int regra){
    Node *newNode = new Node;
    newNode->regra = regra;
    newNode->tk = NULL;
    newNode->parent = root;
    return newNode;
}

compiler::Node* compiler::AST::createLeaf(Token* tk){
    Node *newNode = new Node;
    newNode->tk = tk;
    newNode->regra = -1;
    newNode->parent = root;
    return newNode;
}

compiler::Node* compiler::AST::addChild(Node* parent, Node* child){
    parent->children.push_back(child);
    child->parent = parent;
    return parent;
}

compiler::Node* compiler::AST::getRoot(){
    return root;
}

void compiler::AST::setRoot(Node* nroot){
    root = nroot;
}

compiler::SymbolTable::SymbolTable() { }

compiler::SymbolTable::~SymbolTable() { }

compiler::HashTable::iterator compiler::SymbolTable::insert(std::string name, int kind, int type, std::pair<int, int> pos, int scope, std::string content) {
    Symbol *s = new Symbol;
    s->name = name;
    s->kind = kind;
    s->type = type;
    s->pos = pos;
    s->scope = scope;
    s->content = content;
    auto it = table.find(name);
    if(it==table.end()) {
        std::vector<Symbol*> v = {s};
        return table.insert(it, HashTable::value_type(name, v));
    } else {
        it->second.push_back(s);
        return it;
    }
}

void compiler::SymbolTable::erase(std::string name, int scope) {
    auto it = table.find(name);
    if(it!=table.end()) {
        for(auto i = it->second.begin(); i != it->second.end(); ++i) {
            if((*i)->scope==scope) {
                it->second.erase(i);
                break;
            }
        }
        if(it->second.empty()) table.erase(it);
    }
}

compiler::HashTable::iterator compiler::SymbolTable::lookup(std::string name) { return table.find(name); }

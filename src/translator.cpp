#include "translator.hpp"

compiler::Translator::Translator(AST *ast, SymbolTable *table) {
    this->ast = ast;
    this->table = table;
}

compiler::Translator::~Translator() { }

void compiler::Translator::translate() {
    Node *root = ast->getRoot();
    std::fstream fs;
    fs.open ("output.cpp", std::fstream::in | std::fstream::out | std::fstream::app);
    descend(root, &fs);
    fs.close();
}

void compiler::Translator::descend(Node *r, std::fstream *fs) {
    switch (r->regra) {
        case 78: // x+y or x-y
            //for(auto &i : r->children)
                //descend();
            break;
        default:
            break;
    }
}

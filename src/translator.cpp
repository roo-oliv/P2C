#include "translator.hpp"

compiler::Translator::Translator(AST *ast, SymbolTable *table) {
    this->ast = ast;
    this->table = table;
}

compiler::Translator::~Translator() { }

void compiler::Translator::translate() {
    Node *root = ast->getRoot();
    std::ofstream fs("output.cpp");
    fs << "#include <iostream>\n\n"
        << "int main() {\n";
    #ifdef DEBUG
        std:cerr << "\n\n----------TRANSLATOR----------\n\n");
    #endif
    descend(root, fs, 1);
    fs << "}";
    fs.close();
}

void compiler::Translator::descend(Node *r, std::ofstream &fs, int tabs) {
    std::vector<Node*> v;
    switch (r->regra) {
        case -1:
            switch (r->tk->type) {
                case 11:
                    fs << "a" << table->lookup(r->content)->second[/*v.back()->scope*/0];
                    break;
                case 29: case 31: case 33:
                    break;
                default:
                    fs << r->content << " ";
            }
            break;
        case 6: case 8:
            descend(r->children.back(), fs, tabs);
            fs << ";\n";
            break;
        case 7:
            descend(r->children.back(), fs, tabs);
            fs << "; ";
            descend(r->children.at(1), fs, tabs);
            break;
        case 13:
            fetchChildren(r->children.at(0), v);
            for(int i=0; i<tabs; i++) fs << "\t";
            if(v.size()==1)
                fs << "a" << table->lookup(v.back()->content)->second[/*v.back()->scope*/0] << ".clear()";
            else
                fs << "a" << table->lookup(v.back()->content)->second[/*v.back()->scope*/0] << ".erase("
                    << "a" << table->lookup(v.back()->content)->second[/*v.back()->scope*/0] << ".begin()+"
                    << v.at(1)->content << ")";
            break;
        case 14:
            break;
        case 17:
            break;
        case 19:
            /*???*/
            break;
        case 20:
            fetchChildren(r->children.back(), v);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "a" << table->lookup(v.back()->content)->second[/*v.back()->scope*/0] << " "
                << r->children.at(1)->content << " ";
            descend(r->children.at(0), fs, tabs);
            break;
        case 21:
            fetchChildren(r->children.back(), v);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "auto a" << table->lookup(v.back()->content)->second[/*v.back()->scope*/0] << " ";
            descend(r->children.at(0), fs, tabs);
            break;
        case 44:
            fetchChildren(r->children.at(2), v);
            table->insert("__cmpvar__", 7, r->children.back()->tk->lin, r->children.back()->tk->col, 0);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "auto " << "a" << table->lookup("__cmpvar__")->second[/*v.back()->scope*/0] << " = ";
            if(v.size()>1) {
                fs << "{";
                for (unsigned i = v.size()-1; i-- > 1; ) fs << v.at(i)->content;
                for(int i=0; i<tabs; i++) fs << "\t";
                fs << "};\n";
            } else {
                for(int i=0; i<tabs; i++) fs << "\t";
                fs << "a" << table->lookup(v.back()->content)->second[/*v.back()->scope*/0] << ";\n";
            }
            v.clear();
            fetchChildren(r->children.at(4), v);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "for (auto " << "a" << table->lookup(v.back()->content)->second[/*v.back()->scope*/0]
                << " : a" << table->lookup("__cmpvar__")->second[/*v.back()->scope*/0] << ")";
            descend(r->children.at(0), fs, tabs);
            break;
        case 45:
            break;
        case 46:
            fetchChildren(r->children.at(2), v);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "auto a" << table->lookup(r->children.at(3)->content)->second[/*r->children.at(3)->scope*/0];
            descend(r->children.at(2), fs, tabs);
            descend(r->children.at(0), fs, tabs);
            break;
        case 49:
            fs << " {\n";
            descend(r->children.back(), fs, tabs+1);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "}\n";
            break;
        case 50:
            fs << " {\n";
            descend(r->children.at(1), fs, tabs+1);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "}\n";
            break;
        default:
            for (unsigned i = r->children.size(); i-- > 0; )
                descend(r->children.at(i), fs, tabs);
            break;
    }
    /*for (unsigned i = r->children.size(); i-- > 0; )
        descend(r->children[i], fs);
    //if(r->regra==-1) std::cerr << r->getKindName() << " : " << r->content << std::endl;
    if(r->regra==-1) {
        switch (r->kind) {
            case 0: case 2:
                fs << r->tk->lexema;
                break;
            case 1: // !!!! "**"
                fs << r->tk->lexema;
                break;
            case 3: // !!!! "nonlocal"
                fs << r->tk->lexema;
                break;
            case 4: // !!!! "pass"
                fs << r->tk->lexema;
                break;
            case 5: // !!!! ":"
                fs << r->tk->lexema;
                break;
            case 6:
                fs << r->tk->lexema;
                break;
            case 7:
                fs << r->tk->lexema;
                break;
            case 8:
                fs << r->tk->lexema;
                break;
            case 9:
                fs << r->tk->lexema;
                break;
            case 10:
                fs << r->tk->lexema;
                break;
            case 11:
                fs << r->tk->lexema;
                break;
            case 12:
                fs << r->tk->lexema;
                break;
            case 13:
                if(r->tk->type==33) fs << ";\n";
                else if(r->tk->type==31) fs << "{\n";
                else fs <<"}\n";
                break;
            case 14:
                fs << "NULL";
                break;
            case 15:
                fs << r->tk->lexema;
                break;
            case 16:
                fs << r->tk->lexema;
                break;
            case 17:
                fs << r->tk->lexema;
                break;
            case 18:
                fs << r->tk->lexema;
                break;
            case 19:
                fs << r->tk->lexema;
                break;
            case 20:
                fs << r->tk->lexema;
                break;
            case 21:
                fs << r->tk->lexema;
                break;
        }
    }*/
}

void compiler::Translator::fetchChildren(Node *r, std::vector<Node*> &v) {
    for(auto &i : r->children)
        fetchChildren(i,v);
    if(r->regra==-1) v.push_back(r);
}

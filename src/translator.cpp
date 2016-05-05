#include "translator.hpp"

compiler::Translator::Translator(AST *ast, SymbolTable *table) {
    this->ast = ast;
    this->table = table;
}

compiler::Translator::~Translator() { }

void compiler::Translator::translate() {
    Node *root = ast->getRoot();
    std::ofstream fs("output.cpp");
    fs << "#include <iostream>\n"
        << "#include \"pybuiltins.hpp\"\n"
        << "\n#define a" << table->lookup("print", 0) << "(arg) print(arg)\n"
        << "#define a" << table->lookup("abs", 0) << "(arg) abs(arg)\n"
        << "\nint main() {\n";
    #ifdef DEBUG
        std::cerr << "\n\n----------TRANSLATOR----------\n\n";
    #endif
    descend(root, fs, 1);
    fs << "}";
    fs.close();
}

void compiler::Translator::descend(Node *r, std::ofstream &fs, int tabs) {
    int aux;
    std::vector<Node*> *v;
    #ifdef DEBUG
        std::cerr << "Content = " << r->content << " : Kind = " << r->getKindName() << " : Rule = " << r->regra << "\n";
    #endif
    switch (r->regra) {
        case -1:
            switch (r->tk->type) {
                case 3:
                    fs << " || ";
                    break;
                case 4:
                    fs << " && ";
                    break;
                case 5:
                    fs << "!";
                    break;
                case 11:
                    //std::cerr << r->content << " : " << r->scope << "\n";
                    fs << "a" << table->lookup(r->content, r->scope);
                    break;
                case 15:
                    fs << "true";
                    break;
                case 19:
                    fs << "else if";
                    break;
                case 25:
                    fs << "false";
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
            v = AST::fetchLeaves(r->children.at(0));
            for(int i=0; i<tabs; i++) fs << "\t";
            if(v->size()==1)
                fs << "a" << table->lookup(v->back()->content, v->back()->scope) << ".clear()";
            else
                fs << "a" << table->lookup(v->back()->content, v->back()->scope) << ".erase("
                    << "a" << table->lookup(v->back()->content, v->back()->scope) << ".begin()+"
                    << v->at(1)->content << ")";
            break;
        case 14:
            break;
        case 15:
            for(int i=0; i<tabs; i++) fs << "\t";
            descend(r->children.back(), fs, tabs);
            break;
        case 17:
            break;
        case 19:
            /*???*/
            break;
        case 20:
            v = AST::fetchLeaves(r->children.back());
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "a" << table->lookup(v->back()->content, v->back()->scope) << " "
                << r->children.at(1)->content << " ";
            descend(r->children.at(0), fs, tabs);
            break;
        case 21:
            v = AST::fetchLeaves(r->children.back());
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "auto a" << table->lookup(v->back()->content, v->back()->scope) << " ";
            descend(r->children.at(0), fs, tabs);
            break;
        case 44:
            v = AST::fetchLeaves(r->children.at(2));
            aux = r->children.back()->scope;
            table->insert("__cmpvar__", aux, 7, r->children.back()->tk->lin, r->children.back()->tk->col, 0);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "auto " << "__cmpvar__" << table->lookup("__cmpvar__", aux) << " = ";
            if(v->size()>1) {
                fs << "{";
                for (unsigned i = v->size()-1; i-- > 1; ) fs << v->at(i)->content;
                fs << "};\n";
            } else {
                fs << "a" << table->lookup(v->back()->content, v->back()->scope) << ";\n";
            }
            v->clear();
            v = AST::fetchLeaves(r->children.at(4));
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "for (auto " << "a" << table->lookup(v->back()->content, v->back()->scope)
                << " : __cmpvar__" << table->lookup("__cmpvar__", aux) << ")";
            descend(r->children.at(0), fs, tabs);
            fs << "\n";
            break;
        case 45:
            break;
        case 46:
            v = AST::fetchLeaves(r->children.at(2));
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "auto a" << table->lookup(r->children.at(3)->content, r->children.at(3)->scope) << " = [&]";
            //for (int i = v->size()-2; i > 0; i-=2)
            //    fs << " auto a" << table->lookup(v->at(i)->content)->second[/*v->at(i)->scope*/0] << v->at(i-1)->content;
            for (unsigned i = v->size(); i-- > 1; ) {
                if(v->at(i)->kind==5) {
                    //std::cerr << "To lookup: " << v->at(i-1)->content << " in scope " << v->at(i-1)->scope << "\n";
                    fs << v->at(i)->content << " auto a" << table->lookup(v->at(i-1)->content, v->at(i-1)->scope);
                    i--;
                    //std::cerr << "Success!\n";
                } else {
                    fs << v->at(i)->content;
                }
            }
            fs << ") ";
            descend(r->children.at(0), fs, tabs);
            fs << ";\n";
            break;
        case 49:
            fs << " {\n";
            descend(r->children.back(), fs, tabs+1);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "}";
            break;
        case 50:
            fs << " {\n";
            descend(r->children.at(1), fs, tabs+1);
            for(int i=0; i<tabs; i++) fs << "\t";
            fs << "}";
            break;
        case 61: case 62: case 63: case 64:
            //fs << r->children.back()->content;
            for (unsigned i = r->children.size(); i-- > 0; ) {
                if(r->children.at(i)->kind!=5) {
                    /*std::cerr << r->children.at(i)->content << " ";
                    std::cerr << "\n";*/
                    descend(r->children.at(i), fs, tabs);
                }
            }
            break;
        /*case 64:
            fs << "else";
            descend(r->children.at(0), fs, tabs);
            break;*/
        case 70: case 72: case 73:
            fs << "(";
            for (unsigned i = r->children.size(); i-- > 0; )
                descend(r->children.at(i), fs, tabs);
            fs << ")";
            break;
        /*case 92:
            std::cerr << "92: " << r->children.back()->content << ":" << r->children.back()->scope << "=" << table->lookup(r->children.back()->content, r->children.back()->scope) << "\n";
            fs << "a" << table->lookup(r->children.back()->content, r->children.back()->scope);
            break;*/
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

/*void compiler::Translator::v = AST::fetchLeaves(Node *r, std::vector<Node*> &v) {
    for(auto &i : r->children)
        v = AST::fetchLeaves(i,v);
    if(r->regra==-1) v->push_back(r);
}*/

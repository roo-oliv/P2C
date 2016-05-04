//=================================
// include guard
#ifndef __TRANSLATOR_HPP_INCLUDED__
#define __TRANSLATOR_HPP_INCLUDED__

//=================================
// included dependencies
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "datastructures.hpp"
#include "exception.hpp"

namespace compiler {
    class Translator {
    public:
        Translator(AST*, SymbolTable*);
        ~Translator();
        void translate();
    private:
        void descend(Node*, std::ofstream&, int);
        //void fetchChildren(Node*, std::vector<Node*>&);
        AST *ast;
        SymbolTable *table;
    };
}

#endif // __TRANSLATOR_HPP_INCLUDED__

//=================================
// include guard
#ifndef __LEXYCAL_HPP_INCLUDED__
#define __LEXYCAL_HPP_INCLUDED__

//=================================
// included dependencies
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "datastructures.hpp"

namespace compiler {
    class LexycalAnalyzer {
    public:
        LexycalAnalyzer();
        ~LexycalAnalyzer();
        TokenList* process();
        void createToken(int);
        TokenList* getList();
        int getLine();
        int getColumn();
    private:
        char c, ant;
        bool dot;
        std::string content;
        TokenList *list;
        IndentStack *stack;
        int state, line, column, initial_col, space, indentLevel;
    };
}

#endif // __LEXYCAL_HPP_INCLUDED__

#include <iostream>
#include "datastructures.hpp"
#include "lexycal.hpp"
#include "syntax.hpp"
#include "semantic.hpp"
#include "translator.hpp"

int main() {
    #ifdef DEBUG
        std::cerr << "\n---------!DEBUG MODE ON!---------\n\n";
    #endif
    compiler::LexycalAnalyzer lexycal;
    lexycal.process();
    compiler::Token *t = lexycal.getList()->readHeading()->next;
    compiler::SyntaxAnalyzer syntax;
    syntax.analyze(t);
    compiler::AST *ast = syntax.getAST();
    compiler::SemanticAnalyzer semantic;
    semantic.analyze(ast);
    return 0;
}

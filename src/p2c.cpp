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
    /*compiler::SymbolTable *table = semantic.getSymbolTable();
    table->insert("x", 7, 0, 0, 0);
    table->insert("a", 6, 0, 0, 0);
    table->insert("b", 6, 0, 0, 0);*/
    compiler::Translator translator(ast, semantic.getSymbolTable());
    translator.translate();
    return 0;
}

//=================================
// include guard
#ifndef __SYNTAX_HPP_INCLUDED__
#define __SYNTAX_HPP_INCLUDED__

//=================================
// included dependencies
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stack>
#include "datastructures.hpp"
#include "exception.hpp"

namespace compiler {
    class SyntaxAnalyzer {
    public:
        std::vector<std::string> split(std::string, char);
        int fillTable(std::vector<std::vector<std::string>>&, std::string);
        int fillTable(std::vector<std::vector<int>>&, std::string);
		AST* getAST();
        int analyze(Token*);
		std::vector<std::vector<std::string>>* getActionTable();
		std::vector<std::vector<std::string>>* getGoToTable();
		std::vector<std::vector<int>>* getRulesTable();
    private:
		std::vector<std::vector<std::string>> action; // tabela action
	    std::vector<std::vector<std::string>> go; // tabela goto
	    std::vector<std::vector<int>> rules; // tabela de regras. [n][0] = numero de tokens na parte direita da regra, [n][1] = numero da regra pai (index na tabela - chamei de ruleSet)
		AST ast;
    };
}

#endif // __SYNTAX_HPP_INCLUDED__

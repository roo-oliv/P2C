//=================================
// include guard
#ifndef __SEMANTIC_HPP_INCLUDED__
#define __SEMANTIC_HPP_INCLUDED__

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
    class SemanticAnalyzer {
	public:
		SemanticAnalyzer();
		~SemanticAnalyzer();
		int analyze(AST*);
	private:
		SymbolTable table;
		void decorate(Node*);
		Token descend(Node*);
		int getType(Token*);
		Token concat(std::vector<Token>&);
		int concat(std::vector<Node*>&);
	};
}

#endif // __SEMANTIC_HPP_INCLUDED__

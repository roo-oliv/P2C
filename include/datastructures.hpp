//=================================
// include guard
#ifndef __DATASTRUCTURES_HPP_INCLUDED__
#define __DATASTRUCTURES_HPP_INCLUDED__

//=================================
// included dependencies
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

namespace compiler {
    struct Token;
    struct Indent;
    struct Node;
	struct Symbol;

    struct Token {
        int type, col, lin;	// Tipo, linha e coluna (de inicio) do Token
        std::string lexema;	// Conteúdo do Token
        Token *next, *parent;// Ponteiros para o proximo/anterior Token
    };

    struct Indent {
        int spaces;
        Indent *next;
    };

    struct Node {
        int regra, kind, scope;
        Token *tk;
        std::vector<Node*> children;
        std::string content = "";
        Node *parent;
        std::string getKindName();
    };

    struct Symbol {
        int kind, lin, col, args, scope;
        std::string name;
    };

	typedef std::unordered_map<std::string, std::vector<Symbol*>> HashTable;

	class TokenList {
    public:
        TokenList();
        ~TokenList();
        int push(Token*);
        Token* pop();
        Token* readTrailing();
        Token* readHeading();
        int getSize();
    private:
        Token *heading, *trailing;
        int size;
    };

	class IndentStack {
    public:
        IndentStack();
        ~IndentStack();
        int push(Indent*);
        Indent* pop();
        Indent* getTop();
        int getNElem();
        int getSize();
    private:
        Indent *top;
        int nElem, size;
    };

	class AST {
    public:
        AST();
        ~AST();
        Node* createNode(int);
        Node* createLeaf(Token*);
        Node* addChild(Node*, Node*);
        Node* getRoot();
        void setRoot(Node*);
        static std::vector<Node*> *fetchLeaves(Node*);
    private:
        Node* root;
    };

	class SymbolTable {
	public:
		SymbolTable();
		~SymbolTable();
		HashTable::iterator insert(std::string, int, int, int, int, int);
		//void erase(std::string, int);
		void *lookup(std::string, int);
        std::vector<int> scopes;
	private:
		HashTable table;
	};
}

#endif // __DATASTRUCTURES_HPP_INCLUDED__

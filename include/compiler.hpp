#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctype.h>
#include <vector>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#ifdef DEBUG
	extern int DEBUG_LEVEL, DEBUG_SPECIFIER;
#endif

namespace compiler {
	struct Token;
    struct Indent;
    struct Node;
	struct Symbol;

	typedef std::unordered_map<std::string, std::vector<Symbol*>> HashTable;

	class exception: public std::exception {
	public:
		exception(std::vector<Token>&, std::string);
		virtual const char* what() const throw();
	private:
		std::string message;
	};


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
    private:
        Node* root;
    };

	class SymbolTable {
	public:
		SymbolTable();
		~SymbolTable();
		auto insert(std::string, int, int, std::pair<int, int>, int, std::string);
		void erase(std::string, int);
		auto lookup(std::string);
	private:
		HashTable table;
	};

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

    class SyntaxAnalyzer {
    public:
        std::vector<std::string> split(std::string, char);
        int fillTable(std::vector<std::vector<std::string>>&, std::string);
        int fillTable(std::vector<std::vector<int>>&, std::string);
		AST* getAST();
        int analyze();
    private:
		AST ast;
    };

	class SemanticAnalyzer {
	public:
		SemanticAnalyzer();
		~SemanticAnalyzer();
		int analyze();
	private:
		AST *ast;
		SymbolTable table;
		void decorate(AST*);
		Token descend(Node*);
		int getType(Token*);
		Token concat(std::vector<Token>&);
	};
}

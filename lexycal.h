//Universidade Federal do ABC - Disciplina: Compiladores
//Analisador Léxico
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>

using namespace std;

//Definição da estrutura dos tokens
struct Token {
	int type, col, lin;	// Tipo, linha e coluna (de inicio) do Token
	string lexema;	// Conteúdo do Token
	Token *next, *parent;// Ponteiros para o proximo/anterior Token
};

//Definicao da estrutura de identação
struct Indent {
	int spaces;
	Indent *next;
};

//Lista Encadeada de tokens
class TokenList {
public:
	TokenList() {
		heading = new Token;
		trailing = heading;
		size = 0;
	}
	~TokenList() { }

	//Inserir novos tokens na lista
	int push(Token* t) {
		t->next = NULL;
		trailing->next = t;
		t->parent = trailing;
		trailing = t;
		size++;
		return 0;
	}
	
	//Retirar o último token da lista
	Token* pop() {
		if(size>0){
			Token* t = trailing;
			trailing = trailing->parent;
			trailing->next = NULL;
			t->parent = NULL;
			size--;
			return t;
		}else{
			cout<<"lista vazia"<<endl;
			return NULL;
		}
	}

	//Verificar o ultimo token da lista
	Token* readTrailing() { return trailing; }
	
	//Verificar o primeiro token da lista
	Token* readHeading() { return heading; }
	
	//Quantidade de elementos da lista
	int getSize() { return size; }
	
private:
	Token *heading, *trailing;
	int size;
};

//Pilha de identação
class IndentStack {
public:
	IndentStack() {
		top = NULL;
		nElem = size = 0;
	}
	~IndentStack() {}
	
	//Inserir elementos na pilha
	int push(Indent *i) {
		i->next = top;
		top = i;
		nElem++;
		size += i->spaces;
		return 0;
	}
	
	//Retirar o elemento do topo da pilha
	Indent* pop() {
		if(nElem > 0) {
			Indent* i = top;
			top = top->next;
			nElem--;
			size = size - i->spaces;
			return i;
		}else {
			cout << "ERROR: empty stack!" << endl;
			return NULL;
		}
	}
	
	//Verificar o elemento do topo da pilha
	Indent* getTop() { return top; }
	
	//Verificar a quantidade de elementos da pilha
	int getNElem() { return nElem; }
	
	//Verificar a quantidade de espaços que foram inseridos no total
	int getSize() { return size; }
	
private:
	Indent *top;
	int nElem, size;
};

class Analyzer {
public:
    Analyzer() {
        content = "";
        column = state = initial_col = space = 0;
        line = 1;
        dot = false;
        list = new TokenList();
        stack = new IndentStack();
    }
    ~Analyzer() {
        
    }
    
    //Funcao de leitura e criacao dos tokens
    TokenList* process() {
        while( cin >> noskipws >> c ) {
    		column++;
        	
        	//Transição de estados
        	switch(state) {
        		//Estado 0 - inicial
        		case 0:
		    		//Reconhecimento de operadores
			    	if(content == "/") {
			    		if(c == '/') {
			    			content += c;
			    			createToken(38);
			    			continue;
			    		}else if( c == '=') {
			    			content += c;
			    			createToken(41);
			    			continue;
			    		}else createToken(38);
			    	}else if(content == "=") {
		    			if(c == '=') {
		    				content += c;
		    				createToken(34);
		    				continue;
		    			}else createToken(39);
			    	}else if(content == ">") {
		    			if(c == '=') {
		    				content += c;
		    				createToken(34);
		    				continue;
		    			}else createToken(34);
			    	}else if(content == "<") {
		    			if(c == '=') {
		    				content += c;
		    				createToken(34);
		    				continue;
		    			}else createToken(34);
			    	}else if(content == "!") {
		    			if(c == '=') {
		    				content += c;
		    				createToken(34);
		    				continue;
		    			}else {
		    				content += c;
		    				state = -1;
		    			}
			    	}else if(content == "+") {
		    			if(c == '=') {
		    				content += c;
		    				createToken(41);
		    				continue;
		    			}else createToken(37);
			    	}else if(content == "-") {
		    			if(c == '=') {
		    				content += c;
		    				createToken(41);
		    				continue;
		    			}else createToken(37);
			    	}else if(content == "*") {
		    			if(c == '=') {
		    				content += c;
		    				createToken(41);
		    				continue;
		    			}else if(c == '*') {
		    				content += c;
		    				createToken(9);
		    				continue;
		    			}else createToken(38);
			    	}else if(content == "%") {
		    			if(c == '='){
		    				content += c;
		    				createToken(41);
		    				continue;
		    			}else createToken(38);
			    	}
			    	//Símbolo inicial de identificadores e números
			    	if( isalpha(c) || c == '_' ) {
				    	content += c;
				    	initial_col = column;
                    	state = 11;
			    	}else if ( isdigit(c) ){
				    	content += c;
				    	initial_col = column;
                    	state = 28;
			    	}
			    	break;
				//Estado 11 - identificadores e palavras-chave   
				case 11:
			    	if( isalpha(c) || isdigit(c) || c == '_' ) content += c;
			    	else {
			    		if( c == ' ' || c == ','  || c == '.' ||  c == ':' || c == ';' || 
			    		    c == 012 ||	c == '(' || c == ')' || c == '[' || c == ']') {
			    			//Palavras chave
			    			if(content == "and") createToken(4);
			    			else if(content == "or") createToken(3);
			    			else if(content == "not") createToken(5);
			    			else if(content == "if") createToken(2);
			    			else if(content == "elif") createToken(19);
			    			else if(content == "else") createToken(20);
			    			else if(content == "for") createToken(10);
			    			else if(content == "while") createToken(26);
			    			else if(content == "in") createToken(1);
			    			else if(content == "continue") createToken(36);
			    			else if(content == "break") createToken(22);
			    			else if(content == "return") createToken(30);
			    			else if(content == "def") createToken(8);
			    			else if(content == "False") createToken(25);
			    			else if(content == "True") createToken(15);
			    			else if(content == "None") createToken(12);
			    			else if(content == "del") createToken(7);
			    			else if(content == "from") createToken(21);
			    			else if(content == "global") createToken(32);
			    			else if(content == "nonlocal") createToken(35);
			    			else if(content == "pass") createToken(16);
			    			else if(content == "yield") createToken(27);
			    		}
			    		//Identificadores
			    		if(state != 0)	createToken(state);
			    	}
			    	break;
				//Estado 28 - inteiros e ponto-flutuante 
				case 28:
			    	if( isdigit(c) ) content += c;
			    	else if(c == '.' && dot == false) {
			    		content += c;
			    		dot = true;
			    	}else if( c == ' ' || c == ',' || c == ':' || c == ';' ||  c == '(' || c == ')' ||
			    			  c == '[' || c == ']' ||  c == 012 || c == '+' || c == '-' || c == '*' || 
			    			  c == '/' || c == '%'|| c == '<' || c == '>' || c == '=') {
				    	createToken(state);
                    	dot = false;
			    	}else {
			        	content += c;
				    	state = -1;
			    	}
			    	break;
			    //Identação
			    case 42:
				    if( column == 1) {
		    			if(c == 040) space++;
		    			else if (c == 011) space = space + 4;
		    			else space = 0;
				    }
				    else if(c == 040 && column != 1) space++;
			    	else if(c == 011 && column != 1) space += 4;
		    		if(c != 040 && c != 011) {
		    		    if( space > stack->getSize() ) {
		    		    	Indent *i = new Indent();
		    		    	i->spaces = space - stack->getSize();
		    		    	stack->push(i);
		    		    	stringstream ss;
		    				ss << i->spaces;
		    				content += ss.str();
							createToken(31);
							space = 0;
		    		    }else if( space < stack->getSize() ) {
	    		    		int dif = stack->getSize() - space;
	    		    		while(dif) {
	    		    			Indent *i = stack -> pop();
	    		    			if(i->spaces <= dif) {
	    		    				dif -= i->spaces;
	    		    				cout << i->spaces << endl;
	    		    				stringstream ss;
				    				ss << i->spaces;
				    				content += ss.str();
									createToken(29);
	    		    			}else {
	    		    				Indent *d = new Indent();
				    		    	d->spaces = i -> spaces - dif;
				    		    	stack->push(d);
				    		    	dif = 0;
				    		    	content= "indentation doesn't match";
									createToken(-1);
	    		    			}
	    		    		}
		    		    	space = 0;
		    		    } else {
		    		    	space = 0;
		    		    }
		    		    if( isalpha(c) || c == '_' ) {
					    	content += c;
					    	initial_col = column;
	                    	state = 11;
				    	}else if ( isdigit(c) ){
					    	content += c;
					    	initial_col = column;
	                    	state = 28;
				    	}else state = 0;
		    		}
			    break;
				//Estado -1 - erro     
				case -1:
			    	if( isalpha(c) || isdigit(c) || c == '_' ) content += c;
			    	else if( c == ' ' || c == ',' || c == 012 ) createToken(-1);
			    	break;
		    }
		    //Reconhecimento de delimitadores
		    if(state == 0) {
			    switch(c) {
			    	case '.':
		    			initial_col = column;
		    			content += c;
    					createToken(6);
			    		break;
			    	case ',':
			    		initial_col = column;
			    		content += c;
	    				createToken(24);
			    		break;
			    	case ':':
			    		initial_col = column;
			    		content += c;
			    		createToken(23);
			    		break;
			    	case ';':
			    		initial_col = column;
			    		content += c;
	    				createToken(40);
			    		break;
			    	case '(':
			    		initial_col = column;
			    		content += c;
	    				createToken(17);
			    		break;
			    	case ')':
			    		initial_col = column;
			    		content += c;
	    				createToken(13);
			    		break;
			    	case '[':
			    		initial_col = column;
			    		content += c;
	    				createToken(18);
			    		break;
			    	case ']':
			    		initial_col = column;
			    		content += c;
	    				createToken(14);
			    		break;
			    }
			    //Símbolo inicial de operadores
	        	if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%'||
	        		c == '<' || c == '>' || c == '=' || c == '!') {
					content += c;
					initial_col = column;
	        	}
	        	//Criação do token NEWLINE
	        	if(c==012) {
		    		content = "\\n";
		    		initial_col = column;
		    		createToken(33);
		    		state = 42;
		    		line++;
 					column = 0;
	        	}
		    }
        }
    	if( space < stack->getSize() ) {
    		int dif = stack->getSize() - space;
    		while(dif) {
    			Indent *i = stack -> pop();
    			if(i->spaces <= dif) {
    				dif -= i->spaces;
    				stringstream ss;
    				ss << i->spaces;
    				content += ss.str();
					createToken(29);
    			}else {
    				Indent *d = new Indent();
    		    	d->spaces = i -> spaces - dif;
    		    	stack->push(d);
    		    	dif = 0;
    		    	content= "indentation doesn't match";
					createToken(-1);
    			}
    		}
    	}
    	if(content != "") createToken(state);
    	//$ final
    	content = "$";
    	column++;
    	createToken(42);
    	space = 0;
    }
        
    //Criação do token e inserção na lista
    void createToken(int stt){
        Token* t = new Token;
        t->type = stt;
        t->lexema = content;
        t->col = initial_col;
        t->lin = line;
        list->push(t);
        content = "";
        state = 0;
    }
    
    TokenList* getList() { return list; }
    
    int getLine() { return line; }
    
    int getColumn(){ return column; }
    
private:
    char c, ant;
    bool dot;
    string content;
    TokenList *list;
    IndentStack *stack;
    int state, line, column, initial_col, space, indentLevel;
};
//Universidade Federal do ABC - Disciplina: Compiladores
//Analisador Léxico
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>

using namespace std;

//Definicao da estrutura dos tokens
struct Token {
	int type, col, lin;	// Tipo, linha e coluna (do inicio) do Token
	string lexema;	// Conteudo do Token
	Token *next;	// Ponteiro para o proximo Token
	Token *parent; // Ponteiro para o Token Pai
};

//Lsita Encadeada de tokens
class TokenList {
public:
	TokenList() {
		heading = new Token;
		trailing = heading;
		size = 0;
	}
	~TokenList() {

	}

	//Inserir novos tokens na lista
	int push(Token* t) {
		t->next = NULL;
		trailing->next = t;
		t->parent = trailing;
		trailing = t;
		size++;
		return 0;
	}
	
	//Retirar o ultimo token da lista
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
	Token* readTrailing() {
		return trailing;
	}
	
	Token* readHeading(){
		return heading;
	}
	
	int getSize(){
		return size;
	}
	
	

private:
	Token *heading;
	Token *trailing;
	int size;
};

class Analyzer {
public:
    Analyzer() {
        content = "";
        column = 0;
        line = 1;
        state = 0;
       	initial_col = 0;
        dot = false;
        list = new TokenList();
        indentStack = new TokenList();
    }
    ~Analyzer() {
        
    }
    
    //Funcao de leitura e criacao dos tokens
    TokenList* process() {
    	
        while( cin >> noskipws >> c ) {
        	
        	//Linha e coluna atual
    		column++;
    		
    		//verificação de identação
    		if( (c == ' ' || c == ' ' ) && (column == 1) ){
    			
    		}
        	
        	//Transição de estados
        	switch(state) {
        		//Estado 0 - inicial
        		case 0:
			    	if (content == "/"){
			    		if(c == '/'){
			    			content += c;
			    			state = 38;
			    			createToken();
			    			state = 0;
			    			continue;
			    		}else if( c == '='){
			    			content += c;
			    			state = 41;
			    			createToken();
			    			state = 0;
			    			continue;
			    		}else{
			    			state = 38;
			    			createToken();
			    			state = 0;
			    		}
			    	}else if (content == "="){
			    			if(c == '='){
			    				content += c;
			    				state = 34;
			    				createToken();
			    				state = 0;
			    				continue;
			    			}else{
			    				state = 39;
			    				createToken();
			    				state = 0;
			    			}
			    	}else if (content == ">"){
			    			if(c == '='){
			    				content += c;
			    				state = 34;
			    				createToken();
			    				state = 0;
			    				continue;
			    			}else{
			    				state = 34;
			    				createToken();
			    				state = 0;
			    			}
			    	}else if (content == "<"){
			    			if(c == '='){
			    				content += c;
			    				state = 34;
			    				createToken();
			    				state = 0;
			    				continue;
			    			}else{
			    				state = 34;
			    				createToken();
			    				state = 0;
			    			}
			    	}else if (content == "!"){
			    			if(c == '='){
			    				content += c;
			    				state = 34;
			    				createToken();
			    				state = 0;
			    				continue;
			    			}else{
			    				content += c;
			    				state = -1;
			    			}
			    	}else if (content == "+"){
			    			if(c == '='){
			    				content += c;
			    				state = 41;
			    				createToken();
			    				state = 0;
			    				continue;
			    			}else{
			    				state = 37;
			    				createToken();
			    				state = 0;
			    			}
			    		
			    	}else if (content == "-"){
			    			if(c == '='){
			    				content += c;
			    				state = 41;
			    				createToken();
			    				state = 0;
			    				continue;
			    			}else{
			    				state = 37;
			    				createToken();
			    				state = 0;
			    			}
			    	}else if (content == "*"){
			    			if(c == '='){
			    				content += c;
			    				state = 41;
			    				createToken();
			    				state = 0;
			    				continue;
			    			}else if(c == '*'){
			    				content += c;
			    				state = 9;
			    				createToken();
			    				state = 0;
			    				continue;
			    			}else {
			    				state = 38;
			    				createToken();
			    				state = 0;
			    			}
			    	}else if (content == "%"){
			    			if(c == '='){
			    				content += c;
			    				state = 41;
			    				createToken();
			    				state = 0;
			    				continue;
			    			}else{
			    				state = 38;
			    				createToken();
			    				state = 0;
			    			}
			    	}
			    	
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
				//Estado 1 - identificadores e palavras-chave   
				case 11:
			    	if( isalpha(c) || isdigit(c) || c == '_' ){
				    	content += c;
			    	}else {
			    		if( c == ' ' || c == ','  || c == '.' ||  c == ':' || c == ';' || c == 012 ||
			    			c == '(' || c == ')' || c == '[' || c == ']') {
			    			//Palavras chave
			    			if(content == "and") {
				    			state = 4;
				    			createToken();
				    			state = 0;
			    			}else if(content == "or") {
			    				state = 3;
				    			createToken();
				    			state = 0;
			    			}else if(content == "not") {
			    				state = 5;
			    				createToken();
			    				state = 0;
			    			}else if(content == "if") {
			    				state = 2;
			    				createToken();
			    				state = 0;
			    			}else if(content == "elif") {
			    				state = 19;
			    				createToken();
			    				state = 0;
			    			}else if(content == "else") {
			    				state = 20;
			    				createToken();
			    				state = 0;
			    			}else if(content == "for") {
			    				state = 10;
			    				createToken();
			    				state = 0;
			    			}else if(content == "while") {
			    				state = 26;
			    				createToken();
			    				state = 0;
			    			}else if(content == "in") {
			    				state = 1;
			    				createToken();
			    				state = 0;
			    			}else if(content == "continue") {
			    				state = 36;
			    				createToken();
			    				state = 0;
			    			}else if(content == "break") {
			    				state = 22;
			    				createToken();
			    				state = 0;
			    			}else if(content == "return") {
			    				state = 30;
			    				createToken();
			    				state = 0;
			    			}else if(content == "def") {
			    				state = 8;
			    				createToken();
			    				state = 0;
			    			}else if(content == "False") {
			    				state = 25;
			    				createToken();
			    				state = 0;
			    			}else if(content == "True") {
			    				state = 15;
			    				createToken();
			    				state = 0;
			    			}else if(content == "None") {
			    				state = 12;
			    				createToken();
			    				state = 0;
			    			}else if(content == "del") {
			    				state = 7;
			    				createToken();
			    				state = 0;
			    			}else if(content == "from") {
			    				state = 21;
			    				createToken();
			    				state = 0;
			    			}else if(content == "global") {
			    				state = 32;
			    				createToken();
			    				state = 0;
			    			}else if(content == "nonlocal") {
			    				state = 35;
			    				createToken();
			    				state = 0;
			    			}else if(content == "pass") {
			    				state = 16;
			    				createToken();
			    				state = 0;
			    			}else if(content == "yield") {
			    				state = 27;
			    				createToken();
			    				state = 0;
			    			}
			    			
			    		}
			    		
			    		if(state != 0) {
				    		createToken();
                    		state = 0;
				  		}
			    	}
			    	break;
				//Estado 2 - inteiros e ponto-flutuante 
				case 28:
			    	if( isdigit(c) ){
				    	content += c;
			    	}else if(c == '.' && dot == false) {
			    		content += c;
			    		dot = true;
			    	}else if( c == ' ' || c == ',' | c == ':' || c == ';' ||  c == '(' || c == ')' ||
			    			c == '[' || c == ']' ||  c == 012 || c == '+' || c == '-' || c == '*' || 
			    			c == '/' || c == '%'|| c == '<' || c == '>' || c == '=') {
				    	createToken();
                    	state = 0;
                    	dot = false;
			    	}else {
			        	content += c;
				    	state = -1;
			    	}
			    	break;
				//Estado -1 - erro     
				case -1:
			    	if( isalpha(c) || isdigit(c) || c == '_' ) {
				    	content += c;
			    	}else if( c == ' ' || c == ',' || c == 012 ) {
				    	createToken();
				    	state = 0;
			    	}
			    	break;
			    default:
			    	cout << "Estado invalido" << endl;
		    }
		    
		    if(state == 0) {
			    switch(c){
			    	case '.':
		    			initial_col = column;
		    			content += c;
		    	   		state = 6;
    					createToken();
    					state = 0;

			    		break;
			    	case ',':
			    		initial_col = column;
			    		content += c;
			    	   	state = 24;
	    				createToken();
	    				state = 0;
			    		break;
			    	case ':':
			    		initial_col = column;
			    		content += c;
			    	   	state = 23;
	    				createToken();
	    				state = 0;
			    		break;
			    	case ';':
			    		cout << "teste2" << endl;
			    		initial_col = column;
			    		content += c;
			    	   	state = 40;
	    				createToken();
	    				state = 0;
			    		break;
			    	case '(':
			    		initial_col = column;
			    		content += c;
			    	   	state = 17;
	    				createToken();
	    				state = 0;
			    		break;
			    	case ')':
			    		initial_col = column;
			    		content += c;
			    	   	state = 13;
	    				createToken();
	    				state = 0;
			    		break;
			    	case '[':
			    		initial_col = column;
			    		content += c;
			    	   	state = 18;
	    				createToken();
	    				state = 0;
			    		break;
			    	case ']':
			    		initial_col = column;
			    		content += c;
			    	   	state = 14;
	    				createToken();
	    				state = 0;
			    		break;
	
			    }
			    
	        	if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%'||
	        		c == '<' || c == '>' || c == '=' || c == '!') {
					content += c;
					initial_col = column;
	    			state = 0;
	        	}
	        	
	        	if(c==012){
		    		content = "\\n";
		    		state = 33;
		    		initial_col = column;
		    		createToken();
		    		state = 0;
		    		line++;
 					column = 0;
	        	}

		    }
            	
        }
        	
    	if(content != ""){
 			createToken();
    	}
    	
    	state = 42;
    	content = "$";
    	line++;
    	column = 1;
    	createToken();
		   
    }
        
    //Criacao do token e insercao na lista
    void createToken(){
        Token* t = new Token;
        t->type = state;
        t->lexema = content;
        t->col =initial_col;
        t->lin = line;
        list->push(t);
        content = "";
    }
    
    TokenList* getList(){
    	return list;
    }
    
    int getLine(){
    	return line;
    }
    
    int getColumn(){
    	return column;
    }
    
private:
    char c, ant;
    bool dot;
    string content;
    TokenList *list, *indentStack;
    int state, line, column,initial_col;
};
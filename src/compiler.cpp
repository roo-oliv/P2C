#include <compiler.hpp>

struct compiler::Token {
    int type, col, lin;	// Tipo, linha e coluna (de inicio) do Token
    std::string lexema;	// Conteúdo do Token
    Token *next, *parent;// Ponteiros para o proximo/anterior Token
};

struct compiler::Indent {
    int spaces;
    Indent *next;
};

struct compiler::Node {
    int regra, kind, scope;
    Token *tk;
    std::vector<Node*> children;
    std::string content = "";
    Node *parent;
};

struct compiler::Symbol {
    int kind, type, scope;
    std::pair<int, int> pos; // position = (line, column)
    std::string name, content;
};

compiler::exception::exception(Node &root, std::string specification) {
    std::stringstream ss;
    bool flag = true;
    formExpression(root, ss, flag);
    ss << "\n" << specification << std::endl;
    message = ss.str();
}

const char* compiler::exception::what() const throw() {
    return message.c_str();
}

void compiler::exception::formExpression(Node &root, std::stringstream &ss, bool &empty) {
    if(root.children.empty()) {
        if(empty) {
            ss << "Error found in expression at line " << root.tk->lin << ", column " << root.tk->col << ":\n ... ";
            empty = false;
        }
        ss << root.tk->lexema;
        if(root.tk->lexema!="[")
            ss << " ";
    } else {
        for (unsigned i = root.children.size(); i-- > 0; )
            formExpression(*(root.children[i]), ss, empty);
    }
}

compiler::TokenList::TokenList() {
    heading = new Token;
    trailing = heading;
    size = 0;
}

compiler::TokenList::~TokenList() { }

//Inserir novos tokens na lista
int compiler::TokenList::push(Token* t) {
    t->next = NULL;
    trailing->next = t;
    t->parent = trailing;
    trailing = t;
    size++;
    return 0;
}

//Retirar o último token da lista
compiler::Token* compiler::TokenList::pop() {
    if(size>0){
        Token* t = trailing;
        trailing = trailing->parent;
        trailing->next = NULL;
        t->parent = NULL;
        size--;
        return t;
    }else{
        std::cout<<"lista vazia"<<std::endl;
        return NULL;
    }
}

//Verificar o ultimo token da lista
compiler::Token* compiler::TokenList::readTrailing() { return trailing; }

//Verificar o primeiro token da lista
compiler::Token* compiler::TokenList::readHeading() { return heading; }

//Quantidade de elementos da lista
int compiler::TokenList::getSize() { return size; }

compiler::IndentStack::IndentStack() {
    top = NULL;
    nElem = size = 0;
}

compiler::IndentStack::~IndentStack() {}

//Inserir elementos na pilha
int compiler::IndentStack::push(Indent *i) {
    i->next = top;
    top = i;
    nElem++;
    size += i->spaces;
    return 0;
}

//Retirar o elemento do topo da pilha
compiler::Indent* compiler::IndentStack::pop() {
    if(nElem > 0) {
        Indent* i = top;
        top = top->next;
        nElem--;
        size = size - i->spaces;
        return i;
    }else {
        std::cout << "ERROR: empty stack!" << std::endl;
        return NULL;
    }
}

//Verificar o elemento do topo da pilha
compiler::Indent* compiler::IndentStack::getTop() { return top; }

//Verificar a quantidade de elementos da pilha
int compiler::IndentStack::getNElem() { return nElem; }

//Verificar a quantidade de espaços que foram inseridos no total
int compiler::IndentStack::getSize() { return size; }

compiler::AST::AST(){
    Node* root = new Node;
}
compiler::AST::~AST() {}

compiler::Node* compiler::AST::createNode(int regra){
    Node *newNode = new Node;
    newNode->regra = regra;
    newNode->tk = NULL;
    newNode->parent = root;
    return newNode;
}

compiler::Node* compiler::AST::createLeaf(Token* tk){
    Node *newNode = new Node;
    newNode->tk = tk;
    newNode->regra = -1;
    newNode->parent = root;
    return newNode;
}

compiler::Node* compiler::AST::addChild(Node* parent, Node* child){
    parent->children.push_back(child);
    child->parent = parent;
    return parent;
}

compiler::Node* compiler::AST::getRoot(){
    return root;
}

void compiler::AST::setRoot(Node* nroot){
    root = nroot;
}

compiler::SymbolTable::SymbolTable() { }

compiler::SymbolTable::~SymbolTable() { }

auto compiler::SymbolTable::insert(std::string name, int kind, int type, std::pair<int, int> pos, int scope, std::string content) {
    Symbol *s = new Symbol;
    s->name = name;
    s->kind = kind;
    s->type = type;
    s->pos = pos;
    s->scope = scope;
    s->content = content;
    auto it = table.find(name);
    if(it==table.end()) {
        std::vector<Symbol*> v = {s};
        return table.insert(it, HashTable::value_type(name, v));
    } else {
        it->second.push_back(s);
        return it;
    }
}

void compiler::SymbolTable::erase(std::string name, int scope) {
    auto it = table.find(name);
    if(it!=table.end()) {
        for(auto i = it->second.begin(); i != it->second.end(); ++i) {
            if((*i)->scope==scope) {
                it->second.erase(i);
                break;
            }
        }
        if(it->second.empty()) table.erase(it);
    }
}

auto compiler::SymbolTable::lookup(std::string name) { return table.find(name); }

compiler::LexycalAnalyzer::LexycalAnalyzer() {
    content = "";
    column = state = initial_col = space = 0;
    line = 0;
    dot = false;
    list = new TokenList();
    stack = new IndentStack();
}

compiler::LexycalAnalyzer::~LexycalAnalyzer() {

}

//Funcao de leitura e criacao dos tokens
compiler::TokenList* compiler::LexycalAnalyzer::process() {
    #ifdef DEBUG
        if(DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==1) {
            printf("\n\n%s\n\n", "---------LEXYCAL ANALYZER---------");
        }
    #endif
    while( std::cin >> std::noskipws >> c ) {
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
                        std::stringstream ss;
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
                                #ifdef DEBUG
                                    if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==1) && DEBUG_LEVEL>1) {
                                        std::cerr << "Identation spaces: " << i->spaces << std::endl;
                                    }
                                #endif
                                std::stringstream ss;
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
    //newline final
    content = "\\n";
    column++;
    createToken(33);
    space = 0;
    if( space < stack->getSize() ) {
        int dif = stack->getSize() - space;
        while(dif) {
            Indent *i = stack -> pop();
            if(i->spaces <= dif) {
                dif -= i->spaces;
                std::stringstream ss;
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
void compiler::LexycalAnalyzer::createToken(int stt){
    Token* t = new Token;
    t->type = stt;
    t->lexema = content;
    t->col = initial_col;
    t->lin = line;
    list->push(t);
    content = "";
    state = 0;
}

compiler::TokenList* compiler::LexycalAnalyzer::getList() { return list; }

int compiler::LexycalAnalyzer::getLine() { return line; }

int compiler::LexycalAnalyzer::getColumn(){ return column; }

std::vector<std::string> compiler::SyntaxAnalyzer::split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str);
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }
    return internal;
}

int compiler::SyntaxAnalyzer::fillTable(std::vector<std::vector<std::string>> &table, std::string path) {
    std::string line;
    std::ifstream file(path);
    if(file.is_open()) {
        while ( getline(file,line)) {
            std::vector<std::string> tableLine = split(line, ';');
            table.push_back(tableLine);
        }
        file.close();
        return 0;
    } else {
        throw std::runtime_error("Error while opening "+path);
    }
}

int compiler::SyntaxAnalyzer::fillTable(std::vector<std::vector<int>> &table, std::string path) {
    std::string line;
    std::ifstream file(path);
    if (file.is_open()) {
        int ruleSet = 0;
        int count = 0;
        std::string temp = "S";
        while ( getline(file, line) ) {
            std::vector<std::string> tableLine = split(line, ';');
            std::vector<std::string> ruleTokens = split(tableLine[1], ' ');
            std::string ruleName = tableLine[0];
            if(ruleName != temp) {
                temp = ruleName;
                ruleSet++;
            }
            std::vector<int> rule;
            rule.push_back(ruleTokens.size());
            rule.push_back(ruleSet);
            table.push_back(rule);
            #ifdef DEBUG
                if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>2) {
                    printf("(%d) %s -> ", count, ruleName.c_str());
                    for(int i = 0; i < ruleTokens.size(); i++) {
                        printf("%s ", ruleTokens[i].c_str());
                    }
                    printf("%s\n", "");
                }
            #endif
            count++;
        }
        file.close();
        return 0;
    } else {
        throw std::runtime_error("Error while opening "+path);
    }
}

int compiler::SyntaxAnalyzer::analyze() {
    LexycalAnalyzer la;
    la.process();
    Token* t =  la.getList()->readHeading()->next;

    #ifdef DEBUG
        if(DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) {
            printf("\n\n%s\n\n", "Loading tables...");
        }
    #endif
    // Fill action and goto tables from CSV tables
    try {
        fillTable(action, "../include/action.csv");
        fillTable(go, "../include/goto.csv");
        fillTable(rules, "../include/regras.csv");
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    #ifdef DEBUG
        if(DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) {
            printf("\n\n%s\n\n", "Tables successfuly loaded!");
            printf("\n%s\n\n", "--------SINTAX ANALYSER--------");
        }
    #endif
    // inicia a pilha de estados
    std::stack<int> states;
    int state;
    std::stack<Node*> nodes;
    states.push(0);
    int c = 0;
    // analizador sintático
    while(1) {
        c++;
        //define estado atual como o estado do topo da pilha
        state = states.top();
        #ifdef DEBUG
            if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>1) {
                printf("Estado: %d, Token atual: %d (%s), ",state,t->type-1,t->lexema.c_str());
            }
        #endif
        //pega a action atual na tabela a partir do estado e do token
        std::string action_now = action[state][t->type-1];
        #ifdef DEBUG
            if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>1) {
                printf("Action: %s\n", action_now.c_str());
            }
        #endif

        if(action_now!="") {
            //caso SHIFT
            if(action_now.at(0)=='S'){
                #ifdef DEBUG
                    if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>1) {
                        printf("%s", "[SHIFT] -> ");
                    }
                #endif
                int newstate;
                sscanf(action_now.c_str(), "S%d", &newstate); // le o novo estado
                states.push(newstate); // coloca o novo estado no topo da pilha
                Node* leaf = ast.createLeaf(t);
                nodes.push(leaf);
                t = t->next; // passa pro proximo token
                #ifdef DEBUG
                    if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>1) {
                        printf("Novo Estado: %d\n",newstate);
                    }
                #endif
            } else if(action_now.at(0)=='R') {
                #ifdef DEBUG
                    if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>1) {
                        printf("%s", "[REDUCE] -> ");
                    }
                #endif
                int ruleToReduce;
                sscanf(action_now.c_str(), "R%d", &ruleToReduce); // le o index da regra pra aplicar reduce
                Node *parent = ast.createNode(ruleToReduce);
                for( int i = 0; i < rules[ruleToReduce][0]; i++ ) {
                    states.pop(); // retira da pilha os tokens da regra
                    Node* child = nodes.top();
                    ast.addChild(parent, child);
                    nodes.pop();
                }
                state = states.top(); // pega o novo estado do topo da pilha
                #ifdef DEBUG
                    if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>1) {
                        printf("Regra a Reduzir: %d, Numero de tokens da regra: %d, Ruleset: %d , Novo estado pos-pop: %d, ", ruleToReduce, rules[ruleToReduce][0], rules[ruleToReduce][1], state);
                    }
                #endif
                int newstate_r;
                sscanf(go[state][rules[ruleToReduce][1]].c_str(), "%d", &newstate_r);
                #ifdef DEBUG
                    if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>1) {
                        printf("Goto: %s\n", go[state][rules[ruleToReduce][1]].c_str());
                    }
                #endif
                states.push(newstate_r); // coloca o novo estado no topo da pilha
                nodes.push(parent);
            } else if(action_now == "acc") {
                ast.setRoot(nodes.top());
                #ifdef DEBUG
                    if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>0) {
                        std::cout << "\nInput is syntatically correct.\n\n";
                    }
                #endif
                break;
            }
        } else {
            if(t->type==33) {
                t = t->next;
            } else {
                std::cout << "\nSyntax error detected.\n";
                break;
            }
        }
    }

    #ifdef DEBUG
        if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==2) && DEBUG_LEVEL>0) {
            std::vector<Node*> queue;
            queue.push_back(ast.getRoot());
            int n = queue.size();
            int s = queue.size()-1;
            int old_n = 0;
            for(int i = 0; i < n; i++) {
                for(int k = old_n; k < n; k++){
                    if(queue[k]->regra!=-1) {
                        printf("R:%d ", queue[k]->regra);
                    } else {
                        if(DEBUG_LEVEL<2)
                            printf("T:%d ", queue[k]->tk->type);
                        else
                            std::cout << "T:" << queue[k]->tk->lexema << " ";
                    }
                    if(k==n-1) {
                        printf("%s", "| ");
                    }
                    if(k==s) {
                        printf("%s\n","");
                    }
                }
                std::vector<Node*> children = queue[i]->children;
                if(!children.empty()){
                    int m = children.size();
                    for(int j = m-1; j >= 0; j--) {
                        queue.push_back(children[j]);
                    }
                }
                old_n = n;
                n = queue.size();
                if(i==s) {
                    s = n-1;
                }
            }
        }
    #endif

    return 0;
}

compiler::AST* compiler::SyntaxAnalyzer::getAST() {return &ast;}

std::vector<std::vector<std::string>>* compiler::SyntaxAnalyzer::getActionTable() {return &action;}
std::vector<std::vector<std::string>>* compiler::SyntaxAnalyzer::getGoToTable() {return &go;}
std::vector<std::vector<int>>* compiler::SyntaxAnalyzer::getRulesTable() {return &rules;}

compiler::SemanticAnalyzer::SemanticAnalyzer() {}

compiler::SemanticAnalyzer::~SemanticAnalyzer() {}

int compiler::SemanticAnalyzer::analyze() {
    compiler::SyntaxAnalyzer sa;
    sa.analyze();
    ast = sa.getAST();
    Node *root = ast->getRoot();
    std::stringstream ss;
    exception *e;
    try {
        #ifdef DEBUG
            if(DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==3) {
                std::cerr << "\n\n---------SEMANTIC ANALYZER---------\n\n";
            }
        #endif
        decorate(root);
        /*Token resultant = descend(root);
        if(resultant.type==30 || resultant.type==22 || resultant.type==36) {
            std::vector<Token> expression = {resultant};
            ss << "Unable to handle expression.";
            e = new exception(expression, ss.str());
            throw *e;
        }*/
    } catch (compiler::exception &e) {
		std::cerr << e.what() << std::endl;
        return 1;
	}
}

void compiler::SemanticAnalyzer::decorate(Node *root) { // Decorate tree with node kinds and content
    for(auto &i : root->children)   // Descend tree to decorate children first
        decorate(i);
    if(root->tk) {  // if node has a token it's a leaf node
        #ifdef DEBUG
            if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==3) && DEBUG_LEVEL>1) {
                std::cerr << "decorating leaf node of token: " << root->tk->lexema << std::endl;
            }
        #endif
        switch (root->tk->type) { // map each token type to a node kind
            case 1:
                root->kind = 2;
                break;
            case 2:
                root->kind = 15;
                break;
            case 3:
                root->kind = 1;
                break;
            case 4:
                root->kind = 1;
                break;
            case 5:
                root->kind = 0;
                break;
            case 6:
                root->kind = 5;
                break;
            case 7:
                root->kind = 0;
                break;
            case 8:
                root->kind = 18;
                break;
            case 9:
                root->kind = 1;
                break;
            case 10:
                root->kind = 17;
                break;
            case 11:
                root->kind = 20; /*!!!!!*/
                break;
            case 12:
                root->kind = 14;
                break;
            case 13:
                root->kind = 5;
                break;
            case 14:
                root->kind = 5;
                break;
            case 15:
                root->kind = 10;
                break;
            case 16:
                root->kind = 19;
                break;
            case 17:
                root->kind = 5;
                break;
            case 18:
                root->kind = 5;
                break;
            case 19:
                root->kind = 15;
                break;
            case 20:
                root->kind = 16;
                break;
            case 21:
                root->kind = -1;
                break;
            case 22:
                root->kind = 4;
                break;
            case 23:
                root->kind = 5;
                break;
            case 24:
                root->kind = 5;
                break;
            case 25:
                root->kind = 10;
                break;
            case 26:
                root->kind = 15;
                break;
            case 27:
                root->kind = 4;
                break;
            case 28:
                root->kind = 9;
                break;
            case 29:
                root->kind = 13;
                break;
            case 30:
                root->kind = 4;
                break;
            case 31:
                root->kind = 13;
                break;
            case 32:
                root->kind = 3;
                break;
            case 33:
                root->kind = 13;
                break;
            case 34:
                root->kind = 1;
                break;
            case 35:
                root->kind = 3;
                break;
            case 36:
                root->kind = 4;
                break;
            case 37:
                root->kind = 20; /*!!!!!!*/
                break;
            case 38:
                root->kind = 1;
                break;
            case 39:
                root->kind = 2;
                break;
            case 40:
                root->kind = 5;
                break;
            case 41:
                root->kind = 2;
                break;
        }
        root->content += root->tk->lexema; // fill the node content with its token lexema
    } else {
        for (unsigned i = root->children.size(); i-- > 0; ) // fill the node content with its children contents
            root->content += root->children[i]->content;
        #ifdef DEBUG
            if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==3) && DEBUG_LEVEL>1 && (root->children.size()>1 || DEBUG_LEVEL>2)) {
                std::cerr << "decorating node of children: ";
                for (unsigned i = root->children.size(); i-- > 0; )
                    std::cerr << root->children[i]->content << " ";
                std::cerr << std::endl;
            }
        #endif
        switch (root->children.size()) { // initial break down of simple expressions up to 3 elements and more complex ones
            case 1:
                root->kind = root->children[0]->kind;
                break;
            case 2:
                if(root->children[1]->kind==20)
                    root->children[1]->kind = 0;
                root->kind = concat(root->children);
                if(root->kind==21)
                    root->content = "";
                break;
            case 3:
                if(root->children[1]->kind==20)
                    root->children[1]->kind = 0;
                root->kind = concat(root->children);
                break;
            default:
                root->kind = concat(root->children);
                break;
        }
    }
    #ifdef DEBUG
        if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==3) && DEBUG_LEVEL>1 && (root->tk || (root->children.size()>1 || DEBUG_LEVEL>2))) {
            std::cerr << "\tdecorated with kind: ";
            switch (root->kind) {
                case 0:
                    std::cerr << "unary operator\n";
                    break;
                case 1:
                    std::cerr << "binary operator\n";
                    break;
                case 2:
                    std::cerr << "assignment operator\n";
                    break;
                case 3:
                    std::cerr << "scope operator\n";
                    break;
                case 4:
                    std::cerr << "flow control\n";
                    break;
                case 5:
                    std::cerr << "delimiter\n";
                    break;
                case 6:
                    std::cerr << "parameter\n";
                    break;
                case 7:
                    std::cerr << "variable\n";
                    break;
                case 8:
                    std::cerr << "function\n";
                    break;
                case 9:
                    std::cerr << "number\n";
                    break;
                case 10:
                    std::cerr << "boolean\n";
                    break;
                case 11:
                    std::cerr << "tuple\n";
                    break;
                case 12:
                    std::cerr << "list\n";
                    break;
                case 13:
                    std::cerr << "indentation\n";
                    break;
                case 14:
                    std::cerr << "none\n";
                    break;
                case 15:
                    std::cerr << "conditional\n";
                    break;
                case 16:
                    std::cerr << "else\n";
                    break;
                case 17:
                    std::cerr << "for\n";
                    break;
                case 18:
                    std::cerr << "def\n";
                    break;
                case 19:
                    std::cerr << "pass\n";
                    break;
                case 20:
                    std::cerr << "pending\n";
                    break;
                case 21:
                    std::cerr << "whole expression\n";
                    break;
                default:
                    std::cerr << "invalid\n";
                    break;
            }
        }
    #endif
}

int compiler::SemanticAnalyzer::concat(std::vector<Node*> &expression) {
    std::stringstream ss;
    exception *e;
    #ifdef DEBUG
        if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==3) && DEBUG_LEVEL>1) {
            std::cerr << "Concatenating children from node of rule " << expression[0]->parent->regra << std::endl;
        }
    #endif
    switch (expression.size()) {
        case 2:
            switch(expression[0]->kind) {
                case 7: case 8: case 9: case 10:
                    return expression[0]->kind;
                case 5:
                    if(expression[1]->kind==5) {
                        switch (expression[0]->tk->type) {
                            case 13:
                                return 11;
                            case 14:
                                return 12;
                            default:
                                ss << "UnknownError: Semantic Analyzer received an unexpected expression";
                                e = new exception(*(expression[0]->parent), ss.str());
                                throw *e;
                        }
                    }
                    ss << "UnknownError: Semantic Analyzer received an unexpected expression";
                    e = new exception(*(expression[0]->parent), ss.str());
                    throw *e;
                case 13:
                    return 21;
                case 21:
                    return expression[1]->kind;
                default:
                    ss << "TypeError: bad operand type for unary " << expression[1]->tk->lexema << ": ";
                    switch(expression[0]->kind) {
                        case 11:
                            ss << "\'PyTupleObject\'";
                            break;
                        case 12:
                            ss << "\'PyListObject\'";
                            break;
                        case 14:
                            ss << "\'NoneType\'";
                            break;
                        default:
                            ss << "UnknownError: Semantic Analyzer received an unexpected expression";
                            break;
                    }
                    e = new exception(*(expression[0]->parent), ss.str());
                    throw *e;
            }
        case 3:
            if(expression[0]->kind==5) {
                switch (expression[0]->tk->type) {
                    case 13:
                        return expression[1]->kind;
                    case 14:
                        return 12;
                    default:
                        ss << "UnknownError: Semantic Analyzer received an unexpected expression";
                        e = new exception(*(expression[0]->parent), ss.str());
                        throw *e;
                }
            }
            /*switch (expression) {
                case :
            }*/
            return -1;
        default:
            return -1;
    }
}

/*compiler::Token compiler::SemanticAnalyzer::descend(Node* root) {
    if(root->regra==-1) return *(root->tk);
    std::vector<Token> expression;
    for(auto &i : root->children) {
        expression.push_back(descend(i));
    }
    return concat(expression);
}

compiler::Token compiler::SemanticAnalyzer::concat(std::vector<Token> &expression) {
    #ifdef DEBUG
        if((DEBUG_SPECIFIER==0 || DEBUG_SPECIFIER==3) && DEBUG_LEVEL>1) {
            if(expression.size()>1) {
                std::cerr << "Concatenating expression: ";
                for(auto &i : expression) {
                    std::cerr << i.lexema << "(" << i.type << ")" << " ";
                }
                std::cerr << std::endl;
            }
        }
    #endif
    std::stringstream ss;
    exception *e;
    switch(expression.size()) {
        case 1:
            return expression[0];
        case 2:
            switch(expression[1].type) {
                case 37: // '+' or '-'
                    switch (expression[0].type) {
                        case 17: // '(' indicates PyTupleObject
                            ss << "SemanticException: Unary operator \'" << expression[1].lexema
                               << "\' doesn\'t expect operand of type PyTupleObject.";
                            e = new exception(expression, ss.str());
                            throw *e;
                        case 18: // '[' indicates PyListObject
                            ss << "SemanticException: Unary operator \'" << expression[1].lexema
                               << "\' doesn\'t expect operand of type PyListObject.";
                            e = new exception(expression, ss.str());
                            throw *e;
                        default:
                            return expression[0];
                    }
                default:
                    if(expression[0].type == 33 || // "\n"
                       expression[1].type == 30 || // "return"
                       (expression[1].type == 33 &&
                           !(expression[0].type==30 || expression[0].type==22 || expression[0].type==36)))
                        return expression[1];
                    ss << "SemanticException: Can't handle operation between \'" << expression[1].lexema
                       << " and " << expression[0].lexema << ".";
                    e = new exception(expression, ss.str());
                    throw *e;
            }
        case 3:
            switch (expression[1].type) {
                case 37: // '+' or '-'
                    if(expression[1].lexema=="+") {
                        if(expression[0].type==18 && expression[2].type==18) {
                            return expression[0];
                        } else if((expression[0].type!=18 && expression[2].type!=18) && (expression[0].type!=17 && expression[2].type!=17)) {
                            return expression[0];
                        } else {
                            ss << "SemanticException: Operator \'+\' doesn\'t expect operands of given types.";
                            e = new exception(expression, ss.str());
                            throw *e;
                        }
                    } else {
                        if((expression[0].type!=18 && expression[2].type!=18) && (expression[0].type!=17 && expression[2].type!=17)) {
                            return expression[0];
                        } else {
                            ss << "SemanticException: Operator \'-\' doesn\'t expect operands of given types.";
                            e = new exception(expression, ss.str());
                            throw *e;
                        }
                    }
                case 38: // '*', '/' or '%'
                    if(expression[0].type==18 || expression[1].type==18 || expression[0].type==17 || expression[1].type==17) {
                        ss << "SemanticException: Operator \'" << expression[1].lexema << "\' doesn\'t expect operands of given types.";
                        e = new exception(expression, ss.str());
                        throw *e;
                    }
                    return expression[0];
                case 34: // '>', '<', "!=", "==", ">=" or "<="
                    if(expression[0].type==18 && expression[2].type==18) {
                        return expression[0];
                    } else if((expression[0].type!=18 && expression[2].type!=18) && (expression[0].type!=17 && expression[2].type!=17)) {
                        return expression[0];
                    } else {
                        ss << "SemanticException: Operator \'" << expression[1].lexema << "\' doesn\'t expect operands of given types.";
                        e = new exception(expression, ss.str());
                        throw *e;
                    }
                case 39: // '='

                case 41: // "+=", "-=", "*=", "/=" or "%="

                case 9: // "**"

                default:
                    return expression[2];
            }
        default:
            switch (expression[0].type) {
                case 29: // Indent
                    return expression[1];
                case 30: // "return"
                    if(expression.back().type==8) {
                        return expression.end()[-2];
                    } else {
                        ss << "SemanticException: Unable to handle \'" << expression[0].lexema << "\'";
                        e = new exception(expression, ss.str());
                        throw *e;
                    }
                default:
                ss << "SemanticException: Unable to handle expression.";
                e = new exception(expression, ss.str());
                throw *e;
            }
    }
}*/

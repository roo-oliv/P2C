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
    int regra;
    Token *tk;
    std::vector<Node*> children;
    Node *parent;
};

struct compiler::Symbol {
    std::vector<std::pair<Token*, int>> tokens;
};

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

/*std::pair<std::unordered_map<std::string, compiler::Symbol*>::iterator,bool> compiler::SymbolTable::addEntry(Token &token, int scope) {
    this.find(token->lexema);
    Symbol *s = new Symbol;
    s->tokens.emplace_back(&token, scope);
    return table.insert(s->tokens, s);
}

void compiler::SymbolTable::removeEntry(std::string key) { table.erase(key); }

std::unordered_map<std::string, compiler::Symbol*>::iterator compiler::SymbolTable::find(std::string key) { table.find(key); }*/

compiler::LexycalAnalyzer::LexycalAnalyzer() {
    content = "";
    column = state = initial_col = space = 0;
    line = 1;
    dot = false;
    list = new TokenList();
    stack = new IndentStack();
}

compiler::LexycalAnalyzer::~LexycalAnalyzer() {

}

//Funcao de leitura e criacao dos tokens
compiler::TokenList* compiler::LexycalAnalyzer::process() {
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
                                std::cout << i->spaces << std::endl;
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
        throw std::runtime_error("Error while opening CSV");
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
                printf("(%d) %s -> ", count, ruleName.c_str());
                for(int i = 0; i < ruleTokens.size(); i++) {
                    printf("%s ", ruleTokens[i].c_str());
                }
                printf("%s\n", "");
                count++;
            #endif
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

    std::vector<std::vector<std::string>> action; // tabela action
    std::vector<std::vector<std::string>> go; // tabela goto
    std::vector<std::vector<int>> rules; // tabela de regras. [n][0] = numero de tokens na parte direita da regra, [n][1] = numero da regra pai (index na tabela - chamei de ruleSet)

    #ifdef DEBUG
        printf("\n\n%s\n\n", "--------------DEBUG MODE ON----------------");
        printf("\n\n%s\n\n", "Loading tables...");
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
        printf("\n\n%s\n\n", "Tables successfuly loaded!");
        printf("\n%s\n\n", "--------------SINTAX ANALYSER----------------");
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
            printf("Estado: %d, Token atual: %d (%s), ",state,t->type-1,t->lexema.c_str());
        #endif
        //pega a action atual na tabela a partir do estado e do token
        std::string action_now = action[state][t->type-1];
        #ifdef DEBUG
            printf("Action: %s\n", action_now.c_str());
        #endif

        if(action_now!="") {
            //caso SHIFT
            if(action_now.at(0)=='S'){
                #ifdef DEBUG
                    printf("%s", "[SHIFT] -> ");
                #endif
                int newstate;
                sscanf(action_now.c_str(), "S%d", &newstate); // le o novo estado
                states.push(newstate); // coloca o novo estado no topo da pilha
                Node* leaf = ast.createLeaf(t);
                nodes.push(leaf);
                t = t->next; // passa pro proximo token
                #ifdef DEBUG
                    printf("Novo Estado: %d\n",newstate);
                #endif
            } else if(action_now.at(0)=='R') {
                #ifdef DEBUG
                    printf("%s", "[REDUCE] -> ");
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
                    printf("Regra a Reduzir: %d, Numero de tokens da regra: %d, Ruleset: %d , Novo estado pos-pop: %d, ", ruleToReduce, rules[ruleToReduce][0], rules[ruleToReduce][1], state);
                #endif
                int newstate_r;
                sscanf(go[state][rules[ruleToReduce][1]].c_str(), "%d", &newstate_r);
                #ifdef DEBUG
                    printf("Goto: %s\n", go[state][rules[ruleToReduce][1]].c_str());
                #endif
                states.push(newstate_r); // coloca o novo estado no topo da pilha
                nodes.push(parent);
            } else if(action_now == "acc") {
                ast.setRoot(nodes.top());
                std::cout << "\n!!!! Entrada aceita !!!!!\n\n";
                break;
            }
        } else {
            if(t->type==33) {
                t = t->next;
            } else {
                std::cout << "\n---Erro de sintaxe detectado---\n";
                break;
            }
        }
    }

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
                printf("T:%d ", queue[k]->tk->type);
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

    return 0;
}

compiler::AST* compiler::SyntaxAnalyzer::getAST() {return &ast;}

compiler::SemanticAnalyzer::SemanticAnalyzer() {}

compiler::SemanticAnalyzer::~SemanticAnalyzer() {}

int compiler::SemanticAnalyzer::analyze() {
    compiler::SyntaxAnalyzer sa;
    sa.analyze();
    ast = sa.getAST();
    Node *root = ast->getRoot();
    try {
        #ifdef DEBUG
            std::cerr << "\n\n----------Starting semantic alasys----------\n";
        #endif
        descend(root);
    } catch (char const* exception) {
        std::cerr << exception;
        return 1;
    }
}

compiler::Token compiler::SemanticAnalyzer::descend(Node* root) {
    if(root->regra==-1) return *(root->tk);
    std::vector<Token> expression;
    for(auto &i : root->children) {
        expression.push_back(descend(i));
    }
    return concat(expression);
}

compiler::Token compiler::SemanticAnalyzer::concat(std::vector<Token> &expression) {
    #ifdef DEBUG
        if(expression.size()>1) {
            std::cerr << "Concatenating expression: ";
            for(auto &i : expression) {
                std::cerr << i.lexema << "(" << i.type << ")" << " ";
            }
            std::cerr << std::endl;
        }
    #endif
    switch(expression.size()) {
        case 1:
            return expression[0];
        case 2:
            switch(expression[0].type) {
                case 37: // '+' or '-'
                    switch (expression[1].type) {
                        case 17: // '(' indicates PyTupleObject
                            throwSemanticException(expression);
                        case 18: // '[' indicates PyListObject
                            throwSemanticException(expression);
                        default:
                            return expression[1];
                    }
                default:
                    if(expression[0].type == 33)
                        return expression[1];
                    throw("Unknown error.\n");
            }
        case 3:
            switch (expression[1].type) {
                case 37:
                    if(expression[1].lexema=="+") {
                        if(expression[0].type==18 && expression[2].type==18)
                            return expression[0];
                        else if((expression[0].type!=18 && expression[2].type!=18) && (expression[0].type!=17 && expression[2].type!=17))
                            return expression[0];
                        else
                            throwSemanticException(expression);
                    } else {
                        if((expression[0].type!=18 && expression[2].type!=18) && (expression[0].type!=17 && expression[2].type!=17))
                            return expression[0];
                        else
                            throwSemanticException(expression);
                    }
                case 38:
                    return expression[0];
                default:
                    return expression[2];
            }
        default:
            throw("Unknown error.\n");
    /*switch (operatorType) {
        case -1:
            return childrenTypes[0]; // PODE DAR ERRO
        case 38: // '*', '/' and '%''
            if(childrenTypes.size()!=2) throw std::runtime_error("Invalid number of operands");
            for(int i=0; i<2; i++)
                if(!(childrenTypes[i]==28 || childrenTypes[i]==15 || childrenTypes[i]==25))
                    throw std::runtime_error("Invalid type of operands for operator of type "+operatorType);
            return 28;
        case 37: // '+' and '-'
            if(childrenTypes.size()>2) throw std::runtime_error("Invalid number of operands");
            if(childrenTypes.size()==1 && !(childrenTypes[0]==28 || childrenTypes[0]==15 || childrenTypes[0]==25))
                throw std::runtime_error("Invalid type of operand for operator of type"+operatorType);
            if(!(childrenTypes[0]==18 && childrenTypes[1]==18) ||
                (!(childrenTypes[0]==28 || childrenTypes[0]==15 || childrenTypes[0]==25) &&
                !(childrenTypes[1]==28 || childrenTypes[1]==15 || childrenTypes[1]==25)))
            return childrenTypes[0];
        case 4: // 'and'
            break;
        case 3: // 'or'
            break;
        case 5: // 'not'
            break;
        case 36: // 'continue'
            break;
        case 22: // 'break'
            break;
        case 30: // 'return'
            break;
        default:
            break;*/
    }
}

void compiler::SemanticAnalyzer::throwSemanticException(std::vector<Token> &expression) {
    std::stringstream ss;
    ss << "Error in expression at line " << expression[0].lin << " from column " << expression[0].col <<
          ":\n\t" << expression[0].lexema << " PyTupleObject" <<
          "\nSemantic Exception: Can't handle unary operator \'" << expression[0].lexema << "\' for PyTupleObject.\n";
    throw("Semantic Exception.\n");
    throw std::runtime_error("Semantic Exception!\n");
}

#include <iostream>
#include <string>
#include <ctype.h>
#include <stack>
#include <vector>
#include <sstream>
#include <fstream>
#include <lexical.hpp>
#include <AST.hpp>
#include <syntax.hpp>
#include <stdexcept>

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str);
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }
    return internal;
}

int fillTable(std::vector<std::vector<std::string>> table, std::string path) {
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

int fillTable(std::vector<std::vector<int>> table, std::string path) {
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
            rules.push_back(rule);
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
        throw std::runtime_error("Error while opening CSV");
    }
}

int analyse() {
    Analyzer la;
    la.process();
    Token* t =  la.getList()->readHeading()->next;

    std::vector<std::vector<std::string>> action; // tabela action
    std::vector<std::vector<std::string>> go; // tabela goto
    std::vector<std::vector<int>> rules; // tabela de regras. [n][0] = numero de tokens na parte direita da regra, [n][1] = numero da regra pai (index na tabela - chamei de ruleSet)

    #ifdef DEBUG
        printf("\n\n%s\n\n", "--------------DEBUG MODE ON----------------");
        printf("\n\n%s\n\n", "----------LOADING DATA FROM FILES----------");
    #endif
    // Fill action and goto tables from CSV tables
    try {
        fillTable(action, "action.csv");
        fillTable(go, "goto.csv");
        fillTable(rules, "regras.csv");
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    #ifdef DEBUG
        printf("\n%s\n\n", "--------------SINTAX ANALYSER----------------");
    #endif
    // inicia a pilha de estados
    std::stack<int> states;
    int state;

    std::stack<DS::Node*> nodes;

    DS::AST ast;

    states.push(0);

    int c = 0;
    // analizador sintático
    while(1) {
        c++;
        //define estado atual como o estado do topo da pilha
        state = states.top();
        printf("Estado: %d, Token atual: %d (%s), ",state,t->type-1,t->lexema.c_str());
        //pega a action atual na tabela a partir do estado e do token
        std::string action_now = action[state][t->type-1];
        printf("Action: %s\n", action_now.c_str());

        if(action_now!="") {
            //caso SHIFT
            if(action_now.at(0)=='S'){
                printf("%s", "[SHIFT] -> ");
                int newstate;
                sscanf(action_now.c_str(), "S%d", &newstate); // le o novo estado
                states.push(newstate); // coloca o novo estado no topo da pilha
                DS::Node* leaf = ast.createLeaf(t);
                nodes.push(leaf);
                t = t->next; // passa pro proximo token
                printf("Novo Estado: %d\n",newstate);
            } else if(action_now.at(0)=='R') {
                printf("%s", "[REDUCE] -> ");
                int ruleToReduce;
                sscanf(action_now.c_str(), "R%d", &ruleToReduce); // le o index da regra pra aplicar reduce
                DS::Node *parent = ast.createNode(ruleToReduce);
                for( int i = 0; i < rules[ruleToReduce][0]; i++ ) {
                    states.pop(); // retira da pilha os tokens da regra
                    DS::Node* child = nodes.top();
                    ast.addChild(parent, child);
                    nodes.pop();
                }
                state = states.top(); // pega o novo estado do topo da pilha
                printf("Regra a Reduzir: %d, Numero de tokens da regra: %d, Ruleset: %d , Novo estado pos-pop: %d, ", ruleToReduce, rules[ruleToReduce][0], rules[ruleToReduce][1], state);
                int newstate_r;
                sscanf(go[state][rules[ruleToReduce][1]].c_str(), "%d", &newstate_r);
                printf("Goto: %s\n", go[state][rules[ruleToReduce][1]].c_str());
                states.push(newstate_r); // coloca o novo estado no topo da pilha
                nodes.push(parent);
            } else if(action_now == "acc") {
                ast.setRoot(nodes.top());
                cout << "\n!!!! Entrada aceita !!!!!\n\n";
                break;
            }
        } else {
            if(t->type==33) {
                t = t->next;
            } else {
                cout << "\n---Erro de sintaxe detectado---\n";
                break;
            }
        }
    }

    std::vector<DS::Node*> queue;
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
        std::vector<DS::Node*> children = queue[i]->children;
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

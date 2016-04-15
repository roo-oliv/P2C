#include <iostream>
#include <string>
#include <ctype.h>
#include <stack>
#include <vector>
#include <sstream>
#include <fstream>
#include "lexycal.h"
using namespace std;


vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str);
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}


int main() {
    
    Analyzer la;
    la.process();
    Token* t =  la.getList()->readHeading()->next;
    
    vector< vector<string> > action; // tabela action
    vector< vector<string> > go; // tabela goto
    vector< vector<int> > rules; // tabela de regras. [n][0] = numero de tokens na parte direita da regra, [n][1] = numero da regra pai (index na tabela - chamei de ruleSet)
    
    
    //le o csv da tabela action e preenche o vetor
    string line;
    ifstream tabelaAction("action.csv");
    if (tabelaAction.is_open()) {
        while ( getline(tabelaAction,line) ) {
            vector<string> tableLine = split(line, ';');
            action.push_back(tableLine);
        }
        tabelaAction.close();
    } else cout << "ERRO ao abrir tabela action";
    
    //le o csv da tabela goto e preenche o vetor
    ifstream tabelaGoto("goto.csv");
    if (tabelaGoto.is_open()) {
        while ( getline(tabelaGoto,line) ) {
            vector<string> tableLine = split(line, ';');
            go.push_back(tableLine);
        }
        tabelaGoto.close();
    } else cout << "ERRO ao abrir tabela goto";
    
    printf("\n\n%s\n\n", "--------------INICIO DEBUG----------------");
    //le o csv da tabela de regras e preenche o vetor - a ordem em que as regras aparecem esta ligada ao seu indice na tabela goto
    ifstream tabelaRegras("regras.csv");
    if (tabelaRegras.is_open()) {
        int ruleSet = 0;
        int count = 0;
        string temp = "S";
        while ( getline(tabelaRegras,line) ) {
            vector<string> tableLine = split(line, ';');
            vector<string> ruleTokens = split(tableLine[1], ' ');
            string ruleName = tableLine[0];
            if(ruleName != temp) {
                temp = ruleName;
                ruleSet++;
            }
            vector<int> rule;
            rule.push_back(ruleTokens.size());
            rule.push_back(ruleSet);
            rules.push_back(rule);
            printf("(%d) %s -> %d, Ruleset: %d\n", count, ruleName.c_str(), rule[0], rule[1]);
            count++;
        }
        tabelaRegras.close();
    } else cout << "ERRO ao abrir tabela de regras";
    printf("\n%s\n\n", "--------------FIM DEBUG----------------");
    
    printf("\n%s\n\n", "--------------INICIO ANALISADOR SINTATICO----------------");
    
    // inicia a pilha de estados
    stack <int> states;
    int state;
    
    states.push(0);
    
    int c = 0;
    // analizador sintático
    while(1) {
        c++;
        //define estado atual como o estado do topo da pilha
        state = states.top();
        printf("Estado: %d, Token atual: %d (%s), ",state,t->type-1,t->lexema.c_str());
        //pega a action atual na tabela a partir do estado e do token
        string action_now = action[state][t->type-1];
        printf("Action: %s\n", action_now.c_str());

        //caso SHIFT
        if(action_now.at(0)=='S'){
            printf("%s", "[SHIFT] -> ");
            int newstate;
            sscanf(action_now.c_str(), "S%d", &newstate); // le o novo estado depois do shift
            states.push(newstate); // coloca o novo estado no topo da pilha
            t = t->next; // passa pro proximo token
            printf("Novo Estado: %d\n",newstate);
        } else if(action_now.at(0)=='R') {
            printf("%s", "[REDUCE] -> ");
            int ruleToReduce;
            sscanf(action_now.c_str(), "R%d", &ruleToReduce); // le o index da regra pra aplicar reduce
            for( int i = 0; i < rules[ruleToReduce][0]; i++ ) {
                states.pop(); // retira da pilha os tokens da regra
            }
            state = states.top(); // pega o novo estado do topo da pilha
            printf("Regra a Reduzir: %d, Numero de tokens da regra: %d, Ruleset: %d , Novo estado pos-pop: %d, ", ruleToReduce, rules[ruleToReduce][0], rules[ruleToReduce][1], state);
            int newstate_r;
            sscanf(go[state][rules[ruleToReduce][1]-1].c_str(), "%d", &newstate_r);
            printf("Goto: %s\n", go[state][rules[ruleToReduce][1]-1].c_str());
            states.push(newstate_r); // coloca o novo estado no topo da pilha
        } else if(action_now == "acc") {
            cout << "Entrada aceita";
            break;
        } else {
            cout << "Erro de sintaxe detectado";
            break;
        }
    }
    
    return 0;
}
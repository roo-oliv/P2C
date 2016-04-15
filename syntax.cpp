#include <iostream>
#include <string>
#include <ctype.h>
#include <stack>
#include <vector>
#include <sstream>
#include <fstream>
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
    
    vector<vector<string>> action; // tabela action
    vector<vector<string>> go; // tabela goto
    vector<vector<int>> rules; // tabela de regras. [n][0] = numero de tokens na parte direita da regra, [n][1] = numero da regra pai (index na tabela - chamei de ruleSet)
    
    Token* t; // ponteiro para o primeiro token da lista que o eddie já tem no código dele
    
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
    
    //le o csv da tabela de regras e preenche o vetor - a ordem em que as regras aparecem esta ligada ao seu indice na tabela goto
    ifstream tabelaRegras("regras.csv");
    if (tabelaRegras.is_open()) {
        int ruleSet = 0;
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
        }
        tabelaRegras.close();
    } else cout << "ERRO ao abrir tabela de regras";
    
    
    // inicia a pilha de estados
    stack <int> states;
    int state;
    
    states.push(0);
    
    // analizador sintático
    while(1) {
        //define estado atual como o estado do topo da pilha
        state = states.top();

        //pega a action atual na tabela a partir do estado e do token
        string action_now = action[state][t->type];

        //caso SHIFT
        if(action_now.at(0)=='S'){
            int newstate;
            sscanf(action_now, "%*s%d", &newstate); // le o novo estado depois do shift
            states.push(newstate); // coloca o novo estado no topo da pilha
            t = t->next; // passa pro proximo token
        } else if(action_now.at(0)=='R') {
            int ruleToReduce;
            sscanf(action_now, "%*s%d", &ruleToReduce); // le o index da regra pra aplicar reduce
            for( int i = 0; i < rules[ruleToReduce][0]; i++ ) {
                states.pop(); // retira da pilha os tokens da regra
            }
            state = states.top(); // pega o novo estado do topo da pilha
            int newstate;
            sscanf(go[state][rules[ruleToReduce][1]],"%d", &newstate); // le o proximo estado na tabela goto
            states.push(newstate); // coloca o novo estado no topo da pilha
        } else if(action_now == "acc") {
            return 1;
        } else {
            cout << "Erro de sintaxe detectado";
            break;
        }
    }
    
    return 1;
}
#include "semantic.hpp"

compiler::SemanticAnalyzer::SemanticAnalyzer() {}

compiler::SemanticAnalyzer::~SemanticAnalyzer() {}

int compiler::SemanticAnalyzer::analyze(AST *ast) {
    Node *root = ast->getRoot();
    std::stringstream ss;
    exception *e;

    try {
        #ifdef DEBUG
            std::cerr << "\n\n---------SEMANTIC ANALYZER---------\n\n";
        #endif
        decorate(root);
        fillTable(root, 0);
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



void compiler::SemanticAnalyzer::fillTable(Node *root, int scope) {
    int aux;
    std::vector<Node*> *v;
    //std::cerr << root->content << " : " << root->regra << "\n";
    switch (root->regra) {
        case -1:
            //std::cerr << root->content << " : " << scope << "\n";
            root->scope = scope;
            return;
        case 21:
            v = AST::fetchLeaves(root->children[1]);
        	table.insert(v->at(0)->content,scope,v->at(0)->kind,v->at(0)->tk->lin,v->at(0)->tk->col,0);
            v->at(0)->scope = scope;
            #ifdef DEBUG
                //for (unsigned i = 0; i < root->children.size(); i++ )
                std::cerr << "Include in table the variable: '"<< root->children[1]->content << "' ";
                std::cerr <<"\nColumn: "<< v->at(0)->tk->col<< "";
                std::cerr <<"\nLine: "<< v->at(0)->tk->lin<< "\n";
                std::cerr << std::endl;
          	#endif
            fillTable(root->children[0], scope);
            return;
        case 44:
            v = AST::fetchLeaves(root->children[4]);
            table.scopes.push_back(scope);
            aux = table.scopes.size()-1;
            for(auto &i: *v) {
                if(i->kind==20) {
                    i->kind = 6;
                    i->scope = aux;
                    #ifdef DEBUG
                        std::cerr << "to insert " << i->content << " in scope " << aux << "\n";
                    #endif
                    table.insert(i->content,aux,i->kind,i->tk->lin,i->tk->col,0);
                }
            }
            for(auto &i : root->children) {
                i->scope = aux;
                fillTable(i, aux);
            }
            return;
        case 45: case 61: case 62: case 63: case 64: case 65:
            table.scopes.push_back(scope);
            aux = table.scopes.size()-1;
            for(auto &i : root->children) {
                i->scope = aux;
                fillTable(i, aux);
            }
            return;
        case 46:
            v = AST::fetchLeaves(root->children[2]);
            table.scopes.push_back(scope);
            aux = table.scopes.size()-1;
            for(auto &i : *v) {
                if(i->kind==20) {
                    i->kind=6;
                    i->scope = aux;
                    #ifdef DEBUG
                        std::cerr << "to insert " << i->content << " in scope " << aux << "\n";
                    #endif
                    table.insert(i->content,aux,i->kind,i->tk->lin,i->tk->col,0);
                }
            }
            table.insert(root->children[3]->content,scope,root->children[3]->kind,root->children[3]->tk->lin,root->children[3]->tk->col,(v->size() - 1)/2);
            root->children[3]->scope = scope;
            #ifdef DEBUG
                std::cerr << "Include in table the function '"<< root->children[3]->content << "' with ";
    	        std::cerr << (v->size() - 1)/2<< " arguments.";
                std::cerr <<"\nColumn: "<< root->children[3]->tk->col<< "";
                std::cerr <<"\nLine: "<< root->children[3]->tk->lin<< "\n";
                std::cerr << std::endl;
          	#endif
            fillTable(root->children.at(0), aux);
            return;
        default:
            for(auto &i : root->children)
                fillTable(i, scope);
            return;
    }
}

compiler::SymbolTable* compiler::SemanticAnalyzer::getSymbolTable() {return &table;}

void compiler::SemanticAnalyzer::decorate(Node *root) { // Decorate tree with node kinds and content
    for(auto &i : root->children)   // Descend tree to decorate children first
        decorate(i);
    if(root->tk) {  // if node has a token it's a leaf node
        #ifdef DEBUG
            std::cerr << "decorating leaf node of token: " << root->tk->lexema << std::endl;
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
                root->kind = 20;
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
            std::cerr << "Decorating node of children: ";
            for (unsigned i = root->children.size(); i-- > 0; )
                std::cerr << root->children[i]->content << " ";
            std::cerr << std::endl;
        #endif
        root->kind = concat(root->regra, root->children);
        /*switch (root->children.size()) { // initial break down of simple expressions up to 3 elements and more complex ones
            case 1:
                root->kind = root->children[0]->kind;
                break;
            case 2:
                if(root->children[1]->kind==20) // can only be a '+' or a '-', pending for unary or binary operator attribution
                    root->children[1]->kind = 0;  // in this case (root->children.size()==2) there is only one operand, so it will be an unary operator
                root->kind = concat(root->children);
                if(root->kind==21) // in this case the node corresponds for a whole expression and won't concatenate with others anymore
                    root->content = ""; // it's content is cleared
                break;
            case 3:
                if(root->children[1]->kind==20) // can only be a '+' or a '-', pending for unary or binary operator attribution
                    root->children[1]->kind = 1; // in this case (root->children.size()==3) there is two operands, so it will be a binary operator
                root->kind = concat(root->children);
                break;
            default:
                root->kind = concat(root->children);
                break;
        }*/
    }
    #ifdef DEBUG
        std::cerr << "\tdecorated with kind: " << root->getKindName() << "\n\n";
    #endif
}

int compiler::SemanticAnalyzer::concat(int rule, std::vector<Node*> &expression) {
    std::stringstream ss;
    exception *e;
    #ifdef DEBUG
        std::cerr << "\tConcatenating children from node of rule " << expression[0]->parent->regra << std::endl;
    #endif
    switch (rule) {
        case 21: // $ $(= $)
            if(expression[1]->kind==20) expression[1]->kind=7; // resolving pending kind to VARIABLE
            if((expression[1]->kind==7) || (expression[1]->kind==22 && (expression[0]->kind==11 || expression[0]->kind==12 || expression[0]->kind==20 || expression[0]->kind==7 || expression[0]->kind==8))) return 21; // return WHOLE_EXPRESSION kind
            if(expression[1]->kind==22) {
                ss << "TypeError: \'" << expression[0]->getKindName() << "\' is not iterable";
                e = new exception(*(expression[0]->parent), ss.str());
                throw *e;
            }
            ss << "SyntaxError: can't assign to literal";
            e = new exception(*(expression[0]->parent), ss.str());
            throw *e;
        case 27: // return $
            return 21; // return WHOLE_EXPRESSION kind
        case 32: case 53: case 100: // $ , $
            if((expression[0]->kind==20 || expression[0]->kind==22) && (expression[2]->kind==20 || expression[2]->kind==22)) // if both elements have PENDING or VARIABLES_TUPLE kind they may be suitable for a VARIABLES_TUPLE kind rather than TUPLE
                return 22; // return VARIABLES_TUPLE kind
            return 11; // return TUPLE kind
        case 46: // def $ $ : \n
            expression[3]->kind = 8;
            return 21;
        case 48: case 47: case 87: case 89:
            if(expression[1]->kind==20) expression[1]->kind = 22; // if contents of tuple are all of PENDING kind then resolve to VARIABLES_TUPLE kind
            if(expression[1]->kind==22) return 22; // return VARIABLES_TUPLE kind
            return 11; // return TUPLE
        case 52: // $ = $ // parameter inside VARIABLES_TUPLE
            if(expression[2]->kind==20) expression[2]->kind = 6;
            if(expression[2]->kind==6) return 6;
            ss << "SyntaxError: invalid syntax";
            e = new exception(*(expression[0]->parent), ss.str());
            throw *e;
        case 54: // $ = $ , $ // parameter inside VARIABLES_TUPLE
            if(expression[4]->kind==20) expression[4]->kind = 6;
            if(expression[4]->kind==6) return 22;
            ss << "SyntaxError: invalid syntax";
            e = new exception(*(expression[0]->parent), ss.str());
            throw *e;
        case 76: // $ > $ | $ < $ | $ >= $ | $ <= $ | $ == $ | $ != $
            return 10; // return BOOLEAN kind
        case 78: // $ + $ | $ - $
            expression[1]->kind = 1; // resolving pending kind to BINARY_OPERATOR
            if((expression[0]->kind==11 || expression[2]->kind==11) || // Can't handle tuples
               ((expression[0]->kind==14 || expression[2]->kind==14) && (expression[0]->kind!=expression[2]->kind || expression[1]->tk->lexema!="+")) || // // Can only handle TUPLE + TUPLE
               ((expression[0]->kind==12 || expression[2]->kind==12) && (expression[0]->kind!=expression[2]->kind || expression[1]->tk->lexema!="+")) // Can only handle LIST + LIST
              ) {
                ss << "TypeError: unsupported operand type(s) for " << expression[1]->tk->lexema << ": \'" << expression[0]->getKindName() << "\' and \'" << expression[2]->getKindName() << "\'";
                e = new exception(*(expression[0]->parent), ss.str());
                throw *e;
            } else {
                if(expression[0]->kind!=12)
                    return 9; // return NUMBER kind
                else
                    return expression[0]->kind; // return LIST or TUPLE kind
            }
        case 80: // $ * $ | $ / $ | $ % $
            if((expression[0]->kind==14 || expression[2]->kind==14) || // Can't handle none types
               ((expression[0]->kind==11 || expression[2]->kind==11) && (expression[0]->kind==expression[2]->kind || expression[1]->tk->lexema!="*")) || // Can only handle one operand TUPLE and the operator must be '*'
               ((expression[0]->kind==12 || expression[2]->kind==12) && (expression[0]->kind==expression[2]->kind || expression[1]->tk->lexema!="*")) // Can only handle one operand LIST and the operator must be '*'
            ) {
                ss << "TypeError: unsupported operand type(s) for " << expression[1]->tk->lexema << ": \'" << expression[0]->getKindName() << "\' and \'" << expression[2]->getKindName() << "\'";
                e = new exception(*(expression[0]->parent), ss.str());
                throw *e;
            } else {
                if(expression[0]->kind!=12 && expression[2]->kind!=12)
                    return 9; // return NUMBER kind
                else
                    return 12; // return LIST kind
            }
        case 81: // + $ | - $
            expression[1]->kind = 0; // resolving pending kind to UNARY_OPERATOR
            switch (expression[0]->kind) {
                case 7: case 8: case 9: case 10: case 20:
                    return expression[0]->kind; // return ELEMENT kind
                default:
                    ss << "TypeError: bad operand type for unary " << expression[1]->tk->lexema << ": \'" << expression[0]->getKindName() << "\'";
                    e = new exception(*(expression[0]->parent), ss.str());
                    throw *e;
            }
        case 90: case 91: // [] : [$]
            return 12; // return LIST kind
        default:
            return expression[0]->kind;
    }
}

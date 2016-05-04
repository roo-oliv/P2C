#include "exception.hpp"

compiler::exception::exception(Token &root, std::string specification) {
    std::stringstream ss;
    bool flag = true;
    formExpression(&root, ss, flag);
    ss << "\n" << specification << std::endl;
    message = ss.str();
}

compiler::exception::exception(Node &root, std::string specification) {
    std::stringstream ss;
    bool flag = true;
    formExpression(&root, ss, flag);
    ss << "\n" << specification << std::endl;
    message = ss.str();
}

const char* compiler::exception::what() const throw() {
    return message.c_str();
}

void compiler::exception::formExpression(Node *root, std::stringstream &ss, bool &empty) {
    if(root->children.empty()) {
        if(empty) {
            ss << "\nError found in expression at line " << root->tk->lin << ", column " << root->tk->col << ":\n ... ";
            empty = false;
        }
        ss << root->tk->lexema << " ";
    } else {
        for (unsigned i = root->children.size(); i-- > 0; )
            formExpression(root->children[i], ss, empty);
    }
}

void compiler::exception::formExpression(Token *root, std::stringstream &ss, bool &empty) {
    if(empty) {
        ss << "\nError found in expression at line " << root->lin << ", column " << root->col << ":\n ... ";
        empty = false;
    }

    if(root->parent->lin==root->lin) {
        formExpression(root->parent, ss, empty);
    } else {
        while(root->next->lin==root->lin) {
            if(root->type!=29 && root->type!=31 && root->type!=33)
                ss << root->lexema << " ";
            root = root->next;
        }
        if(root->type!=29 && root->type!=31 && root->type!=33)
            ss << root->lexema << " ";
    }
}

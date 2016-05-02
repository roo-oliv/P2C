#include "exception.hpp"

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
        ss << root.tk->lexema << " ";
    } else {
        for (unsigned i = root.children.size(); i-- > 0; )
            formExpression(*(root.children[i]), ss, empty);
    }
}

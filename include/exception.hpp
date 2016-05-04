//=================================
// include guard
#ifndef __EXCEPTION_HPP_INCLUDED__
#define __EXCEPTION_HPP_INCLUDED__

//=================================
// included dependencies
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include "datastructures.hpp"

namespace compiler {
    class exception: public std::exception {
	public:
		exception(Token&, std::string);
    	exception(Node&, std::string);
		virtual const char* what() const throw();
	private:
		std::string message;
		void formExpression(Node*, std::stringstream&, bool&);
        void formExpression(Token*, std::stringstream&, bool&);
	};
}

#endif // __EXCEPTION_HPP_INCLUDED__

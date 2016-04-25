#include <iostream>
#include <string>
#include <ctype.h>
#include <stack>
#include <vector>
#include <sstream>
#include <fstream>
#include <lexical.hpp>
#include <AST.hpp>
#include <stdexcept>

std::vector<std::string> split(std::string, char);
int fillTable(std::vector<std::vector<std::string>>, std::string);
int fillTable(std::vector<std::vector<int>>, std::string);
int analyse();

//=================================
// include guard
#ifndef __PYBUILTINS_HPP_INCLUDED__
#define __PYBUILTINS_HPP_INCLUDED__

//=================================
// included dependencies
#include <iostream>
#include <cmath>

auto print(auto a) {std::cout << a << std::endl;}
auto abs(auto a) {return std::abs(a);}

#endif // __PYBUILTINS_HPP_INCLUDED__

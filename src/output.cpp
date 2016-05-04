#include <iostream>
#include "pybuiltins.hpp"

#define a0x205c180(arg) print(arg)
#define a0x205c1e0(arg) abs(arg)

int main() {
	auto a0x2063aa0 = [&]( auto a0x2063a70, auto a0x2063990)  {
		return a0x2063a70* a0x2063990;
	};
	auto a0x2063130 = [&]( auto a0x2063100)  {
		auto __cmpvar__0x20674f0 = {1,2,3,4};
		for (auto a0x20636c0 : __cmpvar__0x20674f0) {
a0x205c180( a0x2063aa0( a0x2063100, a0x20636c0) ) ;
		}
	};
a0x2063130( 7 ) ;
}
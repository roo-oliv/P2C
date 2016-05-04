#include <iostream>
#include "pybuiltins.hpp"

#define a0x18e5990(arg) print(arg)
#define a0x1905d80(arg) abs(arg)

int main() {
	auto a0x1908b90 = []( auto a0x1908b20, auto a0x1908960)  {
		return 83 * a0x1908960;
	};
a0x18e5990( a0x1908b90( 8 , 7 ) ) ;
}
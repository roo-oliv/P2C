#include <iostream>
#include "pybuiltins.hpp"

#define a0x192f990(arg) print(arg)
#define a0x194fd80(arg) abs(arg)

int main() {
	auto a0x1952b90 = []( auto a0x1952b20, auto a0x1952960)  {
		return a0x1952b20* a0x1952960;
	};
a0x192f990( a0x1952b90( 8 , 7 ) ) ;
}
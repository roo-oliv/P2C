#include <iostream>
#include "pybuiltins.hpp"

#define a0x150be50(arg) print(arg)
#define a0x150bef0(arg) abs(arg)

int main() {
	auto a0x151c800 = 8.0 ;
	auto a0x151c470 = a0x151c800+ 4 ;
if ( a0x151c470> a0x151c800)  {
a0x150be50( a0x151c470/ 4 ) ;
	}if ( a0x151c470< 0 )  {
a0x150be50( - 1 ) ;
	}else if( (a0x151c470> 0  && a0x151c800< 4 ))  {
a0x150be50( - 2 ) ;
	}else if( (a0x151c470== 0  || a0x151c470> 10 ))  {
a0x150be50( 1 ) ;
	}else  {
a0x150be50( - 3 ) ;
	}}
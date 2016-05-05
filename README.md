# P2C
This software compiles code from a subset of the Python 3 language to C++14 code.

# Compiling instructions for a linux environment

`cd ./src`

`make`

# Usage

`./P2C.exe < <path_to_input_file>`

or simply

`./P2C.exe`

After executing, the result of the compilation will be written to the file `/src/output.cpp`

To compile this file one needs to have `pybuiltins.hpp`, which is already supplied in the `/src` folder.

Also it is necessary a compiler that supports C++14, we reccomend the package `g++-5` available for Ubuntu from the repository: `ppa:ubuntu-toolchain-r/test`.

To compile and execute the output file run this command or similar depending on the compiler used:

`g++-5 -std=c++14 -o output.exe output.cpp && ./output.exe`

# Authors

Camila do Amaral Sass

Cauê Massi Correa

Eduardo Thomas Koller

Rodrigo Martins de Oliveira

Wedeueis Braz da Silva



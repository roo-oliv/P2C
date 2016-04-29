#include <compiler.hpp>

#ifdef DEBUG
    int DEBUG_LEVEL, DEBUG_SPECIFIER;
#endif

int main() {
    #ifdef DEBUG
        printf("    !DEBUG MODE ON!\n  Choose debugging level:\n  0 - minimalistic, only for progress observation\n  1 - superficial, for fast debug\n  2 - detailed, gives better overview\n  3 - in depth, as much details as possible\n>>> ");
        std::cin >> DEBUG_LEVEL;
        printf("    Choose what to debug:\n  0 - everything\n 1 - Lexycal Analyzer\n 2 - Syntax Analyzer\n 3 - Semantic Analyzer\n 4 - Translator\n>>> ");
        std::cin >> DEBUG_SPECIFIER;
        std::cout << "\n::::::::::::::::START::::::::::::::::\n";
    #endif
    compiler::SemanticAnalyzer sa;
    sa.analyze();
    return 0;
}

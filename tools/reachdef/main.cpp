#include "Analysis/reachdef.h"
#include <iostream>

int main() {
    ReachDefAnalysisPass pass;
    pass.execute(std::cin, std::cout);
    return 0;
}
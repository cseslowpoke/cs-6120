#include "Passes/tdce.h"
#include <iostream>

int main() {
    TDCE pass;
    pass.execute(std::cin, std::cout);
    return 0;
}
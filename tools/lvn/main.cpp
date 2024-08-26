#include "Passes/lvn.h"
#include <iostream>

int main() {
    LVN pass;
    pass.execute(std::cin, std::cout);
    return 0;
}
#include "Analysis/dominator.h"
#include <iostream>

int main() {
  DominatorTreePass pass;
  pass.execute(std::cin, std::cout);
}

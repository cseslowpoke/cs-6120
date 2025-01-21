#include "Passes/intoSSA.h"
#include <cstdio>

int main() {
  intoSSA pass;

  // open a file
  freopen("test.json", "r", stdin);
  pass.execute(std::cin, std::cout);
}

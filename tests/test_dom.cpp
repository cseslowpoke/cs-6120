#include "Analysis/dominator.h"
#include <nlohmann/json.hpp>

int main() {
  auto ir = nlohmann::json::parse(std::cin);
  DominatorTreePass pass;
  pass.run(ir);
}

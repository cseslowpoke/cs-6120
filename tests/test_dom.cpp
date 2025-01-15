#include "Analysis/dominator.h"
#include <nlohmann/json.hpp>

int main() {
  FILE *fp = fopen("test.json", "r");
  auto ir = nlohmann::json::parse(fp);
  DominatorTreePass pass;
  pass.run(ir);
}

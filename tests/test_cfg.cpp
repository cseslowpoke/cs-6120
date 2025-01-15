#include "Analysis/cfg.h"
#include <iostream>
#include <nlohmann/json.hpp>
int main() {
  // open file name "test.json"
  FILE *fp = fopen("test.json", "r");
  auto ir = nlohmann::json::parse(fp);
  for (auto func : ir["functions"]) {
    auto form = formBlock(func["instrs"]);
    auto blockmap = BlockMap(form);
    buildCFG(blockmap);
    addEntry(blockmap);
    addTerminators(blockmap);
    for (size_t i = 0; i < blockmap.size(); i++) {
      std::cout << blockmap.getname(i) << ": \n"
                << blockmap[i].Data().dump(-1) << std::endl;
    }
    for (int i = 0; i < blockmap.size(); i++) {
      const std::string &from = blockmap.getname(i);
      for (auto &to : blockmap[i].getSucc()) {
        if (to != nullptr) {
          std::cout << from << " -> " << to->getName() << std::endl;
        } else {
          std::cout << from << " ->  " << std::endl;
        }
      }
    }
  }
}

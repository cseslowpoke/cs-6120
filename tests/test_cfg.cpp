#include "Analysis/cfg.h"
#include <iostream>
#include <nlohmann/json.hpp>
int main() {
  auto ir = nlohmann::json::parse(std::cin);
  for (auto func : ir["functions"]) {
    auto form = form_block(func["instrs"]);
    auto blockmap = BlockMap(form);
    for (size_t i = 0; i < blockmap.size(); i++) {
      std::cout << blockmap.getname(i) << ": \n"
                << blockmap[i].dump(-1) << std::endl;
    }
    auto cfg = get_cfg(blockmap);
    for (auto &[from, to_node] : cfg) {
      for (auto &to : to_node) {
        std::cout << from << " -> " << to << std::endl;
      }
    }
  }
}

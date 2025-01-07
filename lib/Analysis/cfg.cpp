#include "Analysis/cfg.h"

json form_block(json &body) {
  json func = json::array();
  json cur_block = json::array();
  for (auto instr : body) {
    if (instr.contains("op")) {
      cur_block.push_back(instr);
      if (terminator.find(instr["op"]) != terminator.end()) {
        // std::cout << "Block: " << cur_block.dump(-1) << std::endl;
        func.push_back(cur_block);
        cur_block.clear();
      }
    } else {
      if (!cur_block.empty()) {
        // std::cout << "Block: " << cur_block.dump(-1) << std::endl;
        func.push_back(cur_block);
        cur_block.clear();
      }
      cur_block.push_back(instr);
    }
  }
  if (!cur_block.empty()) {
    // std::cout << "Block: " << cur_block.dump(-1) << std::endl;
    func.push_back(cur_block);
  }
  return func;
}

BlockMap::BlockMap(json &body) {
  for (auto &block : body) {
    std::string name;
    if (block[0].contains("label")) {
      name = block[0]["label"];
    } else {
      name = "b" + std::to_string(order.size());
    }
    index_map.insert({name, order.size()});
    name_map.insert({order.size(), name});
    order.push_back(block);
  }
}

// std::map<std::string, json> block_map(json &body) {
//   std::map<std::string, json> name2block;
//   for (auto &block : body) {
//     std::string name;
//     if (block[0].contains("label")) {
//       name = block[0]["label"];
//     } else {
//       name = "b" + std::to_string(name2block.size());
//     }
//     name2block.insert({name, block});
//   }
//   return name2block;
// }

std::map<std::string, std::vector<std::string>> get_cfg(BlockMap &blockmap) {
  std::map<std::string, std::vector<std::string>> cfg;
  for (int i = 0; i < blockmap.size(); i++) {
    auto &block = blockmap[i];
    auto &name = blockmap.getname(i);
    auto &last = block.back();
    if (!last.contains("op")) {
      if (i != blockmap.size() - 1) {
        cfg[name].push_back(blockmap.getname(i + 1));
      }
    } else if (last.at("op") == "ret") {
      cfg[name].push_back(" ");
    } else if (last.at("op") == "jmp") {
      cfg[name].push_back(last.at("label")[0]);
    } else if (last.at("op") == "br") {
      cfg[name].push_back(last.at("label")[0]);
      cfg[name].push_back(last.at("label")[1]);
    } else {
      cfg[name].push_back(blockmap.getname(i + 1));
    }
  }
  return cfg;
}

// void cfg(json prog) {
//   for (auto func : prog["functions"]) {
//     auto form = form_block(func["instrs"]);
//     auto blockmap = BlockMap(form);
//     for (size_t i = 0; i < blockmap.size(); i++) {
//       auto &block = blockmap[i];
//       // std::cout << name << ": \n  " << block.dump(-1) << std::endl;
//     }
//     auto cfg = get_cfg(blockmap);
//     for (auto &[from, to] : cfg) {
//       // std::cout << from << " -> " << to << std::endl;
//     }
//   }
// }

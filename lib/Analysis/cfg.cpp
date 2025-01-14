#include "Analysis/cfg.h"

BlockMap::BlockMap(std::vector<BasicBlock> &body) {
  for (auto &block : body) {
    auto blockd = block.Data();
    std::string name;
    if (blockd.contains("label")) {
      name = blockd["label"];
    } else {
      name = "b" + std::to_string(order.size());
    }
    index_map.insert({name, order.size()});
    name_map.insert({order.size(), name});
  }
  order = std::move(body);
}

std::vector<BasicBlock> formBlock(json &body) {
  std::vector<BasicBlock> func;
  json cur_block = json::array();
  for (auto instr : body) {
    if (instr.contains("op")) {
      cur_block.push_back(instr);
      if (terminator.find(instr["op"]) != terminator.end()) {
        func.push_back(BasicBlock(cur_block));
        cur_block.clear();
      }
    } else {
      if (!cur_block.empty()) {
        func.push_back(BasicBlock(cur_block));
        cur_block.clear();
      }
      cur_block.push_back(instr);
    }
  }
  if (!cur_block.empty()) {
    func.push_back(BasicBlock(cur_block));
  }
  return func;
}

void buildCFG(BlockMap &Function) {
  for (int i = 0; i < Function.size(); i++) {
    auto &block = Function[i];
    auto &name = Function.getname(i);
    auto &last = block.Data().back();
    if (!last.contains("op")) {
      if (i != Function.size() - 1) {
        block.addSucc(&Function[i + 1]);
        Function[i + 1].addPred(&block);
      }
    } else if (last.at("op") == "ret") {
      block.addSucc(nullptr);
    } else if (last.at("op") == "jmp") {
      int index = Function.getindex(last.at("labels")[0]);
      auto &target = Function[index];
      block.addSucc(&target);
      target.addPred(&block);
    } else if (last.at("op") == "br") {
      int index0 = Function.getindex(last.at("labels")[0]);
      int index1 = Function.getindex(last.at("labels")[1]);
      auto &target0 = Function[index0];
      auto &target1 = Function[index1];
      block.addSucc(&target0);
      block.addSucc(&target1);
      target0.addPred(&block);
      target1.addPred(&block);
    } else {
      if (i != Function.size() - 1) {
        block.addSucc(&Function[i + 1]);
        Function[i + 1].addPred(&block);
      }
    }
  }
}

#include "Analysis/cfg.h"
#include <iostream>

BlockMap::BlockMap(BlockMap::BasicBlocks &body) {
  int i = 0;
  for (auto *block : body) {
    auto blockd = block->Data().at(0);
    std::string name;
    if (blockd.contains("label")) {
      name = blockd["label"];
    } else {
      name = "b" + std::to_string(i);
    }
    block->setName(name);
    index_map.insert({name, i});
    name_map.insert({i, name});
    i++;
  }
  order = std::move(body);
}

BlockMap::BasicBlocks formBlock(json &body) {
  BlockMap::BasicBlocks func;
  json cur_block = json::array();
  for (auto instr : body) {
    if (instr.contains("op")) {
      cur_block.push_back(instr);
      if (terminator.find(instr["op"]) != terminator.end()) {
        func.push_back(new BasicBlock(cur_block));
        cur_block.clear();
      }
    } else {
      if (!cur_block.empty()) {
        func.push_back(new BasicBlock(cur_block));
        cur_block.clear();
      }
      cur_block.push_back(instr);
    }
  }
  if (!cur_block.empty()) {
    func.push_back(new BasicBlock(cur_block));
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
      std::string a = last.at("labels")[0];
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

void addTerminators(BlockMap &func) {
  for (int i = 0; i < func.size(); i++) {
    auto &block = func[i];
    auto &last = block.Data().back();
    if (last.contains("op")) {
      if (terminator.find(last.at("op")) == terminator.end()) {
        if (i == func.size() - 1) {
          json ret = {{"op", "ret"}, {"args", {}}};
          block.Data().push_back(ret);
        } else {
          json jmp = {{"op", "jmp"}, {"labels", {func.getname(i + 1)}}};
          block.Data().push_back(jmp);
        }
      }
    }
  }
}

void addEntry(BlockMap &func) {
  auto &entry = func.getEntry();
  if (!entry.getPred().empty()) {
    auto blocks = func.getBlocks();
    json entry_instr = {{{"label", "entry"}}};
    BasicBlock *new_entry = new BasicBlock(entry_instr);
    blocks.insert(blocks.begin(), new_entry);
    BlockMap new_func(blocks);
    func = new_func;
    func[0].addSucc(&func[1]);
    func[1].addPred(&func[0]);
  }
}

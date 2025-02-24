#include "Passes/intoSSA.h"
#include "Analysis/cfg.h"
#include "Analysis/dominator.h"
#include <nlohmann/json.hpp>
#include <set>

namespace {

std::map<std::string, std::vector<std::string>> getVariable(BlockMap &blocks) {
  std::map<std::string, std::vector<std::string>> VariableList;
  for (auto &block : blocks.getBlocks()) {
    for (auto &inst : block->Data()) {
      if (inst.contains("dest")) {
        auto &Vlist = VariableList[inst["dest"]];
        int flag = 0;
        for (auto &bb : Vlist) {
          if (bb == block->getName()) {
            flag = 1;
            break;
          }
        }
        if (flag) {
          continue;
        }
        VariableList[inst["dest"]].push_back(block->getName());
      }
    }
  }
  return VariableList;
}

void addPhi(BlockMap &blocks, DominatorTree &domTree) {
  auto frontier = domTree.getFrontier();
  auto VariableList = getVariable(blocks);
  std::map<std::string, std::map<std::string, std::set<std::string>>> phi;
  for (auto &[id, blocks] : VariableList) {
    for (int i = 0; i < blocks.size(); i++) {
      auto &block = blocks[i];
      for (auto &frontierBlock : frontier[block]) {
        if (phi[frontierBlock][id].find(block) !=
            phi[frontierBlock][id].end()) {
          continue;
        }
        phi[frontierBlock][id].insert(block);

        bool flag = 0;
        for (auto &bb : blocks) {
          if (bb == frontierBlock) {
            flag = 1;
            break;
          }
        }
        if (flag) {
          continue;
        }
        blocks.push_back(frontierBlock);
      }
    }
  }
  for (auto &[blockName, insts] : phi) {
    for (auto &[id, fromBlocks] : insts) {
      if (fromBlocks.size() <= 1) {
        continue;
      }
      auto &block = blocks[blockName];
      json phiInst = {{"op", "phi"},
                      {"dest", id},
                      {"args", json::array()},
                      {"labels", json::array()},
                      {"type", "int"}};
      std::cerr << fromBlocks.size() << std::endl;
      for (auto &fromBlock : fromBlocks) {
        phiInst["args"].push_back(id);
        phiInst["labels"].push_back(fromBlock);
      }
      block.Data().insert(block.Data().begin() + 1, phiInst);
    }
  }
}

void rename(BasicBlock &entry, BlockMap &blockmap,
            std::map<std::string, std::vector<std::string>> &vstack) {}

json blockToIR(BlockMap &blockmap) {
  json ir = json::array();
  for (auto block : blockmap.getBlocks()) {
    auto blockIR = block->Data();
    if (blockIR.empty()) {
      continue;
    }
    ir.insert(ir.end(), blockIR.begin(), blockIR.end());
  }
  return ir;
}

void toSSA(json &func) {
  auto form = formBlock(func);
  auto blocks = BlockMap(form);
  buildCFG(blocks);
  addTerminators(blocks);
  addEntry(blocks);
  DominatorTree domTree(func);
  addPhi(blocks, domTree);
  std::map<std::string, std::vector<std::string>> vstack;
  rename(blocks[0], blocks, vstack);
  func = blockToIR(blocks);
}
} // namespace

void intoSSA::run(json &ir) {
  for (auto &func : ir["functions"]) {
    toSSA(func["instrs"]);
  }
}

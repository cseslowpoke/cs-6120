#include "Analysis/dominator.h"
#include "Analysis/cfg.h"
#include <iostream>

std::map<std::string, std::vector<std::string>>
get_rev_cfg(std::map<std::string, std::vector<std::string>> &cfg) {
  std::map<std::string, std::vector<std::string>> rev_cfg;
  for (auto &[from, edge] : cfg) {
    for (auto &to : edge) {
      rev_cfg[to].push_back(from);
    }
  }
  return rev_cfg;
}

DominatorTree::DominatorTree(json &body) {
  auto form = form_block(body);
  auto blockmap = BlockMap(form);
  auto cfg = get_cfg(blockmap);
  auto rev_cfg = get_rev_cfg(cfg);
  bool change = 0;
  dom[blockmap.getname(0)].insert(blockmap.getname(0));
  do {
    change = 0;
    for (int i = 1; i < blockmap.size(); i++) {
      auto &old_dom = dom[blockmap.getname(i)];
      std::set<std::string> new_dom;
      new_dom.insert(blockmap.getname(i));
      for (auto &pred : rev_cfg[blockmap.getname(i)]) {
        std::set_union(new_dom.begin(), new_dom.end(), dom[pred].begin(),
                       dom[pred].end(),
                       std::inserter(new_dom, new_dom.begin()));
      }
      if (old_dom != new_dom) {
        old_dom = new_dom;
        change = 1;
      }
    }
  } while (change);
}

void DominatorTree::print() {
  for (auto &[node, dom_nodes] : dom) {
    std::cout << node << " : ";
    for (auto &dom_node : dom_nodes) {
      std::cout << dom_node << ", ";
    }
    std::cout << '\n';
  }
}

void DominatorTreePass::run(json &ir) {
  for (auto func : ir["functions"]) {
    DominatorTree DT(func["instrs"]);
    DT.print();
  }
}

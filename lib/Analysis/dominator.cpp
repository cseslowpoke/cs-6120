#include "Analysis/dominator.h"
#include "Analysis/cfg.h"
#include <algorithm>
#include <iostream>

void calculate_frontier(
    std::map<std::string, std::set<std::string>> &frontier,
    std::map<std::string, std::set<std::string>> &dom,
    std::map<std::string, std::vector<std::string>> &rev_cfg,
    BlockMap &blockmap) {

  for (int i = 0; i < blockmap.size(); i++) {
    for (int j = 0; j < blockmap.size(); j++) {
      if (i == j) {
        continue;
      }
      auto &lhs = blockmap.getname(i);
      auto &rhs = blockmap.getname(j);
      if (dom[rhs].find(lhs) != dom[rhs].end()) {
        continue;
      }
      for (auto pred : rev_cfg[rhs]) {
        if (dom[pred].find(lhs) != dom[pred].end()) {
          frontier[lhs].insert(rhs);
          break;
        }
      }
    }
  }
}

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
  for (int i = 1; i < blockmap.size(); i++) {
    for (int j = 0; j < blockmap.size(); j++) {
      dom[blockmap.getname(i)].insert(blockmap.getname(j));
    }
  }
  do {
    change = 0;
    for (int i = 1; i < blockmap.size(); i++) {
      auto &node_name = blockmap.getname(i);
      auto &old_dom = dom[node_name];
      std::set<std::string> new_dom = dom[*rev_cfg[node_name].begin()];
      for (auto &pred : rev_cfg[node_name]) {
        std::set<std::string> tmp;
        std::set_intersection(new_dom.begin(), new_dom.end(), dom[pred].begin(),
                              dom[pred].end(), std::inserter(tmp, tmp.begin()));
        new_dom = std::move(tmp);
      }
      std::set<std::string> self_dom = {node_name};
      std::set_union(new_dom.begin(), new_dom.end(), self_dom.begin(),
                     self_dom.end(), std::inserter(new_dom, new_dom.begin()));
      if (old_dom != new_dom) {
        old_dom = new_dom;
        change = 1;
      }
    }
  } while (change);

  calculate_frontier(frontier, dom, rev_cfg, blockmap);
}

void DominatorTree::print() {
  std::cout << "dom: \n";
  for (auto &[node, dom_nodes] : dom) {
    std::cout << node << " : ";
    for (auto &dom_node : dom_nodes) {
      std::cout << dom_node << ", ";
    }
    std::cout << '\n';
  }
  std::cout << "frontier: \n";
  for (auto &[node, frontier_nodes] : frontier) {
    std::cout << node << " : ";
    for (auto &frontier_node : frontier_nodes) {
      std::cout << frontier_node << ", ";
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

#include "Analysis/dominator.h"
#include "Analysis/cfg.h"
#include <algorithm>
#include <iostream>

void calculate_frontier(std::map<std::string, std::set<std::string>> &frontier,
                        std::map<std::string, std::set<std::string>> &dom,
                        BlockMap &function) {

  for (int i = 0; i < function.size(); i++) {
    for (int j = 0; j < function.size(); j++) {
      if (i == j) {
        continue;
      }
      auto &block0 = function[i];
      auto &block1 = function[j];
      if (dom[block1.getName()].find(block0.getName()) !=
          dom[block1.getName()].end()) {
        continue;
      }
      for (auto pred : block1.getPred()) {
        if (dom[pred->getName()].find(block0.getName()) !=
            dom[pred->getName()].end()) {
          frontier[block0.getName()].insert(block1.getName());
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
  auto form = formBlock(body);
  auto function = BlockMap(form);
  buildCFG(function);
  addEntry(function);
  addTerminators(function);
  bool change = 0;
  dom[function.getname(0)].insert(function.getname(0));
  for (int i = 1; i < function.size(); i++) {
    for (int j = 0; j < function.size(); j++) {
      dom[function.getname(i)].insert(function.getname(j));
    }
  }
  do {
    change = 0;
    for (int i = 1; i < function.size(); i++) {
      auto &node_name = function.getname(i);
      auto &preds = function[i].getPred();
      auto &old_dom = dom[node_name];

      std::set<std::string> new_dom = dom[preds[0]->getName()];
      for (auto &pred : preds) {
        auto &pred_name = pred->getName();
        std::set<std::string> tmp;
        std::set_intersection(new_dom.begin(), new_dom.end(),
                              dom[pred_name].begin(), dom[pred_name].end(),
                              std::inserter(tmp, tmp.begin()));
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

  calculate_frontier(frontier, dom, function);
}

void DominatorTree::print() {
  std::cout << "Dominator Tree: \n";
  for (auto &[node, dom_nodes] : dom) {
    std::cout << node << " : ";
    for (auto &dom_node : dom_nodes) {
      std::cout << dom_node << ", ";
    }
    std::cout << '\n';
  }
  std::cout << "Dominance Frontier: \n";
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

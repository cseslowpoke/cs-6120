#include "Analysis/dominator.h"
#include "Analysis/cfg.h"
#include <iostream>

DominatorTree::DominatorTree(json &body) {
  auto form = form_block(body);
  auto blockmap = BlockMap(form);
}

void DominatorTree::print() { std::cout << "not implemented yet" << std::endl; }

void DominatorTreePass::run(json &ir) {
  DominatorTree DT(ir);
  DT.print();
}

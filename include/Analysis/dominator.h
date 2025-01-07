/*
 * Construct dominance tree
 */
#pragma once

#include "PassBase.h"
#include <nlohmann/json.hpp>

class DominatorTree {
public:
  DominatorTree(json &body);
  void print();

private:
  std::vector<std::vector<std::string>> dom;
};

class DominatorTreePass : public PassBase {
public:
  void run(json &ir) override;
};

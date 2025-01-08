/*
 * Construct dominance tree
 */
#pragma once

#include "PassBase.h"
#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <string>

class DominatorTree {
public:
  DominatorTree(json &body);
  void print();

private:
  std::map<std::string, std::set<std::string>> dom;
};

class DominatorTreePass : public PassBase {
public:
  void run(json &ir) override;
};

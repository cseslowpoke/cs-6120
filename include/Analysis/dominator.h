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
  const std::map<std::string, std::set<std::string>> &getDom() { return dom; }
  const std::map<std::string, std::set<std::string>> &getFrontier() {
    return frontier;
  }

private:
  std::map<std::string, std::set<std::string>> dom;
  std::map<std::string, std::set<std::string>> frontier;
};

class DominatorTreePass : public PassBase {
public:
  void run(json &ir) override;
};

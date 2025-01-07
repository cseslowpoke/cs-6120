/*
 * Reaching definition analysis
 */
#pragma once
#include "Analysis/dffw.h"
#include "PassBase.h"
#include <nlohmann/json.hpp>
#include <set>

class ReachDefAnalysis
    : public DataFlowFramework<ReachDefAnalysis, std::set<std::string>> {
public:
  ReachDefAnalysis(json &body) : DataFlowFramework(body) {}
  std::set<std::string> transfer(std::set<std::string> in, const json &block);
  std::set<std::string> merge(std::set<std::string> &a,
                              std::set<std::string> &b);
  std::set<std::string> init();
  void print();
  bool equal(std::set<std::string> &a, std::set<std::string> &b);
};

class ReachDefAnalysisPass : public PassBase {
public:
  void run(json &ir) override;
};

#include "Analysis/reachdef.h"
#include <cstdio>

std::set<std::string> ReachDefAnalysis::transfer(std::set<std::string> out,
                                                 const json &block) {
  for (auto instr : block) {
    if (instr.contains("dest")) {
      out.insert(instr["dest"]);
    }
  }
  return out;
}

std::set<std::string> ReachDefAnalysis::merge(std::set<std::string> &a,
                                              std::set<std::string> &b) {
  std::set<std::string> out;
  std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                 std::inserter(out, out.begin()));
  return out;
}

std::set<std::string> ReachDefAnalysis::init() {
  return std::set<std::string>();
}

bool ReachDefAnalysis::equal(std::set<std::string> &a,
                             std::set<std::string> &b) {
  return a == b;
}

void ReachDefAnalysis::print() {
  for (size_t i = 0; i < in.size(); i++) {
    printf("Block %lu:\n", i);
    printf("In: ");
    for (auto &s : in[i]) {
      printf("%s ", s.c_str());
    }
    printf("\n");
    printf("Out: ");
    for (auto &s : out[i]) {
      printf("%s ", s.c_str());
    }
    printf("\n");
  }
}

void ReachDefAnalysisPass::run(json &ir) {
  for (auto &func : ir["functions"]) {
    ReachDefAnalysis rda(func["instrs"]);
    rda.print();
  }
}

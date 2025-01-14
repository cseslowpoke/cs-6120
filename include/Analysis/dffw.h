/*
 *  Data Flow Framework
 *  Using Curiously Recurring Template Pattern (CRTP)
 */
#pragma once
#include "Analysis/cfg.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

template <typename T, typename U> class DataFlowFramework {
protected:
  std::vector<U> in, out;

public:
  DataFlowFramework(json &body);
  // virtual U transfer(U &in, json &block) = 0;
  // virtual U merge(U &a, U &b) = 0;
  // virtual U init() = 0;
  // virtual bool equal(U &a, U &b) = 0;
};

template <typename T, typename U>
DataFlowFramework<T, U>::DataFlowFramework(json &body) {
  auto form = formBlock(body);
  auto function = BlockMap(form);
  buildCFG(function);
  std::vector<std::vector<int>> succ_cfg, pred_cfg;
  in.clear();
  out.clear();
  in.resize(function.size());
  out.resize(function.size());

  // Initialize worklist
  std::set<int> worklist;
  for (int i = 0; i < function.size(); i++) {
    worklist.insert(i);
  }
  in[0] = static_cast<T *>(this)->init();
  std::for_each(out.begin(), out.end(),
                [&](U &o) { o = static_cast<T *>(this)->init(); });
  while (!worklist.empty()) {
    int i = *worklist.begin();
    worklist.erase(i);
    for (auto &pred : pred_cfg[i]) {
      in[i] = static_cast<T *>(this)->merge(in[i], out[pred]);
    }
    U ori_out = out[i];
    out[i] = static_cast<T *>(this)->transfer(in[i], function[i]);
    if (!static_cast<T *>(this)->equal(ori_out, out[i])) {
      for (auto &succ : succ_cfg[i]) {
        worklist.insert(succ);
      }
    }
  }
}

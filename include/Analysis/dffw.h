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
  auto form = form_block(body);
  auto blockmap = BlockMap(form);
  auto edg_cfg = get_cfg(blockmap);
  std::vector<std::vector<int>> succ_cfg, pred_cfg;
  succ_cfg.resize(blockmap.size());
  pred_cfg.resize(blockmap.size());
  for (auto &[from, to_node] : edg_cfg) {
    for (auto &to : to_node) {
      succ_cfg[blockmap.getindex(from)].push_back(blockmap.getindex(to));
      pred_cfg[blockmap.getindex(to)].push_back(blockmap.getindex(from));
    }
  }
  in.clear();
  out.clear();
  in.resize(blockmap.size());
  out.resize(blockmap.size());

  // Initialize worklist
  std::set<int> worklist;
  for (int i = 0; i < blockmap.size(); i++) {
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
    out[i] = static_cast<T *>(this)->transfer(in[i], blockmap[i]);
    if (!static_cast<T *>(this)->equal(ori_out, out[i])) {
      for (auto &succ : succ_cfg[i]) {
        worklist.insert(succ);
      }
    }
  }
}

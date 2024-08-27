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
  auto blocks = form_block(body);
  auto name2block = block_map(blocks);
  std::map<std::string, int> name2index;
  for (size_t i = 0; i < name2block.size(); i++) {
    name2index[name2block[i].first] = i;
  }
  auto edg_cfg = get_cfg(name2block);
  std::vector<std::vector<int>> succ_cfg, pred_cfg;
  succ_cfg.resize(name2block.size());
  pred_cfg.resize(name2block.size());
  for (auto &[from, to] : edg_cfg) {
    succ_cfg[name2index[from]].push_back(name2index[to]);
    pred_cfg[name2index[to]].push_back(name2index[from]);
  }
  in.clear();
  out.clear();
  in.resize(name2block.size());
  out.resize(name2block.size());

  // Initialize worklist
  std::set<int> worklist;
  for (int i = 0; i < name2block.size(); i++) {
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
    out[i] = static_cast<T *>(this)->transfer(in[i], name2block[i].second);
    if (!static_cast<T *>(this)->equal(ori_out, out[i])) {
      for (auto &succ : succ_cfg[i]) {
        worklist.insert(succ);
      }
    }
  }
}
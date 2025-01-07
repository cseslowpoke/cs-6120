/*
 * Control Flow Graph
 */
#pragma once
#include <map>
#include <nlohmann/json.hpp>
#include <set>

using json = nlohmann::json;
static std::set<std::string> terminator = {"jmp", "br", "ret"};

json form_block(json &body);

std::map<std::string, json> block_map(json &body);

class BlockMap {
public:
  BlockMap(json &body);
  const json &operator[](std::string index) const {
    return order[index_map.at(index)];
  }
  const json &operator[](size_t index) const { return order[index]; }
  const size_t size() const { return order.size(); }
  const std::string &getname(size_t index) const { return name_map.at(index); }
  const size_t getindex(std::string name) const { return index_map.at(name); }

private:
  std::vector<json> order;
  std::map<std::string, size_t> index_map;
  std::map<size_t, std::string> name_map;
};

std::map<std::string, std::vector<std::string>> get_cfg(BlockMap &blockmap);

// void cfg();

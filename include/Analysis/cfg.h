/*
 * Control Flow Graph
 */
#pragma once
#include <deque>
#include <map>
#include <nlohmann/json.hpp>
#include <set>

using json = nlohmann::json;
static std::set<std::string> terminator = {"jmp", "br", "ret"};

class BasicBlock {
public:
  BasicBlock(json &block) : Block(block) {}
  json &Data() { return Block; }
  std::vector<BasicBlock *> &getPred() { return Predecessors; }
  void addPred(BasicBlock *BB) { Predecessors.push_back(BB); };
  std::vector<BasicBlock *> &getSucc() { return Successors; }
  void addSucc(BasicBlock *BB) { Successors.push_back(BB); };
  std::string &getName() { return Name; }
  void setName(std::string &_name) { Name = _name; }

private:
  json Block;
  std::string Name;
  std::vector<BasicBlock *> Predecessors;
  std::vector<BasicBlock *> Successors;
};

class BlockMap {
public:
  BlockMap(std::vector<BasicBlock> &body);
  BasicBlock &operator[](std::string index) {
    return order[index_map.at(index)];
  }
  BasicBlock &operator[](size_t index) { return order.data()[index]; }
  const size_t size() const { return order.size(); }
  const std::string &getname(size_t index) const { return name_map.at(index); }
  const size_t getindex(std::string name) const { return index_map.at(name); }

  BasicBlock &getEntry() { return order[0]; };

private:
  std::vector<BasicBlock> order;
  std::map<std::string, size_t> index_map;
  std::map<size_t, std::string> name_map;
};

std::vector<BasicBlock> formBlock(json &body);

void buildCFG(BlockMap &blockmap);

void addEntry();

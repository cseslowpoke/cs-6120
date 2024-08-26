#pragma once
#include <nlohmann/json.hpp>
#include <set>

using json = nlohmann::json;
static std::set<std::string> terminator = {"jmp", "br", "ret"};

json form_block(json &body);

std::vector<std::pair<std::string, json>> block_map(json &body);

std::vector<std::pair<std::string, std::string> > get_cfg(std::vector<std::pair<std::string, json>> &blockmap);

void cfg();
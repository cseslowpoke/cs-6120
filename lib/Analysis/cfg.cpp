#include "Analysis/cfg.h"

json form_block(json &body) {
    json func = json::array();
    json cur_block = json::array();
    for (auto instr: body) {
        if (instr.contains("op")) {
            cur_block.push_back(instr);
            if (terminator.find(instr["op"]) != terminator.end()) {
                // std::cout << "Block: " << cur_block.dump(-1) << std::endl;
                func.push_back(cur_block);
                cur_block.clear();
            }
        }
        else {
            if (!cur_block.empty()) {
                // std::cout << "Block: " << cur_block.dump(-1) << std::endl;
                func.push_back(cur_block);
                cur_block.clear();
            }
            cur_block.push_back(instr);
        } 
    }
    if (!cur_block.empty()) {
        // std::cout << "Block: " << cur_block.dump(-1) << std::endl;
        func.push_back(cur_block);
    }
    return func;
}

std::vector<std::pair<std::string, json>> block_map(json &body) {
    std::vector<std::pair<std::string, json>> name2block;
    for (auto &block: body) {
        std::string name;
        if (block[0].contains("label")) {
            name = block[0]["label"];
        }
        else {
            name = "b" + std::to_string(name2block.size());
        }
        name2block.push_back({name, block});
    }
    return name2block;
}

std::vector<std::pair<std::string, std::string> > get_cfg(std::vector<std::pair<std::string, json>> &blockmap) {
    std::vector<std::pair<std::string, std::string> > cfg;
    for (size_t i = 0; i < blockmap.size(); i++) {
        auto &[name, block] = blockmap[i];
        if (block.back().at("op") == "ret") {
            cfg.push_back({name, " "});
            continue;
        }
        if (block.back().at("op") == "jmp") {
            cfg.push_back({name, block.back().at("labels")[0]});
        }
        else if (block.back().back() == "br") {
            cfg.push_back({name, block.back().at("labels")[0]});
            cfg.push_back({name, block.back().at("labels")[1]});
        }
        else {
            if (i != blockmap.size()) {
                cfg.push_back({blockmap[i].first, blockmap[i+1].first});
            }
        }
    }
    return cfg;
}

void cfg(json prog) {
    for (auto func: prog["functions"]) {
        auto block = form_block(func["instrs"]);
        auto name2block = block_map(block);
        for (auto &[name, block]: name2block) {
            // std::cout << name << ": \n  " << block.dump(-1) << std::endl;
        }
        auto cfg = get_cfg(name2block);
        for (auto &[from, to]: cfg) {
            // std::cout << from << " -> " << to << std::endl;
        }
    }
}

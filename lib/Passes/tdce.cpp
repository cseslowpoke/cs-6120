#include "Passes/tdce.h"
#include "Analysis/cfg.h"

bool TDCE::simple_dce(json &body) {
    bool changed = false;
    std::unordered_map<std::string, int> name2pos;
    std::unordered_map<std::string, int> name2cnt;
    // std::priority_queue<std::string> pq;
    std::vector<int> to_remove;
    for (size_t i = 0; i < body.size(); i++) {
        if (body[i].contains("dest")) {
            name2pos[body[i]["dest"]] = (int)i; 
        }
        if (body[i].contains("args")) {
            for (auto &arg: body[i]["args"]) {
                name2cnt[arg] += 1;
            }
        }
    }
    for (auto &[k, v]: name2pos) {
        if (name2cnt[k] == 0) {
            // std::cerr << "Removing: " << v->dump(2) << std::endl;
            to_remove.push_back(v);
            changed = true;
        }
    }
    sort(to_remove.rbegin(), to_remove.rend());
    for (auto &pos: to_remove) {
        body.erase(body.begin() + pos);
    }
    return changed;
}

bool TDCE::unused_local(json &body) {
    bool changed = false;
    auto form = form_block(body);
    for (auto &block: form) {
        std::unordered_map<std::string, int> name2pos;
        std::vector<int> to_remove;
        for (size_t i = 0; i < block.size(); i++) {
            auto &instr = block[i];
            if (instr.contains("args")) {
                for (auto &arg: instr["args"]) {
                    if (name2pos.find(arg) != name2pos.end()) {
                        name2pos.erase(arg);
                    }
                }
            }
            if (instr.contains("dest") ) {
                if (name2pos.find(instr["dest"]) != name2pos.end()) {
                    to_remove.push_back(name2pos[instr["dest"]]);
                }
                name2pos[instr["dest"]] = (int)i;
            }
        }
        sort(to_remove.begin(), to_remove.end());
        for (auto &pos: to_remove) {
            block.erase(block.begin() + pos);
            changed = true;
        }
    }
    // flatten the blocks
    body.clear();
    for (auto &block: form) {
        for (auto &instr: block) {
            body.push_back(instr);
        }
    }
    return changed;
}

void TDCE::run(json &ir) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &func: ir["functions"]) {
            changed |= simple_dce(func["instrs"]);
            changed |= unused_local(func["instrs"]);
        }
    }
}
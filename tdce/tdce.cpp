#include <cmath>
#include <cstddef>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <set>
using json = nlohmann::json;

static std::set<std::string> terminator = {"jmp", "br", "ret"};

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

bool simple_dce(json &body) {
    bool changed = false;
    std::unordered_map<std::string, json::iterator> name2pos;
    std::unordered_map<std::string, int> name2cnt;
    // std::priority_queue<std::string> pq;
    for (int i = 0; i < body.size(); i++) {
        if (body[i].contains("dest")) {
            name2pos[body[i]["dest"]] = body.begin() + i; 
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
            body.erase(v);
            changed = true;
        }
    }
    return changed;
}

bool unused_local(json &body) {
    bool changed = false;
    auto form = form_block(body);
    for (auto &block: form) {
        std::unordered_map<std::string, int> name2pos;
        std::vector<int> to_remove;
        for (size_t i = 0; i < block.size(); i++) {
            auto &instr = block[i];
            if (instr.contains("dest") ) {
                if (name2pos.find(instr["dest"]) != name2pos.end()) {
                    to_remove.push_back(name2pos[instr["dest"]]);
                }
                name2pos[instr["dest"]] = i;
            }
            if (instr.contains("args")) {
                for (auto &arg: instr["args"]) {
                    if (name2pos.find(arg) != name2pos.end()) {
                        name2pos.erase(arg);
                    }
                }
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

int main(){
    json prog = json::parse(std::cin);
    for (auto &func: prog["functions"]) {
        while(simple_dce(func["instrs"]) || unused_local(func["instrs"])) {
            // std::cout << "Changed: " << func["instrs"].dump(2) << std::endl;
        }
    }
    std::cout << prog.dump(2) << std::endl;
    return 0;
}
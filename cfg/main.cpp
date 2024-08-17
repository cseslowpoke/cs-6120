#include <iostream>
#include <string>
#include <set>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

static std::set<std::string> terminator = {"jmp", "br", "ret"};

void form_block(json &body) {
    json cur_block = json::array();
    for (auto instr: body) {
        if (instr.contains("op")) {
            cur_block.push_back(instr);
            if (terminator.find(instr["op"]) != terminator.end()) {
                std::cout << "Block: " << cur_block.dump(-1) << std::endl;
                cur_block.clear();
            }
        }
        else {
            if (!cur_block.empty()) {
                std::cout << "Block: " << cur_block.dump(-1) << std::endl;
                cur_block.clear();
            }
            cur_block.push_back(instr);
        }
    }
    if (!cur_block.empty()) {
        std::cout << "Block: " << cur_block.dump(-1) << std::endl;
    }
}
void cfg() {
    json prog = json::parse(std::cin);
    for (auto func: prog["functions"]) {
        form_block(func["instrs"]);
    }
}
int main() {
    cfg();
    return 0;
}
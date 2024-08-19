#include <iostream>
#include <nlohmann/json.hpp>
#include <queue>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

void simple_dce(json &body) {
    std::unordered_map<std::string, json::iterator> name2pos;
    std::unordered_map<std::string, int> name2cnt;
    std::priority_queue<std::string> pq;
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
            body.erase(v);
        }
    }
}

void dce(json &body) {

}

int main(){
    json prog = json::parse(std::cin);
    for (auto &func: prog["functions"]) {
        simple_dce(func["instrs"]);
    }
    std::cout << prog.dump(2) << std::endl;
    return 0;
}
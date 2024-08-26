#include "Passes/lvn.h"
#include "Analysis/cfg.h"

void LVN::lvn(json &block) {
    std::unordered_map<std::string, std::string> name2name;
    std::unordered_map<std::string, int> name2val;
    std::map<std::tuple<Operator::Op, std::string, std::string>, std::string> op2name;
    for (size_t i = 0; i < block.size(); i++) {
        auto &instr = block[i];
        if (instr.contains("args")) {
            for (auto &arg: instr["args"]) {
                if (name2name.find(arg) != name2name.end()) {
                    arg = name2name[arg];
                }
            }
        }
        if (instr.contains("dest")) {
            auto &name = instr["dest"];
            auto &op = instr["op"];
            if (op == "const") {
                name2name[name] = name;
                // std
                name2val[name] = instr["value"];
            }
            else if (op == "id") {
                if (name2name.find(instr["args"][0]) == name2name.end()) 
                    name2name[name] = instr["args"][0];
                else
                    name2name[name] = name2name[instr["args"][0]];
            }
            else if (op == "add" || op == "sub" || op == "mul" || op == "div") {
                auto &lhs = instr["args"][0];
                auto &rhs = instr["args"][1];
                if (name2val.find(lhs) != name2val.end() && name2val.find(rhs) != name2val.end()) {
                    name2name[name] = name;
                    if (op == "add") {
                        name2val[name] = name2val[lhs] + name2val[rhs];
                    }
                    else if (op == "sub") {
                        name2val[name] = name2val[lhs] - name2val[rhs];
                    }
                    else if (op == "mul") {
                        name2val[name] = name2val[lhs] * name2val[rhs];
                    }
                    else if (op == "div") {
                        if (name2val[rhs] == 0) {
                            continue;
                        }
                        name2val[name] = name2val[lhs] / name2val[rhs];
                    }
                    instr = {
                        {"op", "const"},
                        {"dest", name},
                        {"type", "int"},
                        {"value", name2val[name]}
                    };
                }
                else {
                    Operator oper(op);
                    if (oper.op == Operator::Op::ADD || oper.op == Operator::Op::MUL) {
                        if (lhs < rhs) {
                            std::swap(lhs, rhs);
                        }
                    }
                    if (op2name.find({oper.op, lhs, rhs}) != op2name.end()) {
                        name2name[name] = op2name[{oper.op, lhs, rhs}];
                    }
                    else {
                        op2name[{oper.op, lhs, rhs}] = name2name[name];
                        name2name[name] = name;
                    }
                }
            }
        }
    }
}

void LVN::run(nlohmann::json &ir) {
    for (auto &func: ir["functions"]) {
        auto &instrs = func["instrs"];
        auto form = form_block(instrs);
        for (auto &block: form) {
            lvn(block);
        }
        instrs.clear();
        for (auto &block: form) {
            for (auto &instr: block) {
                instrs.push_back(instr);
            }
        }
    }
}
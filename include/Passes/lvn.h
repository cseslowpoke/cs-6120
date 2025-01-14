#pragma once
#include "Analysis/cfg.h"
#include "PassBase.h"

class LVN : public PassBase {
  class Operator {
  public:
    enum class Op { ADD, SUB, MUL, DIV, CONST, ID } op;
    Operator(std::string str) {
      if (str == "add") {
        op = Op::ADD;
      } else if (str == "sub") {
        op = Op::SUB;
      } else if (str == "mul") {
        op = Op::MUL;
      } else if (str == "div") {
        op = Op::DIV;
      } else if (str == "const") {
        op = Op::CONST;
      } else if (str == "id") {
        op = Op::ID;
      }
    }
  };
  void lvn(BasicBlock &block);

public:
  void run(nlohmann::json &ir) override;
};

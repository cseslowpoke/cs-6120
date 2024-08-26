#pragma once

#include <istream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;
class PassBase {
public:
    virtual ~PassBase() = default;

    // must be implemented by derived classes
    virtual void run(json &ir) = 0;

    // reads input from input stream, runs the pass and writes output to output stream
    void execute(std::istream &input, std::ostream &output) {
        json ir;
        ir = json::parse(input);
        run(ir);
        output << ir.dump(2);
    }
};
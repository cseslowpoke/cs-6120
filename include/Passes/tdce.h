#pragma once
#include "PassBase.h"

class TDCE: public PassBase {
bool simple_dce(json &body);
bool unused_local(json &body);
public:
    void run(json &ir) override;  
};
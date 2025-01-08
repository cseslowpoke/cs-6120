#include "PassBase.h"
#include <nlohmann/json.hpp>

class intoSSA : public PassBase {
public:
  void run(json &ir) override;
};

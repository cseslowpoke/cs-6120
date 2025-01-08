#include "PassBase.h"
#include <nlohmann/json.hpp>

class outSSA : PassBase {
public:
  void run(json &ir) override;
};

add_executable(cfg main.cpp)
target_link_libraries(cfg PRIVATE nlohmann_json::nlohmann_json)
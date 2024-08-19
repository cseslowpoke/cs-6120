add_executable(cfg cfg.cpp)
target_link_libraries(cfg PRIVATE nlohmann_json::nlohmann_json)
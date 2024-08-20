add_executable(lvn lvn.cpp)
target_link_libraries(lvn PRIVATE nlohmann_json::nlohmann_json)
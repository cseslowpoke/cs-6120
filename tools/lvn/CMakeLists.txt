add_executable(lvn main.cpp)

target_link_libraries(lvn PRIVATE PassLib nlohmann_json::nlohmann_json)
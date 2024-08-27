add_executable(reachdef main.cpp)

target_link_libraries(reachdef PRIVATE PassLib nlohmann_json::nlohmann_json)
add_executable(dominator main.cpp)

target_link_libraries(dominator PRIVATE PassLib nlohmann_json::nlohmann_json)


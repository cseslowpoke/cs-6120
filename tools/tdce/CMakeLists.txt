add_executable(tdce main.cpp)

target_link_libraries(tdce PRIVATE PassLib nlohmann_json::nlohmann_json)
add_executable(intoSSA intoSSA.cpp)

target_link_libraries(intoSSA PRIVATE PassLib nlohmann_json::nlohmann_json)

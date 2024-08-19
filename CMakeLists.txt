cmake_minimum_required(VERSION 3.10)

project(cs6120-tasks)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 找到 nlohmann_json 庫
find_package(nlohmann_json 3.2.0 REQUIRED)

add_subdirectory(cfg)
add_subdirectory(tdce)
# add_executable(my_executable main.cpp)

# # 將 nlohmann_json 庫鏈接到可執行文件中
# target_link_libraries(my_executable PRIVATE nlohmann_json::nlohmann_json)
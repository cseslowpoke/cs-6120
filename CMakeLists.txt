cmake_minimum_required(VERSION 3.10)

project(cs6120-tasks)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 找到 nlohmann_json 庫
find_package(nlohmann_json 3.2.0 REQUIRED)

# 包含頭文件
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加 lib/ 目錄下的庫
add_library(PassLib STATIC
    ${PROJECT_SOURCE_DIR}/lib/Analysis/cfg.cpp
    ${PROJECT_SOURCE_DIR}/lib/Analysis/dominator.cpp
    ${PROJECT_SOURCE_DIR}/lib/Analysis/reachdef.cpp
    ${PROJECT_SOURCE_DIR}/lib/Passes/tdce.cpp
    ${PROJECT_SOURCE_DIR}/lib/Passes/lvn.cpp
    ${PROJECT_SOURCE_DIR}/lib/Passes/intoSSA.cpp
)

target_link_libraries(PassLib PRIVATE nlohmann_json::nlohmann_json)

# target_link_libraries(PassLib Public ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(tools/tdce)
add_subdirectory(tools/lvn)
add_subdirectory(tools/reachdef)
add_subdirectory(tools/ssa)


# test_cfg
add_executable(test_cfg ${PROJECT_SOURCE_DIR}/tests/test_cfg.cpp)
target_link_libraries(test_cfg PRIVATE PassLib)

# test_dominator
add_executable(test_dom ${PROJECT_SOURCE_DIR}/tests/test_dom.cpp)
target_link_libraries(test_dom PRIVATE PassLib)

# # # 將 nlohmann_json 庫鏈接到可執行文件中
# # target_link_libraries(my_executable PRIVATE nlohmann_json::nlohmann_json)

cmake_minimum_required(VERSION 3.31.5)

project(Sliser VERSION 1.0 LANGUAGES CXX)

find_package(Eigen3 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(sliser src/main.cpp src/sliser.cpp)

target_link_libraries(sliser Eigen3::Eigen)


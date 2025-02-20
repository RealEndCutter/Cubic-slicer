cmake_minimum_required(VERSION 3.31.5)

project(Sliser VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
find_package(Gnuplot REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/externals/gnuplot-iostream)

file(GLOB SRCS src/*.cpp)
add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})


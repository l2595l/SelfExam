cmake_minimum_required(VERSION 3.14)
project(patrol_scheduler CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_library(patrol_scheduler src/patrol_scheduler.cpp)
target_include_directories(patrol_scheduler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(run_tests tests/test_main.cpp)
target_link_libraries(run_tests PRIVATE patrol_scheduler)

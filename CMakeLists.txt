cmake_minimum_required(VERSION 3.20)

project(proxy_scheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(proxy_scheduler
    control-plane/src/main.cpp
)

target_include_directories(proxy_scheduler PRIVATE
    ${Boost_INCLUDE_DIRS}
)

target_compile_options(proxy_scheduler PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
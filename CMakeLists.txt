cmake_minimum_required(VERSION 3.16)
project(QuantEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(quant
    src/main.cpp
    src/csv_data_feed.cpp
    src/portfolio.cpp
    src/moving_average_strategy.cpp
    src/trade_log.cpp
    src/performance.cpp
)

target_include_directories(quant PRIVATE include)

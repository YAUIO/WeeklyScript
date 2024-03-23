cmake_minimum_required(VERSION 3.27)
project(WeeklyScript)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)

FetchContent_Declare(
        boost
        GIT_REPOSITORY https://github.com/boostorg/boost
        GIT_TAG boost-1.84.0
)

FetchContent_MakeAvailable(boost)
FetchContent_MakeAvailable(fmt)

add_executable(WeeklyScript main.cpp)
TARGET_LINK_LIBRARIES(WeeklyScript fmt)
TARGET_LINK_LIBRARIES(WeeklyScript boost)
add_executable(WeeklyScriptGetC mainGet.cpp)
TARGET_LINK_LIBRARIES(WeeklyScriptGetC fmt)

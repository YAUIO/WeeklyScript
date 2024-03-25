cmake_minimum_required(VERSION 3.25)
project(WeeklyScript)
set(CMAKE_CXX_STANDARD 20)
set(Boost_NO_WARN_NEW_VERSIONS 1)
include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)

add_executable(WeeklyScript main.cpp)
TARGET_LINK_LIBRARIES(WeeklyScript fmt)

add_executable(WeeklyScriptGetC mainGet.cpp)
TARGET_LINK_LIBRARIES(WeeklyScriptGetC fmt)

cmake_minimum_required(VERSION 3.27)
project(WeeklyScript)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(fmt)

find_package(Boost 1.84.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(WeeklyScript main.cpp)

TARGET_LINK_LIBRARIES(WeeklyScript fmt)
target_link_libraries(WeeklyScript ${Boost_LIBRARIES})


add_executable(WeeklyScriptGetC mainGet.cpp)
TARGET_LINK_LIBRARIES(WeeklyScriptGetC fmt)

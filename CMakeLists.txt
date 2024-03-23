cmake_minimum_required(VERSION 3.27)
project(WeeklyScript)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)

set(Boost_INCLUDE_DIR "D:/Soft/Programs/CPPPPKG/boost")
set(Boost_LIBRARY_DIR "D:/Soft/Programs/CPPPPKG/boost/lib")
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(WeeklyScript main.cpp)
TARGET_LINK_LIBRARIES(WeeklyScript fmt)

add_executable(WeeklyScriptGetC mainGet.cpp)
TARGET_LINK_LIBRARIES(WeeklyScriptGetC fmt)

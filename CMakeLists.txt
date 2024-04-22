cmake_minimum_required(VERSION 3.27)
project(WeeklyScript)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
set(BUILD_SHARED_LIBS FALSE)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(fmt)

FETCHCONTENT_DECLARE(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG 2.6.1
)

FETCHCONTENT_MAKEAVAILABLE(SFML)

add_executable(WeeklyScript main.cpp
        ReplayParser.cpp
        ReplayParser.h
        setget.cpp
        setget.h
        action.cpp
        action.h
        step.cpp
        step.h)
TARGET_LINK_LIBRARIES(WeeklyScript fmt)
target_link_libraries(WeeklyScript sfml-graphics)

add_executable(WeeklyScriptGetC mainGet.cpp)
TARGET_LINK_LIBRARIES(WeeklyScriptGetC fmt)

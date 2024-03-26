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

#FetchContent_Declare(
#        MediaInfoLib
#        GIT_REPOSITORY https://github.com/MediaArea/MediaInfoLib
#        GIT_TAG v24.01
#)

#FetchContent_Declare(
#        ZenLib1
#        GIT_REPOSITORY https://github.com/MediaArea/ZenLib
#        GIT_TAG v0.4.41
#)

#FetchContent_Declare(
#        zlib1
#        GIT_REPOSITORY https://github.com/madler/zlib/
#        GIT_TAG v1.3.1
#)

FetchContent_MakeAvailable(fmt)
#FetchContent_MakeAvailable(MediaInfoLib)
#FetchContent_MakeAvailable(ZenLib1)
#FetchContent_MakeAvailable(zlib1)
#set(${CMAKE_CURRENT_SOURCE_DIR} cmake-build-debug/)
#add_subdirectory(cmake-build-debug/_deps/mediainfolib-src/Project/CMake)
#add_subdirectory(cmake-build-debug/_deps/zenlib-src/Project/CMake)

add_executable(WeeklyScript main.cpp)
TARGET_LINK_LIBRARIES(WeeklyScript fmt)
#target_link_directories(WeeklyScript PUBLIC cmake-build-debug/_deps/mediainfolib-src/Source/MediaInfo)
#target_link_libraries(WeeklyScript MediaInfoLib)
#target_link_libraries(WeeklyScript ZenLib)

add_executable(WeeklyScriptGetC mainGet.cpp)
TARGET_LINK_LIBRARIES(WeeklyScriptGetC fmt)

cmake_minimum_required(VERSION 3.6)
project(GGJ)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
        Sources/Curve.cpp
        Sources/Player.cpp
        Sources/Timer.cpp
        Sources/Game.cpp
        Sources/gameover.cpp
        Sources/menu.cpp)
add_executable(GGJ ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(GGJ ${SFML_LIBRARIES})
endif()
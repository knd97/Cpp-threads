cmake_minimum_required(VERSION 3.10)
project(SO_2 VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -Wno-missing-braces -pedantic")

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Curses REQUIRED)
find_package(Threads  REQUIRED)

set(SOURCES
    src/Ball.cpp
    src/Screen.cpp
    src/Setup_scene.cpp
    src/Window.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
include_directories(${CURSES_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
target_link_libraries(${PROJECT_NAME} Threads::Threads)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
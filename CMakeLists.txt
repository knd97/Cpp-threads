cmake_minimum_required(VERSION 3.10)
project(SO_1 VERSION 1.0.0)

find_package(Curses REQUIRED)

if(Curses_FOUND)
    message("Curses found")
else()
    message(FATAL_ERROR "Cannot find Curses")
endif()

set(SOURCES
    src/Ball.cpp
    src/Screen.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(${CURSES_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
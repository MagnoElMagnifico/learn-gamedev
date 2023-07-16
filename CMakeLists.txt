cmake_minimum_required(VERSION 3.0.0)
project(
    LearnGameDev
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Weffc++
    -Wconversion
    -Wsign-conversion
)

# TODO: find SFML

#### 000-test ####
add_executable(test 000-test/main.cpp)
target_link_libraries(test sfml-graphics sfml-window sfml-system)

#### 001-hexchess ####
file(GLOB_RECURSE hexchess_src 001-hexchess/*.cpp)
add_executable(hexchess ${hexchess_src})
target_link_libraries(hexchess sfml-graphics sfml-window sfml-audio sfml-system)

# TODO: future projects may overwrite this assets folder
# Copy assets
file(COPY 001-hexchess/assets DESTINATION ${CMAKE_BINARY_DIR})

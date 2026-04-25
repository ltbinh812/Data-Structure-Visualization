cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

project(data-structure-visualizer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(SFML REQUIRED COMPONENTS Graphics Window System Audio)
find_package(ImGui-SFML REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/headers)

add_executable(data-structure-visualizer
    src/main.cpp 
    src/entity.cpp
    src/draw.cpp
    src/handleEvent.cpp
    src/theme.cpp
    src/highlight.cpp
    src/graphPhysics.cpp
    src/importFile.cpp
    src/visualization1.cpp
    src/performVisualization1.cpp
    src/visualization2.cpp
    src/performVisualization2.cpp
    src/visualization3.cpp
    src/performVisualization3.cpp
    src/visualization4.cpp
    src/performVisualization4.cpp
    src/visualization5.cpp
    src/performVisualization5.cpp
    src/visualization6.cpp
    src/performVisualization6.cpp

)

# target_compile_definitions(data-structure-visualizer PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")

target_link_libraries(data-structure-visualizer PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio
    ImGui-SFML::ImGui-SFML
)


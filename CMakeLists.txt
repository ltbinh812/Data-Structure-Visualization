cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

project(DataStructureVisualization)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(SFML 2.5 COMPONENTS graphics window system audio network REQUIRED)
find_package(ImGui-SFML REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(DataStructureVisualization
    src/main.cpp 
    
)

target_link_libraries(DataStructureVisualization PRIVATE 
    sfml-graphics
    sfml-window
    sfml-system
    ImGui-SFML::ImGui-SFML
)


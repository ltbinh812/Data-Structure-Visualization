cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

project(data-structure-visualization)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(SFML REQUIRED COMPONENTS Graphics Window System Audio)
find_package(ImGui-SFML REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(data-structure-visualization
    src/main.cpp 
    
)

target_link_libraries(data-structure-visualization PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio
    ImGui-SFML::ImGui-SFML
)


#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

extern bool isDarkMode;
extern ImVec4 title1Color;
extern ImVec4 title2Color;
extern sf::Color edgeColor;
extern sf::Color outlineColor;
sf::Color applyTheme(bool isDark, ImGuiStyle& style);

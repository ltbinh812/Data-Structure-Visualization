#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>

#include "entity.h"

void drawMainMenu(sf::RenderWindow& window, std::vector<Text>& texts, std::vector<Block>& buttons, std::vector<Image>& images, std::vector<Card>& cards);

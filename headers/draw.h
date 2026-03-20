#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>

#include "entity.h"

enum OperationType{
    INITIALIZE,
    ADD,
    DELETE,
    UPDATE,
    SEARCH,
    GRAPH
};
extern OperationType o;
extern Block* Log;
extern float delayLog;

void setLog(std::string text, sf::Color color = sf::Color(244, 164, 96, 230));
void drawMainMenu(sf::RenderWindow& window, std::vector<Text>& texts, std::vector<Block>& buttons, std::vector<Image>& images, std::vector<Card>& cards);
void drawVisualization1(sf::RenderWindow& window);
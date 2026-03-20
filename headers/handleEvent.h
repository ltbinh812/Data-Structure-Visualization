#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "entity.h"

void handleMainMenuEvents(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView, std::vector<Card>& cards);
void handleVisualization1Events(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView);
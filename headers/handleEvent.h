#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "entity.h"

void handleMainMenuEvents(sf::Event event, sf::RenderWindow& window, sf::View& cameraView, std::vector<Card>& cards);
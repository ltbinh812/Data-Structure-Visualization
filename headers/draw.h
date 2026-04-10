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
    GET_TOP,
    GRAPH
};
extern OperationType o;
extern Block* Log;
extern float delayLog;
extern Block* newNode;
extern bool isStepByStep;   
extern int choosePrevNextButton;
extern bool firstTime;

void setLog(std::string text, sf::Color color = sf::Color(244, 164, 96, 230), sf::Color textColor = sf::Color::Black);
void drawLog(sf::RenderWindow& window);
bool checkMove(Block *Node);
void drawMainMenu(sf::RenderWindow& window, std::vector<Text>& texts, std::vector<Block>& buttons, std::vector<Image>& images, std::vector<Card>& cards);
void drawVisualization1(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color& sfmlBgColor);
void drawVisualization2(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color& sfmlBgColor);
void drawVisualization3(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color& sfmlBgColor);
void drawVisualization4(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color& sfmlBgColor);
// void drawVisualization5(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color& sfmlBgColor);
// void drawVisualization6(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color& sfmlBgColor);
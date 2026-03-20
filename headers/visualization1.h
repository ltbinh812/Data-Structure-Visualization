#pragma once
#include <vector>
#include "entity.h"


extern std::vector<Block*> linkedList;


void initVisualization1(sf::RenderWindow& window);
void insertVisualization1(sf::RenderWindow& window);
void deleteVisualization1(sf::RenderWindow& window);
void updateVisualization1(sf::RenderWindow& window);
void searchVisualization1(sf::RenderWindow& window);
void drawList(sf::RenderWindow& window);
void drawLog(sf::RenderWindow& window);

extern bool isWaiting;
extern float delayTimer;
bool checkMove(Block *Node);
bool checkNextStep(float limitTime);

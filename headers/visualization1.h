#pragma once
#include <vector>
#include "entity.h"



void initStatus1();
void initVisualization1(sf::RenderWindow& window);
void insertVisualization1(sf::RenderWindow& window);
void deleteVisualization1(sf::RenderWindow& window);
void updateVisualization1(sf::RenderWindow& window);
void searchVisualization1(sf::RenderWindow& window);
void drawLinkedList(sf::RenderWindow& window);

extern std::vector<Block*> linkedList;
extern float dtV1;

bool checkNextStepV1(float limitTime);

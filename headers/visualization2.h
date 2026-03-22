#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "visualization2.h"
#include "performVisualization2.h"
#include "entity.h"

extern std::vector<Block*> heapList;
extern float dtV2;
extern bool isMaxHeap;

sf::Vector2f calculatePos(int i, int n);

bool checkNextStepV2(float limitTime);
void drawHeapList(sf::RenderWindow& window);

void initVisualization2(sf::RenderWindow& window);
void insertVisualization2(sf::RenderWindow& window);
void updateVisualization2(sf::RenderWindow& window);
void getTopVisualization2(sf::RenderWindow& window);
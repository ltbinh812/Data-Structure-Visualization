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
extern std::vector<Block*> garbageV2;



void initStatus2();

sf::Vector2f calculatePos(int i, int n);

bool checkNextStepV2(float limitTime, Block* newNode, std::vector<Block*>& heapList);
void drawHeapList(sf::RenderWindow& window, std::vector<Block*>& heapList);

void initVisualization2(sf::RenderWindow& window);
void insertVisualization2(sf::RenderWindow& window);
void updateVisualization2(sf::RenderWindow& window);
void getTopVisualization2(sf::RenderWindow& window);
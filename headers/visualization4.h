#pragma once


#include <vector>
#include "entity.h"
#include "main.h"

extern Block* rootV4;
extern float dtV4;
extern std::vector<Block*> garbageV4;

void initStatus4();
float calculateTrieGapWidth(Block* node);
void calculateTriePos(Block* node, float x = WINDOW_WIDTH / 2.f, float y = 300.f);

void initVisualization4(sf::RenderWindow& window);
void insertVisualization4(sf::RenderWindow& window);
void deleteVisualization4(sf::RenderWindow& window);
void searchVisualization4(sf::RenderWindow& window);

bool checkNextStepV4(float dt, Block* rootV4, Block* newNode);
void drawTrie(Block* node, sf::RenderWindow& window, Block* cloneRootV4);
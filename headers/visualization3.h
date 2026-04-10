#pragma once


#pragma once
#include <vector>
#include "entity.h"

enum class RotationType {
    NONE,
    LEFT_LEFT,    // Xoay Phải (Single Right)
    RIGHT_RIGHT,  // Xoay Trái (Single Left)
    LEFT_RIGHT,   // Xoay Kép (Trái rồi Phải)
    RIGHT_LEFT    // Xoay Kép (Phải rồi Trái)
};

void initStatus3();

void calculateAllPos(Block* node, int heightTree, int level,  float x, float y);
int getHeight(Block* node);
void setColorALVTree(Block* node, sf::Color color = sf::Color::White);
void initVisualization3(sf::RenderWindow& window);
void insertVisualization3(sf::RenderWindow& window);
void deleteVisualization3(sf::RenderWindow& window);
void searchVisualization3(sf::RenderWindow& window);
void drawAVLTree(Block* node, sf::RenderWindow& window);
void clearALVTree(Block* node);

extern Block* newNode;
extern Block* rootV3;
extern float dtV3;
extern std::vector<Block*> garbageV3;

bool checkNextStepV3(float limitTime, Block* rootV3, Block* newNode);

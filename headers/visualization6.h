#pragma once


#include <vector>
#include "entity.h"
#include "main.h"

extern float dtV6;

void initStatus6();

void initVisualization6(sf::RenderWindow& window);
void kruskalVisualization6(sf::RenderWindow& window);

bool checkNextStepV6(float dt);
void drawKruskal(std::vector<Block*>& list, sf::RenderWindow& window, std::vector<int> &visitedEdgeV6, std::vector<bool> &visitedNodeV6, int id = -1, sf::Color e = sf::Color::White);

extern std::vector<std::pair<int,std::pair<int,int>>> adjListV6;
extern std::vector<int> visitedEdgeV6;
extern std::vector<bool> visitedNodeV6;
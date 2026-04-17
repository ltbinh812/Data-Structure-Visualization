#pragma once


#include <vector>
#include "entity.h"
#include "main.h"

extern float dtV5;

void initStatus5();

void initVisualization5(sf::RenderWindow& window);
void dijkstraVisualization5(sf::RenderWindow& window);

bool checkNextStepV5(float dt);
void drawDijkstra(std::vector<Block*>& list, sf::RenderWindow& window, int u = -1, int i = -1, sf::Color e = sf::Color::White);

extern std::vector<std::vector<std::pair<int,int>>> adjListV5;
extern std::vector<int> distV5;
extern std::vector<std::pair<int,int>> prevV5;
extern std::vector<bool> visitedV5;
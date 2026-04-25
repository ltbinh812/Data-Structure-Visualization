#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional> 
#include "entity.h" 

class GraphPhysicsManager {
private:
    std::vector<Block*> nodes;
    Block* draggedNode = nullptr;

    const float NODE_RADIUS = 40.0f;     // ban kinh chuot
    const float SAFE_DISTANCE = 300.0f;  // khoang cach day nhau
    const float PUSH_STRENGTH = 3.5f;   // force push
    
    const float BASE_BOX_SIZE = 750.0f;  
    const int NODES_PER_EXPANSION = 8;  
    const float EXPANSION_STEP = 300.0f; 

public:
    void clearNodes();
    void addNode(Block* node);
    void setNodes(const std::vector<Block*>& newNodes);
    std::vector<Block*>& getNodes();
    
    sf::FloatRect getCurrentBounds();
    float getDistance(sf::Vector2f a, sf::Vector2f b);
    sf::Vector2f getSafeSpawnPosition();
    
    void updatePhysics(float dt);
    void handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window, sf::View& cameraView);
    void drawBounds(sf::RenderWindow& window);
};

extern GraphPhysicsManager graphPhysics;
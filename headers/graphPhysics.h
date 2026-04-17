#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional> // Cần cho std::optional của SFML 3.0
#include "entity.h" // Nhúng class Block của bạn vào đây

class GraphPhysicsManager {
private:
    std::vector<Block*> nodes;
    Block* draggedNode = nullptr;

    // Các thông số tinh chỉnh
    const float NODE_RADIUS = 40.0f;     // Bán kính click chuột
    const float SAFE_DISTANCE = 300.0f;  // Khoảng cách đẩy nhau
    const float PUSH_STRENGTH = 3.5f;   // Lực đẩy
    
    // Cấu hình khung đỏ (Bounding Box)
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
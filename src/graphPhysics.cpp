#include "graphPhysics.h"
#include <cmath>
#include <cstdlib>
#include <utility> 


GraphPhysicsManager graphPhysics;

void GraphPhysicsManager::clearNodes() {
    for(Block* node : nodes) {
        delete node; 
    }
    nodes.clear();
    draggedNode = nullptr;
}

void GraphPhysicsManager::addNode(Block* node) {
    draggedNode = nullptr; 
    nodes.push_back(node);
    sf::Vector2f safePos = getSafeSpawnPosition();
    node->setPosition({safePos.x, safePos.y});
}

void GraphPhysicsManager::setNodes(const std::vector<Block*>& newNodes) {
    clearNodes();
    for (Block* node : newNodes) {
        nodes.push_back(node);
    }
    for(Block* node : nodes) {
        sf::Vector2f safePos = getSafeSpawnPosition();
        node->setPosition({safePos.x, safePos.y});
    }
}


std::vector<Block*>& GraphPhysicsManager::getNodes() {
    return nodes;
}

sf::FloatRect GraphPhysicsManager::getCurrentBounds() {
    int expansionLevel = nodes.size() / NODES_PER_EXPANSION;
    float currentDim = BASE_BOX_SIZE + (expansionLevel * EXPANSION_STEP);
    
    return sf::FloatRect{
        sf::Vector2f{300.0f, 50.0f}, 
        sf::Vector2f{currentDim, currentDim}
    };
}

float GraphPhysicsManager::getDistance(sf::Vector2f a, sf::Vector2f b) {
    sf::Vector2f diff = a - b;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

sf::Vector2f GraphPhysicsManager::getSafeSpawnPosition() {
    sf::FloatRect bounds = getCurrentBounds();
    int maxAttempts = 1000;

    for (int i = 0; i < maxAttempts; ++i) {
        float randX = bounds.position.x + static_cast<float>(rand() % static_cast<int>(bounds.size.x));
        float randY = bounds.position.y + static_cast<float>(rand() % static_cast<int>(bounds.size.y));
        sf::Vector2f newPos{randX, randY};

        bool isSafe = true;
        for (Block* node : nodes) {
            sf::Vector2f nodePos{node->getPosition().first, node->getPosition().second};
            
            if (getDistance(newPos, nodePos) < SAFE_DISTANCE) {
                isSafe = false;
                break; 
            }
        }
        if (isSafe) return newPos;
    }
    
    return sf::Vector2f{0.0f, 0.0f}; 
}

void GraphPhysicsManager::updatePhysics(float dt) {
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            Block* a = nodes[i];
            Block* b = nodes[j];

            sf::Vector2f posA{a->getPosition().first, a->getPosition().second};
            sf::Vector2f posB{b->getPosition().first, b->getPosition().second};
            float dist = getDistance(posA, posB);

            if (dist < SAFE_DISTANCE && dist > 0.001f) {
                sf::Vector2f pushDir = (posB - posA) / dist;
                float overlap = SAFE_DISTANCE - dist;
                
                sf::Vector2f force = pushDir * overlap * PUSH_STRENGTH * dt;

                if (a == draggedNode) {
                    b->setPosition({posB.x + force.x, posB.y + force.y});
                }
                else if (b == draggedNode) {
                    a->setPosition({posA.x - force.x, posA.y - force.y});
                }
                else {
                    a->setPosition({posA.x - force.x * 0.5f, posA.y - force.y * 0.5f});
                    b->setPosition({posB.x + force.x * 0.5f, posB.y + force.y * 0.5f});
                }
            }
        }
    }

    sf::FloatRect bounds = getCurrentBounds();
    for (Block* node : nodes) {
        sf::Vector2f pos{node->getPosition().first, node->getPosition().second};
        bool clamped = false;

        float minX = bounds.position.x;
        float maxX = bounds.position.x + bounds.size.x;
        float minY = bounds.position.y;
        float maxY = bounds.position.y + bounds.size.y;

        if (pos.x < minX) { pos.x = minX; clamped = true; }
        if (pos.x > maxX) { pos.x = maxX; clamped = true; }
        if (pos.y < minY) { pos.y = minY; clamped = true; }
        if (pos.y > maxY) { pos.y = maxY; clamped = true; }

        if (clamped) {
            node->setPosition({pos.x, pos.y});
        }
    }
}

void GraphPhysicsManager::handleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window, sf::View& cameraView) {
    if (!event) return;

    if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (pressed->button == sf::Mouse::Button::Left) {
            draggedNode = nullptr;
            sf::Vector2f mousePos = window.mapPixelToCoords(pressed->position, cameraView);
            for (Block* node : nodes) {
                sf::Vector2f nodePos{node->getPosition().first, node->getPosition().second};
                
                if (getDistance(mousePos, nodePos) <= NODE_RADIUS) {
                    draggedNode = node;
                    break;
                }
            }
        }
    }

    if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            draggedNode = nullptr;
        }
    }

    if (const auto* moved = event->getIf<sf::Event::MouseMoved>()) {
        if (draggedNode != nullptr) {
            if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                draggedNode = nullptr;
            } 
            else {
                sf::Vector2f mousePos = window.mapPixelToCoords(moved->position, cameraView);
                draggedNode->setPosition({mousePos.x, mousePos.y});
            }
        }
    }
}

void GraphPhysicsManager::drawBounds(sf::RenderWindow& window) {
    sf::FloatRect bounds = getCurrentBounds();
    
    sf::RectangleShape box(sf::Vector2f{bounds.size.x, bounds.size.y});
    box.setPosition(sf::Vector2f{bounds.position.x, bounds.position.y});
    
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color(255, 50, 50, 150)); 
    box.setOutlineThickness(3.0f);
    
    window.draw(box);
}
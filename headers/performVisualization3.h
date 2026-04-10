#pragma once


#include <SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"



enum class StepTypeV3 { INITIALIZE, NEW_NODE, INSERT, DELETE, TRAVERSE, UPDATE, SEARCH, FINISH, ROTATE_LEFT_LEFT, ROTATE_RIGHT_RIGHT, HIGHLIGHT_1};

struct AnimationStepV3 {
    std::vector<int> activeLines;         // Dòng code highlight tương ứng
    Block* focusNode;     // Node đang được xét tới 
    int focusNodeVal;     // Giá trị node đang được xét tới
    std::string log;      // Lời nhắn hiện trên UI
    StepTypeV3 type;
    Block* focusAnotherNode;
    Block* deletedNode;
    Block* setWhiteNode = nullptr;
    sf::Color historyColor = sf::Color::White;
    
    AnimationStepV3(std::vector<int> activeLines, Block* focusNode, int focusNodeVal, std::string log, StepTypeV3 type, Block* focusAnotherNode = nullptr, Block* deletedNode = nullptr) {
        this->activeLines = activeLines;
        this->focusNode = focusNode;
        this->focusNodeVal = focusNodeVal;
        this->log = log;
        this->type = type;
        this->focusAnotherNode = focusAnotherNode;
        this->deletedNode = deletedNode;        
    }
};

class cloneVisualization3 {
private:
    Block* cloneNodeV3(Block* originalNode, std::map<Block*, Block*>& visited);
public:
    Block* rootV3 = nullptr;
    Block* newNode = nullptr;
    Block* outDummyRoot = nullptr;
    Block* outDummyNewNode = nullptr;
    std::map<Block* , Block*> mapping;

    cloneVisualization3(Block* rootV3, Block* newNode, const AnimationStepV3& stepV3);   
    void pull(std::vector<Block*>& dummySet, 
              std::map<Block*, Block*>& liveToDummyMap);
    ~cloneVisualization3();
};

extern std::vector<AnimationStepV3> scriptV3;
extern int currentStepIdxV3;
extern std::vector<cloneVisualization3*> historyV3;
extern bool isCalculatingHistoryV3;
extern std::vector<Block*> dummySetV3;
extern std::map<Block*, Block*> liveToDummyMapV3;

bool checkFinishedV3();
void performVisualization3(sf::RenderWindow& window);
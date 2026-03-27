#pragma once


#include <SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"



enum class StepTypeV3 { INITIALIZE, NEW_NODE, INSERT, DELETE, TRAVERSE, UPDATE, SEARCH, FINISH, HIGHLIGHT, ROTATE_LEFT_LEFT, ROTATE_RIGHT_RIGHT};

struct AnimationStepV3 {
    int codeLine;         // Dòng code highlight tương ứng
    Block* focusNode;     // Node đang được xét tới 
    int focusNodeVal;     // Giá trị node đang được xét tới
    std::string log;      // Lời nhắn hiện trên UI
    StepTypeV3 type;
    Block* focusAnotherNode;
    Block* deletedNode;
    std::vector<Block*> AffectedNodes;

    AnimationStepV3(int codeLine, Block* focusNode, int focusNodeVal, std::string log, StepTypeV3 type, Block* focusAnotherNode = nullptr, Block* deletedNode = nullptr) {
        this->codeLine = codeLine;
        this->focusNode = focusNode;
        this->focusNodeVal = focusNodeVal;
        this->log = log;
        this->type = type;
        this->focusAnotherNode = focusAnotherNode;
        this->deletedNode = deletedNode;        
    }
};

extern std::vector<AnimationStepV3> scriptV3;
extern int currentStepIdxV3;

bool checkFinishedV3();
void performVisualization3(sf::RenderWindow& window);
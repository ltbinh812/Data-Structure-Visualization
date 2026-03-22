#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"



enum class StepTypeV2 { INITIALIZE, NEW_NODE, INSERT, DELETE_1, DELETE_2, TRAVERSE, UPDATE, SEARCH_1, SEARCH_2, FINISH, HIGHLIGHT, COMPARE, SWAP_DOWN, SWAP_UP, MOVE };

struct AnimationStepV2 {
    int codeLine;         // Dòng code highlight tương ứng
    int focusNodeIdx;     // Node đang được xét tới 
    int focusNodeVal;     // Giá trị node đang được xét tới
    std::string log;      // Lời nhắn hiện trên UI
    StepTypeV2 type;
    int focusAnotherNodeIdx;

    AnimationStepV2(int codeLine, int focusNodeIdx, int focusNodeVal, std::string log, StepTypeV2 type, int focusAnotherNodeIdx = -1) {
        this->codeLine = codeLine;
        this->focusNodeIdx = focusNodeIdx;
        this->focusNodeVal = focusNodeVal;
        this->log = log;
        this->type = type;
        this->focusAnotherNodeIdx = focusAnotherNodeIdx;        
    }
};

extern std::vector<AnimationStepV2> scriptV2;
extern int currentStepIdxV2;
extern Block* newNode; 

bool checkFinishedV2();
void performVisualization2(sf::RenderWindow& window);
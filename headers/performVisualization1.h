#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "entity.h"

enum class StepTypeV1 { NONE, NEW_NODE, INSERT, DELETE_1, DELETE_2, TRAVERSE, UPDATE, SEARCH_1, SEARCH_2, FINISH, HIGHLIGHT };

struct AnimationStepV1 {
    int codeLine;         // Dòng code highlight tương ứng
    int focusNodeIdx;     // Node đang được xét tới 
    int focusNodeVal;     // Giá trị node đang được xét tới
    std::string log;      // Lời nhắn hiện trên UI
    StepTypeV1 type;
};

extern std::vector<AnimationStepV1> scriptV1;
extern int currentStepIdxV1;
extern Block* newNode; 

bool checkFinishedV1();
void performVisualization1(sf::RenderWindow& window);
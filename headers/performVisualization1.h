#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "entity.h"

enum class StepType { NONE, NEW_NODE, INSERT, DELETE_1, DELETE_2, TRAVERSE, UPDATE, SEARCH_1, SEARCH_2, FINISH, HIGHLIGHT };

struct AnimationStep {
    int codeLine;         // Dòng code highlight tương ứng
    int focusNodeIdx;     // Node đang được xét tới 
    int focusNodeVal;     // Giá trị node đang được xét tới
    std::string log;      // Lời nhắn hiện trên UI
    StepType type;
};

extern int currentStepIdx;
extern std::vector<AnimationStep> script;
extern Block* newNode; // Node chờ thực hiện (dùng cho insert)
extern float dt;

void performInsertVisualization1(sf::RenderWindow& window);
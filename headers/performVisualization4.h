#pragma once


#include <SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"


enum class StepTypeV4 { INITIALIZE, NEW_NODE, INSERT, DELETE, TRAVERSE, UPDATE, SEARCH, FINISH, HIGHLIGHT, ROTATE_LEFT_LEFT, ROTATE_RIGHT_RIGHT};

struct AnimationStepV4 {
    int codeLine;         // Dòng code highlight tương ứng
    Block* focusNode;     // Node đang được xét tới 
    int focusNodeVal;     // Giá trị node đang được xét tới
    std::string log;      // Lời nhắn hiện trên UI
    StepTypeV4 type;
    Block* focusAnotherNode;
    bool isFinished = false;

    AnimationStepV4(int codeLine, Block* focusNode, int focusNodeVal, std::string log, StepTypeV4 type, Block* focusAnotherNode = nullptr) {
        this->codeLine = codeLine;
        this->focusNode = focusNode;
        this->focusNodeVal = focusNodeVal;
        this->log = log;
        this->type = type;
        this->focusAnotherNode = focusAnotherNode;
    }

};

extern std::vector<AnimationStepV4> scriptV4;
extern int currentStepIdxV4;

void performVisualization4(sf::RenderWindow& window);
bool checkFinishedV4();

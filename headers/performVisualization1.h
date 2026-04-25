#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "entity.h"

enum class StepTypeV1 { NONE, NEW_NODE, INSERT, DELETE_1, DELETE_2, TRAVERSE, UPDATE, SEARCH_1, SEARCH_2, FINISH, HIGHLIGHT_1, HIGHLIGHT_2, HIGHLIGHT_3, HIGHLIGHT_4};

struct AnimationStepV1 {
    std::vector<int> activeLines;         
    int focusNodeIdx;      
    int focusNodeVal;     
    std::string log;      
    StepTypeV1 type;
};

struct cloneVisualization1 {
    std::vector<Block*> linkedList;
    Block* newNode = nullptr;
    cloneVisualization1(std::vector<Block*> linkedList, Block* newNode);
    void pull(std::vector<Block*>& linkedList, Block*& newNode);
    ~cloneVisualization1();
};

extern std::vector<AnimationStepV1> scriptV1;
extern int currentStepIdxV1;
extern Block* newNode; 
extern std::vector<cloneVisualization1*> historyV1;
extern bool isCalculatingHistoryV1;

bool checkFinishedV1();
void performVisualization1(sf::RenderWindow& window);
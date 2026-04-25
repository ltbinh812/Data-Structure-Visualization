#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"



enum class StepTypeV2 { INITIALIZE, NEW_NODE, INSERT, DELETE_1, DELETE_2, TRAVERSE, UPDATE, SEARCH_1, SEARCH_2, FINISH, HIGHLIGHT, COMPARE, SWAP_DOWN, SWAP_UP, MOVE, HIGHLIGHT_1, HIGHLIGHT_2 };

struct AnimationStepV2 {
    std::vector<int> activeLines;
    int focusNodeIdx;     
    int focusNodeVal;    
    std::string log;     
    StepTypeV2 type;
    int focusAnotherNodeIdx;
    int setWhiteNodeIdx = -1;

    AnimationStepV2(std::vector<int> activeLines, int focusNodeIdx, int focusNodeVal, std::string log, StepTypeV2 type, int focusAnotherNodeIdx = -1) {
        this->activeLines = activeLines;
        this->focusNodeIdx = focusNodeIdx;
        this->focusNodeVal = focusNodeVal;
        this->log = log;
        this->type = type;
        this->focusAnotherNodeIdx = focusAnotherNodeIdx;        
    }
};

class cloneVisualization2 {
private:
    Block* cloneNodeV2(Block* originalNode, std::map<Block*, Block*>& visited);
    Block* newNode = nullptr;
    std::vector<Block*> cloneHeapList;
public:
    // Block* outDummyRoot = nullptr;
    Block* outDummyNewNode = nullptr;
    std::map<Block* , Block*> mapping;

    cloneVisualization2(Block* newNode, const AnimationStepV2& stepV2);   
    void pull(std::vector<Block*>& dummySet, 
              std::map<Block*, Block*>& liveToDummyMap);
    ~cloneVisualization2();
};


extern std::vector<AnimationStepV2> scriptV2;
extern int currentStepIdxV2;
extern Block* newNode; 
extern std::vector<cloneVisualization2*> historyV2;
extern bool isCalculatingHistoryV2;
extern std::vector<Block*> dummySetV2;
extern std::map<Block*, Block*> liveToDummyMapV2;

bool checkFinishedV2();
void performVisualization2(sf::RenderWindow& window);
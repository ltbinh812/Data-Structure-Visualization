#pragma once


#include <SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"


enum class StepTypeV4 { INITIALIZE, NEW_NODE, INSERT, DELETE, TRAVERSE, UPDATE, SEARCH, FINISH, HIGHLIGHT_1, HIGHLIGHT_2};

struct AnimationStepV4 {
    std::vector<int> activeLines;         // Dòng code highlight tương ứng
    Block* focusNode;     // Node đang được xét tới 
    int focusNodeVal;     // Giá trị node đang được xét tới
    std::string log;      // Lời nhắn hiện trên UI
    StepTypeV4 type;
    Block* focusAnotherNode;
    bool isFinished = false;

    AnimationStepV4(std::vector<int> activeLines, Block* focusNode, int focusNodeVal, std::string log, StepTypeV4 type, Block* focusAnotherNode = nullptr) {
        this->activeLines = activeLines;
        this->focusNode = focusNode;
        this->focusNodeVal = focusNodeVal;
        this->log = log;
        this->type = type;
        this->focusAnotherNode = focusAnotherNode;
    }

};


class cloneVisualization4 {
private:
    Block* cloneNodeV4(Block* originalNode, std::map<Block*, Block*>& visited);
public:
    Block* rootV4 = nullptr;
    Block* newNode = nullptr;
    Block* outDummyRoot = nullptr;
    Block* outDummyNewNode = nullptr;
    std::map<Block* , Block*> mapping;

    cloneVisualization4(Block* rootV4, Block* newNode, const AnimationStepV4& stepV4);   
    void pull(std::vector<Block*>& dummySet, 
              std::map<Block*, Block*>& liveToDummyMap);
    ~cloneVisualization4();
};



extern std::vector<AnimationStepV4> scriptV4;
extern int currentStepIdxV4;
extern std::vector<cloneVisualization4*> historyV4;
extern bool isCalculatingHistoryV4;
extern std::vector<Block*> dummySetV4;
extern std::map<Block*, Block*> liveToDummyMapV4;

void performVisualization4(sf::RenderWindow& window);
bool checkFinishedV4();

#pragma once


#include <SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"


enum class StepTypeV5 { INITIALIZE, CHOOSE_EDGE, NOT_CHOOSE_EDGE, CHOOSE_VERTEX, TRAVERSE_VERTEX, TRAVERSE_EDGE, FINISH, HIGHLIGHT_1};

struct AnimationStepV5 {
    std::vector<int> activeLines;
    int focusNode1;
    int focusNode2; 
    std::string log;
    StepTypeV5 type;
    int focusAnotherNode;

    AnimationStepV5(std::vector<int> activeLines, int focusNode1, int focusNode2, std::string log, StepTypeV5 type, int focusAnotherNode = -1) {
        this->activeLines = activeLines;
        this->focusNode1 = focusNode1;
        this->focusNode2 = focusNode2;
        this->log = log;
        this->type = type;
        this->focusAnotherNode = focusAnotherNode;
    }

};


class cloneVisualization5 {
private:
    std::vector<int> distSnapshot;
    std::vector<std::pair<int,int>> prevSnapshot;
    std::vector<sf::Color> nodeColors;
    std::vector<bool> visitedSnapshot;
    
public:
    cloneVisualization5(const std::vector<int>& liveDist, 
                        const std::vector<std::pair<int,int>>& livePrev,
                        const std::vector<bool>& liveVisited);

    void pull(std::vector<int>& liveDist, 
              std::vector<std::pair<int,int>>& livePrev,
              std::vector<bool>& liveVisited, 
              std::vector<Block*>& liveNodes);

    ~cloneVisualization5() = default; 
};



extern std::vector<AnimationStepV5> scriptV5;
extern int currentStepIdxV5;
extern std::vector<cloneVisualization5*> historyV5;
extern bool isCalculatingHistoryV5;

void performVisualization5(sf::RenderWindow& window);
bool checkFinishedV5();

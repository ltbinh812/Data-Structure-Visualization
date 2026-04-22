#pragma once


#include <SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"


enum class StepTypeV6 { INITIALIZE, CHOOSE_EDGE, NOT_CHOOSE_EDGE, CHOOSE_VERTEX, TRAVERSE_VERTEX, TRAVERSE_EDGE, FINISH, HIGHLIGHT_1};

struct AnimationStepV6 {
    std::vector<int> activeLines;         // Dòng code highlight tương ứng
    int focusNode1;     // Node đang được xét tới 
    int focusNode2;     // Giá trị node đang được xét tới
    std::string log;      // Lời nhắn hiện trên UI
    StepTypeV6 type;
    int focusAnotherNode;

    AnimationStepV6(std::vector<int> activeLines, int focusNode1, int focusNode2, std::string log, StepTypeV6 type, int focusAnotherNode = -1) {
        this->activeLines = activeLines;
        this->focusNode1 = focusNode1;
        this->focusNode2 = focusNode2;
        this->log = log;
        this->type = type;
        this->focusAnotherNode = focusAnotherNode;
    }

};


class cloneVisualization6 {
private:
    std::vector<int> visitedEdgeSnapshot;
    std::vector<sf::Color> nodeColors;
    std::vector<bool> visitedNodeSnapshot;
    
public:
    cloneVisualization6(const std::vector<int>& liveVisitedEdge,
                        const std::vector<bool>& liveVisitedNode);

    void pull(std::vector<int>& liveVisitedEdge,
              std::vector<bool>& liveVisitedNode, 
              std::vector<Block*>& liveNodes);

    ~cloneVisualization6() = default; 
};



extern std::vector<AnimationStepV6> scriptV6;
extern int currentStepIdxV6;
extern std::vector<cloneVisualization6*> historyV6;
extern bool isCalculatingHistoryV6;

void performVisualization6(sf::RenderWindow& window);
bool checkFinishedV6();

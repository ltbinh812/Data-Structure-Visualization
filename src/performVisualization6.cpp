#include "math.h"
#include "visualization6.h"
#include "performVisualization6.h"
#include "main.h"
#include "entity.h"
#include "draw.h"
#include "highlight.h"
#include <climits>
#include "graphPhysics.h"

std::vector<AnimationStepV6> scriptV6;
int currentStepIdxV6 = -1;
bool isCalculatingHistoryV6 = false;
std::vector<cloneVisualization6*> historyV6;     

cloneVisualization6::cloneVisualization6(const std::vector<int>& liveVisitedEdge,
                                         const std::vector<bool>& liveVisitedNode) 
{
    this->visitedEdgeSnapshot = liveVisitedEdge;
    this->visitedNodeSnapshot = liveVisitedNode;
    std::vector<Block*>& liveNodes = graphPhysics.getNodes();
     
    for (Block* node : liveNodes) {
        this->nodeColors.push_back(node->getFillColor()); 
    }
}

void cloneVisualization6::pull(std::vector<int>& liveVisitedEdge,
                               std::vector<bool>& liveVisitedNode, 
                               std::vector<Block*>& liveNodes) 
{ 

    liveVisitedEdge = this->visitedEdgeSnapshot;
    liveVisitedNode = this->visitedNodeSnapshot;

    for (size_t i = 0; i < liveNodes.size(); ++i) {
        Block* node = liveNodes[i];
        node->setFillColor(this->nodeColors[i]);
    }
    
}

void runV6(sf::RenderWindow& window) {
    AnimationStepV6 step = scriptV6[currentStepIdxV6];


    if(step.type == StepTypeV6::TRAVERSE_EDGE) {
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        if(firstTime){
            firstTime = false;
        }
        for(int i = 0; i < nodes.size(); i++){
            if(visitedNodeV6[i] == true) nodes[i] -> setFillColor(sf::Color::Green);
            else nodes[i] -> setFillColor(sf::Color::White);
        }
        if(!visitedNodeV6[adjListV6[step.focusNode1].second.first]) nodes[adjListV6[step.focusNode1].second.first] -> setFillColor(sf::Color::Yellow);
        if(!visitedNodeV6[adjListV6[step.focusNode1].second.second]) nodes[adjListV6[step.focusNode1].second.second] -> setFillColor(sf::Color::Yellow);
        drawKruskal(nodes, window, visitedEdgeV6, visitedNodeV6, step.focusNode1, sf::Color::Yellow);
        if(isCalculatingHistoryV6 || checkNextStepV6(1.4f)){
            currentStepIdxV6++;
            if(!isCalculatingHistoryV6) historyV6[currentStepIdxV6]->pull(visitedEdgeV6, visitedNodeV6, graphPhysics.getNodes());
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV6::CHOOSE_EDGE) {
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        if(firstTime){
            firstTime = false;
            visitedEdgeV6[step.focusNode1] = 1;
            visitedNodeV6[adjListV6[step.focusNode1].second.first] = true;
            visitedNodeV6[adjListV6[step.focusNode1].second.second] = true;
        }
        for(int i = 0; i < nodes.size(); i++){
            if(visitedNodeV6[i] == true) nodes[i] -> setFillColor(sf::Color::Green);
            else nodes[i] -> setFillColor(sf::Color::White);
        }
        drawKruskal(nodes, window, visitedEdgeV6, visitedNodeV6);
        if(isCalculatingHistoryV6 || checkNextStepV6(1.4f)){
            currentStepIdxV6++;
            if(!isCalculatingHistoryV6) historyV6[currentStepIdxV6]->pull(visitedEdgeV6, visitedNodeV6, graphPhysics.getNodes());
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV6::NOT_CHOOSE_EDGE) {
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        if(firstTime){
            firstTime = false;
            visitedEdgeV6[step.focusNode1] = -1;
        }
        for(int i = 0; i < nodes.size(); i++){
            if(visitedNodeV6[i] == true) nodes[i] -> setFillColor(sf::Color::Green);
            else nodes[i] -> setFillColor(sf::Color::White);
        }
        drawKruskal(nodes, window, visitedEdgeV6, visitedNodeV6);
        if(isCalculatingHistoryV6 || checkNextStepV6(1.4f)){
            currentStepIdxV6++;
            if(!isCalculatingHistoryV6) historyV6[currentStepIdxV6]->pull(visitedEdgeV6, visitedNodeV6, graphPhysics.getNodes());
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV6::HIGHLIGHT_1){
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        if(firstTime){
            firstTime = false;
        }
        for(int i = 0; i < nodes.size(); i++){
            if(visitedNodeV6[i] == true) nodes[i] -> setFillColor(sf::Color::Green);
            else nodes[i] -> setFillColor(sf::Color::White);
        }
        drawKruskal(nodes, window, visitedEdgeV6, visitedNodeV6);
        if(isCalculatingHistoryV6 || checkNextStepV6(1.4f)){
            currentStepIdxV6++;
            if(!isCalculatingHistoryV6) historyV6[currentStepIdxV6]->pull(visitedEdgeV6, visitedNodeV6, graphPhysics.getNodes());
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV6::FINISH){
        choosePrevNextButton = 0;
        firstTime = true;

    }


}

void getPosV6() {
    sf::FloatRect bounds = graphPhysics.getCurrentBounds();
    getRectangleMinMax({bounds.position.x, bounds.position.y});
    getRectangleMinMax({bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y});
}

void performVisualization6(sf::RenderWindow& window) {
    if(currentStepIdxV6 == -1 || currentStepIdxV6 >= scriptV6.size()) return;
    if(isCalculatingHistoryV6){
        resetRectangleMinMax();
        currentStepIdxV6 = 0;
        firstTime = true;
        while(currentStepIdxV6 + 1 < scriptV6.size()){
            historyV6.push_back(new cloneVisualization6(visitedEdgeV6, visitedNodeV6));
            getPosV6();
            if(newNode) getRectangleMinMax(newNode -> getPosition());
            runV6(window);
        }
        if(currentStepIdxV6 + 1 == scriptV6.size()) {
            historyV6.push_back(new cloneVisualization6(visitedEdgeV6, visitedNodeV6));
            getPosV6();
            if(newNode) getRectangleMinMax(newNode -> getPosition());
        }
        isCalculatingHistoryV6 = false;
        currentStepIdxV6 = 0;
        historyV6[0]->pull(visitedEdgeV6, visitedNodeV6, graphPhysics.getNodes());
        firstTime = true;
    }
    if(isStepByStep) {
        if(choosePrevNextButton == -1 && currentStepIdxV6 > 0){
            currentStepIdxV6--;
            historyV6[currentStepIdxV6]->pull(visitedEdgeV6, visitedNodeV6, graphPhysics.getNodes());
            choosePrevNextButton = 0;
            firstTime = true;
        }
        else if(choosePrevNextButton == -1){
            choosePrevNextButton = 0;
        }
        else if(choosePrevNextButton == 1 && currentStepIdxV6 + 1 < scriptV6.size()){
            drawCodeHighlightPanel(6, currentStepIdxV6, isStepByStep, checkFinishedV6(), scriptV6[currentStepIdxV6].activeLines);
            runV6(window);
        }
        else if(choosePrevNextButton == 1){
            choosePrevNextButton = 0;
        }
        if(choosePrevNextButton == 0){
            if(currentStepIdxV6 > 0)    drawCodeHighlightPanel(6, currentStepIdxV6 - 1, isStepByStep, checkFinishedV6(), scriptV6[currentStepIdxV6 - 1].activeLines);
            else                        drawCodeHighlightPanel(6, currentStepIdxV6 - 1, isStepByStep, checkFinishedV6(), {});
            if(currentStepIdxV6 > 0 && scriptV6[currentStepIdxV6 - 1].type == StepTypeV6::TRAVERSE_EDGE) drawKruskal(graphPhysics.getNodes(), window, visitedEdgeV6, visitedNodeV6, scriptV6[currentStepIdxV6 - 1].focusNode1, sf::Color::Yellow);
            else drawKruskal(graphPhysics.getNodes(), window, visitedEdgeV6, visitedNodeV6);
        }
    }
    else{
        if(currentStepIdxV6 > 0 && checkFinishedV6()) drawCodeHighlightPanel(6, currentStepIdxV6, isStepByStep, checkFinishedV6(), scriptV6[currentStepIdxV6 - 1].activeLines);
        else drawCodeHighlightPanel(6, currentStepIdxV6, isStepByStep, checkFinishedV6(), scriptV6[currentStepIdxV6].activeLines);
        runV6(window);
    }
}

bool checkFinishedV6() {
    return currentStepIdxV6 == -1 || currentStepIdxV6 + 1== scriptV6.size();
}


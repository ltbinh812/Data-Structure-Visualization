#include "math.h"
#include "visualization5.h"
#include "performVisualization5.h"
#include "main.h"
#include "entity.h"
#include "draw.h"
#include "highlight.h"
#include <climits>
#include "graphPhysics.h"

std::vector<AnimationStepV5> scriptV5;
int currentStepIdxV5 = -1;
bool isCalculatingHistoryV5 = false;
std::vector<cloneVisualization5*> historyV5;     

cloneVisualization5::cloneVisualization5(const std::vector<int>& liveDist, 
                                         const std::vector<std::pair<int,int>>& livePrev,
                                         const std::vector<bool>& liveVisited) 
{
    this->distSnapshot = liveDist;
    this->prevSnapshot = livePrev;
    this->visitedSnapshot = liveVisited;
    std::vector<Block*>& liveNodes = graphPhysics.getNodes();
     
    for (Block* node : liveNodes) {
        this->nodeColors.push_back(node->getFillColor()); 
    }
}

void cloneVisualization5::pull(std::vector<int>& liveDist, 
                               std::vector<std::pair<int,int>>& livePrev,
                                 std::vector<bool>& liveVisited, 
                               std::vector<Block*>& liveNodes) 
{ 
    std::cout << "$$$$$$$$$$$$ Pulling Dijkstra State by Index:\n";

    liveDist = this->distSnapshot;
    livePrev = this->prevSnapshot;
    liveVisited = this->visitedSnapshot;

    for (size_t i = 0; i < liveNodes.size(); ++i) {
        Block* node = liveNodes[i];
        node->setFillColor(this->nodeColors[i]);
        // if (liveDist[i] == INT_MAX) { 
        //     node->setSubLabel("INF"); 
        // } else {
        //     node->setSubLabel(std::to_string(liveDist[i]));
        // }
    }
    
}

void runV5(sf::RenderWindow& window) {
    AnimationStepV5 step = scriptV5[currentStepIdxV5];


    if(step.type == StepTypeV5::INITIALIZE) {
        std::cout << "initialize\n";
    }
    if(step.type == StepTypeV5::CHOOSE_EDGE) {
        std::cout << "CHOOSE_EDGE\n";
    }
    if(step.type == StepTypeV5::NOT_CHOOSE_EDGE) {
        std::cout << "NOT_CHOOSE_EDGE\n";
    }
    if(step.type == StepTypeV5::CHOOSE_VERTEX) {
        std::cout << "CHOOSE_VERTEX\n";
    }
    if(step.type == StepTypeV5::FINISH) {
        std::cout << "finish\n";
    }
    if(step.type == StepTypeV5::TRAVERSE_VERTEX) {
        std::cout << "TRAVERSE_VERTEX\n";
    }
    if(step.type == StepTypeV5::TRAVERSE_EDGE) {
        std::cout << "TRAVERSE_EDGE\n";
    }
    if(step.type == StepTypeV5::HIGHLIGHT_1) {
        std::cout << "HIGHLIGHT_1\n";
    }

    std::cout << scriptV5.size() << " " << currentStepIdxV5 << std::endl;


    // if(step.type == StepTypeV5::CHOOSE_VERTEX) {
    //     std::vector<Block*>& nodes = graphPhysics.getNodes();
    //     if(firstTime){
    //         firstTime = false;
    //         distV5[step.focusNode1] = 0;
    //     }
    //     for(int i = 0; i < nodes.size(); i++){
    //         if(distV5[i] == INT_MAX) nodes[i] -> setFillColor(sf::Color::White);
    //         else if(visitedV5[i] == false) nodes[i] -> setFillColor(sf::Color(127, 255, 212));
    //         else nodes[i] -> setFillColor(sf::Color::Green);
    //     }
    //     drawDijkstra(nodes, window);
    //     if(isCalculatingHistoryV5 || checkNextStepV5(1.f)){
    //         currentStepIdxV5++;
    //         if(!isCalculatingHistoryV5) historyV5[currentStepIdxV5]->pull(distV5, prevV5, visitedV5, graphPhysics.getNodes());
    //         choosePrevNextButton = 0;
    //         firstTime = true;
    //     }
    // }
    if(step.type == StepTypeV5::TRAVERSE_VERTEX) {
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        if(firstTime){
            firstTime = false;
            visitedV5[step.focusNode1] = true;
        }
        for(int i = 0; i < nodes.size(); i++){
            if(distV5[i] == INT_MAX) nodes[i] -> setFillColor(sf::Color::White);
            else if(visitedV5[i] == false) nodes[i] -> setFillColor(sf::Color(127, 255, 212));
            else nodes[i] -> setFillColor(sf::Color::Green);
        }
        nodes[step.focusNode1]->setFillColor(sf::Color::Yellow);
        drawDijkstra(nodes, window);
        if(isCalculatingHistoryV5 || checkNextStepV5(1.4f)){
            currentStepIdxV5++;
            if(!isCalculatingHistoryV5) historyV5[currentStepIdxV5]->pull(distV5, prevV5, visitedV5, graphPhysics.getNodes());
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    // else if(step.type == StepTypeV5::TRAVERSE_EDGE) {
    //     std::vector<Block*>& nodes = graphPhysics.getNodes();
    //     if(firstTime){
    //         firstTime = false;
    //     }
    //     for(int i = 0; i < nodes.size(); i++){
    //         if(distV5[i] == INT_MAX) nodes[i] -> setFillColor(sf::Color::White);
    //         else if(distV5[i] == 0) nodes[i] -> setFillColor(sf::Color::Blue);
    //         else nodes[i] -> setFillColor(sf::Color::Green);
    //     }
    //     nodes[step.focusNode1]->setFillColor(sf::Color::Yellow);
    //     nodes[step.focusNode2]->setFillColor(sf::Color(255, 193, 37));
    //     drawDijkstra(nodes, window, step.focusNode1, step.focusAnotherNode, sf::Color(255, 193, 37));
    //     if(isCalculatingHistoryV5 || checkNextStepV5(1.f)){
    //         currentStepIdxV5++;
    //         if(!isCalculatingHistoryV5) historyV5[currentStepIdxV5]->pull(distV5, prevV5, visitedV5, graphPhysics.getNodes());
    //         choosePrevNextButton = 0;
    //         firstTime = true;
    //     }
    // }
    else if(step.type == StepTypeV5::CHOOSE_EDGE) {
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        if(firstTime){
            firstTime = false;
            distV5[step.focusNode2] = distV5[step.focusNode1] + adjListV5[step.focusNode1][step.focusAnotherNode].second;
            prevV5[step.focusNode2] = {step.focusNode1, adjListV5[step.focusNode1][step.focusAnotherNode].second};
        }
        for(int i = 0; i < nodes.size(); i++){
            if(distV5[i] == INT_MAX) nodes[i] -> setFillColor(sf::Color::White);
            else if(visitedV5[i] == false) nodes[i] -> setFillColor(sf::Color(127, 255, 212));
            else nodes[i] -> setFillColor(sf::Color::Green);
        }
        nodes[step.focusNode1]->setFillColor(sf::Color::Yellow);
        nodes[step.focusNode2]->setFillColor(sf::Color(127, 255, 212));
        drawDijkstra(nodes, window);
        if(isCalculatingHistoryV5 || checkNextStepV5(1.4f)){
            currentStepIdxV5++;
            if(!isCalculatingHistoryV5) historyV5[currentStepIdxV5]->pull(distV5, prevV5, visitedV5, graphPhysics.getNodes());
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV5::NOT_CHOOSE_EDGE) {
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        if(firstTime){
            firstTime = false;
        }
        for(int i = 0; i < nodes.size(); i++){
            if(distV5[i] == INT_MAX) nodes[i] -> setFillColor(sf::Color::White);
            else if(visitedV5[i] == false) nodes[i] -> setFillColor(sf::Color(127, 255, 212));
            else nodes[i] -> setFillColor(sf::Color::Green);
        }
        nodes[step.focusNode1]->setFillColor(sf::Color::Yellow);
        nodes[step.focusNode2]->setFillColor(sf::Color::Green);
        drawDijkstra(nodes, window, step.focusNode1, step.focusAnotherNode, sf::Color::Red);
        if(isCalculatingHistoryV5 || checkNextStepV5(0.7f)){
            currentStepIdxV5++;
            if(!isCalculatingHistoryV5) historyV5[currentStepIdxV5]->pull(distV5, prevV5, visitedV5, graphPhysics.getNodes());
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV5::HIGHLIGHT_1){
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        if(firstTime){
            firstTime = false;
        }
        for(int i = 0; i < nodes.size(); i++){
            if(distV5[i] == INT_MAX) nodes[i] -> setFillColor(sf::Color::White);
            else if(visitedV5[i] == false) nodes[i] -> setFillColor(sf::Color(127, 255, 212));
            else nodes[i] -> setFillColor(sf::Color::Green);
        }
        drawDijkstra(nodes, window);
        if(isCalculatingHistoryV5 || checkNextStepV5(1.f)){
            currentStepIdxV5++;
            if(!isCalculatingHistoryV5) historyV5[currentStepIdxV5]->pull(distV5, prevV5, visitedV5, graphPhysics.getNodes());
            choosePrevNextButton = 0;
            firstTime = true;
        }        
    }
    else if(step.type == StepTypeV5::FINISH){
        choosePrevNextButton = 0;
        firstTime = true;

    }


}

void getPosV5() {
    sf::FloatRect bounds = graphPhysics.getCurrentBounds();
    getRectangleMinMax({bounds.position.x, bounds.position.y});
    getRectangleMinMax({bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y});
}

void performVisualization5(sf::RenderWindow& window) {
    if(currentStepIdxV5 == -1 || currentStepIdxV5 >= scriptV5.size()) return;
    if(isCalculatingHistoryV5){
        std::cout << "---" << minX << " " << maxX << " " << minY << " " << maxY << std::endl;
        resetRectangleMinMax();
        currentStepIdxV5 = 0;
        firstTime = true;
        while(currentStepIdxV5 + 1 < scriptV5.size()){
            historyV5.push_back(new cloneVisualization5(distV5, prevV5, visitedV5));
            getPosV5();
            if(newNode) getRectangleMinMax(newNode -> getPosition());
            runV5(window);
        }
        if(currentStepIdxV5 + 1 == scriptV5.size()) {
            historyV5.push_back(new cloneVisualization5(distV5, prevV5, visitedV5));
            getPosV5();
            if(newNode) getRectangleMinMax(newNode -> getPosition());
        }
        isCalculatingHistoryV5 = false;
        currentStepIdxV5 = 0;
        historyV5[0]->pull(distV5, prevV5, visitedV5, graphPhysics.getNodes());
        firstTime = true;
        std::cout << "---" << minX << " " << maxX << " " << minY << " " << maxY << std::endl;
    }
    // std::cout << "currentStepIdxV5: " << currentStepIdxV5 << " " << scriptV5.size() << std::endl;
    // std::cout << "size" << graphPhysics.getNodes().size() << std::endl;
    if(isStepByStep) {
        if(choosePrevNextButton == -1 && currentStepIdxV5 > 0){
            currentStepIdxV5--;
            std::cout << "before pull: \n";
            historyV5[currentStepIdxV5]->pull(distV5, prevV5, visitedV5, graphPhysics.getNodes());
            std::cout << "after pull: \n";
            choosePrevNextButton = 0;
            firstTime = true;
        }
        else if(choosePrevNextButton == -1){
            choosePrevNextButton = 0;
        }
        else if(choosePrevNextButton == 1 && currentStepIdxV5 + 1 < scriptV5.size()){
            drawCodeHighlightPanel(5, currentStepIdxV5, isStepByStep, checkFinishedV5(), scriptV5[currentStepIdxV5].activeLines);
            runV5(window);
        }
        else if(choosePrevNextButton == 1){
            choosePrevNextButton = 0;
        }
        if(choosePrevNextButton == 0){
            if(currentStepIdxV5 > 0)    drawCodeHighlightPanel(5, currentStepIdxV5 - 1, isStepByStep, checkFinishedV5(), scriptV5[currentStepIdxV5 - 1].activeLines);
            else                        drawCodeHighlightPanel(5, currentStepIdxV5 - 1, isStepByStep, checkFinishedV5(), {});
            if(currentStepIdxV5 > 0 && scriptV5[currentStepIdxV5 - 1].type == StepTypeV5::NOT_CHOOSE_EDGE) drawDijkstra(graphPhysics.getNodes(), window, scriptV5[currentStepIdxV5 - 1].focusNode1, scriptV5[currentStepIdxV5 - 1].focusAnotherNode, sf::Color::Red);
            else drawDijkstra(graphPhysics.getNodes(), window);
        }
    }
    else{
        if(currentStepIdxV5 > 0 && checkFinishedV5()) drawCodeHighlightPanel(5, currentStepIdxV5, isStepByStep, checkFinishedV5(), scriptV5[currentStepIdxV5 - 1].activeLines);
        else drawCodeHighlightPanel(5, currentStepIdxV5, isStepByStep, checkFinishedV5(), scriptV5[currentStepIdxV5].activeLines);
        runV5(window);
    }
}

bool checkFinishedV5() {
    std::cout << currentStepIdxV5 << " " << scriptV5.size() << std::endl;
    return currentStepIdxV5 == -1 || currentStepIdxV5 + 1== scriptV5.size();
}


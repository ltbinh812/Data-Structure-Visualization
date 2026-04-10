#include<SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"
#include "performVisualization2.h"
#include "visualization2.h"
#include "draw.h"   
#include "highlight.h"


std::vector<AnimationStepV2> scriptV2;

int currentStepIdxV2 = -1;
bool isCalculatingHistoryV2 = false;
std::vector<cloneVisualization2*> historyV2;     

std::vector<Block*> dummySetV2;
std::map<Block*, Block*> liveToDummyMapV2;
std::vector<Block*> cloneHeapListV2;


Block* cloneVisualization2::cloneNodeV2(Block* originalNode, std::map<Block*, Block*>& visited) {
    if (originalNode == nullptr) return nullptr;
    if (visited.find(originalNode) != visited.end()) {  
        return visited[originalNode];
    }
    Block* clonedNode = new Block(*originalNode);
    mapping[clonedNode] = originalNode;
    visited[originalNode] = clonedNode;
    std::cout << "clone node: " << clonedNode -> getLabel() << "\n";
    return clonedNode;
}
cloneVisualization2::cloneVisualization2(Block* newNode, const AnimationStepV2& stepV2) {    
    std::map<Block*, Block*> visited;
    std::cout << "-------------------------\n";
    this->newNode = cloneNodeV2(newNode, visited);
    for(auto const& child : heapList) {
        this->cloneHeapList.push_back(cloneNodeV2(child, visited));
    }




    std::cout << "*********************************\n";
}

void cloneVisualization2::pull(std::vector<Block*>& dummySet, 
                               std::map<Block*, Block*>& liveToDummyMap) 
{   
    // dummy node
    for (Block* dummy : dummySet) {
        delete dummy;
    }
    dummySet.clear();
    liveToDummyMap.clear();
    cloneHeapListV2.clear();

    std::map<Block*, Block*> cloneToDummy; 

    // clone -> dummy
    // live -> dummy
    std::cout << "$$$$$$$$$$$$pull:\n";
    for (auto const& [cloneNode, liveNode] : this->mapping) {
        Block* dummyNode = new Block(*cloneNode);
        std::cout << dummyNode << ".\n";
        std::cout << "dummy node: " << dummyNode -> getLabel() << "\n";
        dummySet.push_back(dummyNode);
        cloneToDummy[cloneNode] = dummyNode;         
        if (liveNode != nullptr) {
            liveToDummyMap[liveNode] = dummyNode; 
        }
    }

    for(auto const& node : this->cloneHeapList) {
        cloneHeapListV2.push_back(cloneToDummy[node]);
    }
    std::cout << "cloneHeapListV2 size: " << cloneHeapListV2.size() << "\n";
    this -> outDummyNewNode = (this->newNode != nullptr) ? cloneToDummy[this->newNode] : nullptr;
    std::cout << "outDummyNewNode: " << (this -> outDummyNewNode ? this -> outDummyNewNode -> getLabel() : "null") << "\n";
}

cloneVisualization2::~cloneVisualization2() {
    // clone to dummy
    for (auto const& pair : mapping) {
        delete pair.first; // clone node
    }
    mapping.clear();
}

bool checkFinishedV2(){
    if(currentStepIdxV2 == -1 || currentStepIdxV2 + 1>= scriptV2.size()) return true;
    return false;
}

void runV2(sf::RenderWindow& window){
    AnimationStepV2& step = scriptV2[currentStepIdxV2];
    if(firstTime && currentStepIdxV2 > 0 && scriptV2[currentStepIdxV2 - 1].setWhiteNodeIdx != -1){
        std::vector<Block*> &list = (!isCalculatingHistoryV2) ? cloneHeapListV2 : heapList;
        list[scriptV2[currentStepIdxV2 - 1].setWhiteNodeIdx] -> setFillColor(sf::Color::White);
    }


    if(step.type == StepTypeV2::INITIALIZE){
        std::cout << "initialize\n";
    }
    if(step.type == StepTypeV2::TRAVERSE){
        std::cout << "traverse\n";
    }
    if(step.type == StepTypeV2::COMPARE){
        std::cout << "compare\n";
    }
    if(step.type == StepTypeV2::SWAP_DOWN){
        std::cout << "swap down\n";
    }
    if(step.type == StepTypeV2::UPDATE){
        std::cout << "update\n";
    }
    if(step.type == StepTypeV2::FINISH){
        std::cout << "finish\n";
    }
    if(step.type == StepTypeV2::NEW_NODE){
        std::cout << "new node\n";
    }
    if(step.type == StepTypeV2::DELETE_1){
        std::cout << "delete 1\n";
    }
    if(step.type == StepTypeV2::DELETE_2){
        std::cout << "delete 2\n";
    }
    if(step.type == StepTypeV2::MOVE){
        std::cout << "move\n";
    }

    std::cout << scriptV2.size() << "\n";

    if(step.type == StepTypeV2::INITIALIZE){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);
        if(firstTime){
            firstTime = false;
        }
        drawHeapList(window, cloneHeapListV2);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::NEW_NODE){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        if(firstTime){
            firstTime = false;
            cloneNewNode = new Block(CIRCLE, 30.f, std::to_string(step.focusNodeVal));
            cloneNewNode->currentPosition = calculatePos(step.focusNodeIdx, list.size() + 1) + sf::Vector2f(0.f, 100.f);
            cloneNewNode->targetPosition = {cloneNewNode->currentPosition};
            cloneNewNode->setPosition(cloneNewNode->currentPosition);
        }
        drawHeapList(window, list);
        if(!isCalculatingHistoryV2) cloneNewNode->draw(window);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::TRAVERSE){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        if(firstTime){
            firstTime = false;
        }

        if(step.focusAnotherNodeIdx != -1)
            list[step.focusAnotherNodeIdx]->setFillColor(sf::Color::White);

        list[step.focusNodeIdx]->setFillColor(sf::Color::Yellow);
        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::COMPARE){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        if(firstTime){
            firstTime = false;
        }

        int child = step.focusAnotherNodeIdx;
        list[child]->setFillColor(sf::Color(222, 184, 135));
        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::SWAP_DOWN){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        int child = step.focusAnotherNodeIdx;
        if(firstTime){
            firstTime = false;
            list[step.focusNodeIdx]->setFillColor(sf::Color::Green);
            list[child]->setFillColor(sf::Color::Blue);
            std::swap(list[step.focusNodeIdx], list[child]);
            std::swap(list[step.focusNodeIdx]->targetPosition, list[child]->targetPosition);
        }
        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::SWAP_UP){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        int parent = step.focusAnotherNodeIdx;
        if(firstTime){
            firstTime = false;
            list[step.focusNodeIdx]->setFillColor(sf::Color::Green);
            list[parent]->setFillColor(sf::Color::Blue);
            std::swap(list[step.focusNodeIdx], list[parent]);
            std::swap(list[step.focusNodeIdx]->targetPosition, list[parent]->targetPosition);
        }
        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::INSERT){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        
        if(firstTime){
            firstTime = false;
            cloneNewNode -> targetPosition = calculatePos(step.focusNodeIdx, list.size() + 1);
            list.push_back(cloneNewNode);
            cloneNewNode = nullptr;
            for(int i = 0; i < list.size(); i++){
                list[i]->targetPosition = calculatePos(i, list.size());
            }
        }
        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::UPDATE){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        if(firstTime){
            firstTime = false;
        }
        
        list[step.focusNodeIdx] -> setFillColor(sf::Color::Green);
        list[step.focusNodeIdx] -> initText(std::to_string(step.focusNodeVal), style4);
        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::DELETE_1){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        if(firstTime){
            firstTime = false;
        }

        list[step.focusNodeIdx] -> setFillColor(sf::Color::Red);
        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::DELETE_2){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        if(firstTime){
            firstTime = false;
        }

        list[step.focusNodeIdx]->targetPosition = {calculatePos(step.focusNodeIdx, list.size()) + sf::Vector2f(0.f, 100.f)};
        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.3f, cloneNewNode, list)){
            // std::swap(list[step.focusNodeIdx], list[step.focusAnotherNodeIdx]);
            if(isCalculatingHistoryV2) garbageV2.push_back(list.back());
            list.pop_back();
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::MOVE){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        if(firstTime){
            firstTime = false;
            list[step.focusNodeIdx] -> setText(list[step.focusAnotherNodeIdx] -> getLabel());
            list[step.focusAnotherNodeIdx] -> setFillColor(sf::Color(32, 178, 170));
        }

        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV2::HIGHLIGHT_1){
        std::vector<Block*> &list = (!isCalculatingHistoryV2 ? cloneHeapListV2 : heapList);
        Block* &cloneNewNode = (!isCalculatingHistoryV2 ? historyV2[currentStepIdxV2]->outDummyNewNode : newNode);

        if(firstTime){
            firstTime = false;
        }
        drawHeapList(window, list);
        if(isCalculatingHistoryV2 || checkNextStepV2(0.5f, cloneNewNode, list)){
            currentStepIdxV2++;
            if(!isCalculatingHistoryV2) historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            choosePrevNextButton = 0;
            firstTime = true;
        }

    }
    else if(step.type == StepTypeV2::FINISH){
        // drawHeapList(window, list);
        choosePrevNextButton = 0;
        firstTime = true;
    }

}


void performVisualization2(sf::RenderWindow& window){
    if(currentStepIdxV2 == -1 || currentStepIdxV2 >= scriptV2.size()) return;
    if(isCalculatingHistoryV2){
        resetRectangleMinMax();
        currentStepIdxV2 = 0;
        firstTime = true;
        while(currentStepIdxV2 + 1 < scriptV2.size()){
            historyV2.push_back(new cloneVisualization2(newNode, scriptV2[currentStepIdxV2]));
            for(auto &node:heapList) getRectangleMinMax(node->getPosition());
            if(newNode) getRectangleMinMax(newNode->getPosition());
            runV2(window);
        }
        if(currentStepIdxV2 + 1 == scriptV2.size()) {
            historyV2.push_back(new cloneVisualization2(newNode, scriptV2[currentStepIdxV2]));
            for(auto &node:heapList) getRectangleMinMax(node->getPosition());
            if(newNode) getRectangleMinMax(newNode->getPosition());
        }
        isCalculatingHistoryV2 = false;
        currentStepIdxV2 = 0;
        historyV2[0]->pull(dummySetV2, liveToDummyMapV2);
        firstTime = true;
    }
    std::cout << "currentStepIdxV2: " << currentStepIdxV2 << " " << scriptV2.size() << std::endl;
    if(isStepByStep) {
        if(choosePrevNextButton == -1 && currentStepIdxV2 > 0){
            currentStepIdxV2--;
            std::cout << "before pull: \n";
            historyV2[currentStepIdxV2]->pull(dummySetV2, liveToDummyMapV2);
            std::cout << "after pull: \n";
            choosePrevNextButton = 0;
            firstTime = true;
        }
        else if(choosePrevNextButton == -1){
            choosePrevNextButton = 0;
        }
        else if(choosePrevNextButton == 1 && currentStepIdxV2 + 1 < scriptV2.size()){
            drawCodeHighlightPanel(2, currentStepIdxV2, isStepByStep, checkFinishedV2(), scriptV2[currentStepIdxV2].activeLines);
            runV2(window);
        }
        else if(choosePrevNextButton == 1){
            choosePrevNextButton = 0;
        }
        if(choosePrevNextButton == 0){
            // std::cout << currentStepIdxV2;
            // std::cout << ": outDummyRoot: " << ((historyV2[currentStepIdxV2]->outDummyRoot) ? historyV2[currentStepIdxV2]->outDummyRoot->getLabel() : "null");
            // std::cout << " outDummyNewNode: " << ((historyV2[currentStepIdxV2]->outDummyNewNode) ? historyV2[currentStepIdxV2]->outDummyNewNode->getLabel() : "null") << std::endl; 
            if(currentStepIdxV2 > 0)    drawCodeHighlightPanel(2, currentStepIdxV2 - 1, isStepByStep, checkFinishedV2(), scriptV2[currentStepIdxV2 - 1].activeLines);
            else                        drawCodeHighlightPanel(2, currentStepIdxV2 - 1, isStepByStep, checkFinishedV2(), {});
            std::vector<Block*> list;
            list = cloneHeapListV2;
            Block* cloneNewNode = historyV2[currentStepIdxV2]->outDummyNewNode;
            drawHeapList(window, list);
            if(cloneNewNode) cloneNewNode -> draw(window);
        }
    }
    else{
        if(currentStepIdxV2 > 0 && checkFinishedV2()) drawCodeHighlightPanel(2, currentStepIdxV2, isStepByStep, checkFinishedV2(), scriptV2[currentStepIdxV2 - 1].activeLines);
        else drawCodeHighlightPanel(2, currentStepIdxV2, isStepByStep, checkFinishedV2(), scriptV2[currentStepIdxV2].activeLines);
        runV2(window);
    }
}
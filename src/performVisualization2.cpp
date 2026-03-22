#include<SFML/Graphics.hpp>
#include <vector>
#include "main.h"
#include "entity.h"
#include "performVisualization2.h"
#include "visualization2.h"



std::vector<AnimationStepV2> scriptV2;

int currentStepIdxV2 = -1;

bool checkFinishedV2(){
    if(currentStepIdxV2 == -1 || currentStepIdxV2 + 1>= scriptV2.size()) return true;
    return false;
}
void performVisualization2(sf::RenderWindow& window){
    if(currentStepIdxV2 == -1 || currentStepIdxV2 >= scriptV2.size()) return;
    AnimationStepV2& step = scriptV2[currentStepIdxV2];

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
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::NEW_NODE){
        if(!newNode){
            newNode = new Block(CIRCLE, 30.f, std::to_string(step.focusNodeVal));
            newNode->currentPosition = calculatePos(step.focusNodeIdx, heapList.size() + 1) + sf::Vector2f(0.f, 100.f);
            newNode->targetPosition = {newNode->currentPosition};
            newNode->setPosition(newNode->currentPosition);
        }
        drawHeapList(window);
        newNode->draw(window);
        if(checkNextStepV2(0.5f)){
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::TRAVERSE){
        if(step.focusAnotherNodeIdx != -1)
            heapList[step.focusAnotherNodeIdx]->setFillColor(sf::Color::White);
        heapList[step.focusNodeIdx]->setFillColor(sf::Color::Yellow);
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::COMPARE){
        int child = step.focusAnotherNodeIdx;
        heapList[child]->setFillColor(sf::Color(222, 184, 135));
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            heapList[child]->setFillColor(sf::Color::White);
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::SWAP_DOWN){
        int child = step.focusAnotherNodeIdx;
        if(child != -1){
            heapList[step.focusNodeIdx]->setFillColor(sf::Color::Green);
            heapList[child]->setFillColor(sf::Color::Blue);
            std::swap(heapList[step.focusNodeIdx], heapList[child]);
            std::swap(heapList[step.focusNodeIdx]->targetPosition, heapList[child]->targetPosition);
            step.focusAnotherNodeIdx = -1;
        }
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            heapList[step.focusNodeIdx]->setFillColor(sf::Color::White);
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::SWAP_UP){
        int parent = step.focusAnotherNodeIdx;
        if(parent != -1){
            heapList[step.focusNodeIdx]->setFillColor(sf::Color::Green);
            heapList[parent]->setFillColor(sf::Color::Blue);
            std::swap(heapList[step.focusNodeIdx], heapList[parent]);
            std::swap(heapList[step.focusNodeIdx]->targetPosition, heapList[parent]->targetPosition);
            step.focusAnotherNodeIdx = -1;
        }
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            heapList[step.focusNodeIdx]->setFillColor(sf::Color::White);
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::INSERT){
        if(newNode){
            newNode -> targetPosition = calculatePos(step.focusNodeIdx, heapList.size() + 1);
            heapList.push_back(newNode);
            newNode = nullptr;
            for(int i = 0; i < heapList.size(); i++){
                heapList[i]->targetPosition = calculatePos(i, heapList.size());
            }
        }
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::UPDATE){
        heapList[step.focusNodeIdx] -> setFillColor(sf::Color::Green);
        heapList[step.focusNodeIdx] -> initText(std::to_string(step.focusNodeVal), style4);
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::DELETE_1){
        heapList[step.focusNodeIdx] -> setFillColor(sf::Color::Red);
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::DELETE_2){
        heapList[step.focusNodeIdx]->targetPosition = {calculatePos(step.focusNodeIdx, heapList.size()) - sf::Vector2f(0.f, 100.f)};
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            std::swap(heapList[step.focusNodeIdx], heapList[step.focusAnotherNodeIdx]);
            delete heapList.back();
            heapList.pop_back();
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::MOVE){
        for(int i = 0; i < heapList.size(); i++){
            heapList[i]->targetPosition = calculatePos(i, heapList.size());
        }
        drawHeapList(window);
        if(checkNextStepV2(0.5f)){
            currentStepIdxV2++;
        }
    }
    else if(step.type == StepTypeV2::FINISH){
        drawHeapList(window);
    }
}
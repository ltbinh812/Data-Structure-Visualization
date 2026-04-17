#include <math.h>
#include "performVisualization1.h"
#include "visualization1.h"
#include "draw.h"
#include "main.h"
#include "entity.h"
#include "highlight.h"
#include "theme.h"

// struct cloneVisualization1 
cloneVisualization1::cloneVisualization1(std::vector<Block*> linkedList, Block* newNode) {
    for(Block* node : linkedList) {
        Block* newCloneNode = new Block(*node);
        this->linkedList.push_back(newCloneNode);
    }
    if(newNode) {
        Block* newCloneNode = new Block(*newNode);
        this->newNode = newCloneNode;
    }
    std::cout << "***********clone linked list\n";
    for(Block* node : this -> linkedList) {
        std::cout << node->getLabel() << " ";
    }
    std::cout << "\n";
}

void cloneVisualization1::pull(std::vector<Block*>& linkedList, Block*& newNode) {
    for (Block* node : linkedList) {
        delete node;
    }
    linkedList.clear();
    if(newNode) {
        delete newNode;
        newNode = nullptr;
    }

    for(Block* node : this->linkedList) {
        Block* newCloneNode = new Block(*node);
        linkedList.push_back(newCloneNode);
    }
    if(this->newNode) {
        Block* newCloneNode = new Block(*this->newNode);
        newNode = newCloneNode;
    }
}

cloneVisualization1::~cloneVisualization1() {
    for (Block* node : linkedList) {
        delete node;
    }
    linkedList.clear();
    if (newNode) { 
        delete newNode;
        newNode = nullptr;
    }
}


std::vector<AnimationStepV1> scriptV1;
int currentStepIdxV1 = -1;
std::vector<cloneVisualization1*> historyV1;     //
bool isCalculatingHistoryV1 = false;            //



bool checkFinishedV1(){
    if(currentStepIdxV1 == -1 || currentStepIdxV1 + 1>= scriptV1.size()) return true;
    return false;
}


void runV1(sf::RenderWindow& window){
    AnimationStepV1& step = scriptV1[currentStepIdxV1];
    std::cout << "size: " <<  scriptV1.size() << "\n";
    if(step.type == StepTypeV1::NEW_NODE){
        std::cout << "NEW NODE\n";
    }
    if(step.type == StepTypeV1::INSERT){
        std::cout << "INSERT\n";
    }
    if(step.type == StepTypeV1::TRAVERSE){
        std::cout << "TRAVERSE\n";
    }
    if(step.type == StepTypeV1::FINISH){
        std::cout << "FINISH\n";
    }

    
    if(step.type == StepTypeV1::NEW_NODE){
        drawLinkedList(window);

        if(newNode == nullptr){
            newNode = new Block(CIRCLE, 30.f, std::to_string(scriptV1[currentStepIdxV1].focusNodeVal));
            newNode->currentPosition = {200.f, 150.0f};
            newNode->targetPosition = {newNode->currentPosition};
            newNode->setPosition(newNode->currentPosition);
        }
        if(!isCalculatingHistoryV1) newNode->draw(window);
        if(isCalculatingHistoryV1 || checkNextStepV1(0.5f)){
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::TRAVERSE){
        drawLinkedList(window);
        linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color::Yellow);
        if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        
        if(newNode)
            if(!isCalculatingHistoryV1) newNode -> draw(window);
        
        if(isCalculatingHistoryV1 || checkNextStepV1(0.6f)){
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::INSERT){
        std::cout << "?1\n";
        if(newNode){
            std::cout << "#1\n";
            std::cout << scriptV1[currentStepIdxV1].focusNodeIdx << " " << linkedList.size() << "\n";
            linkedList.insert(linkedList.begin() + scriptV1[currentStepIdxV1].focusNodeIdx, newNode);
            std::cout << "#2\n";
            linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color::Green);
            std::cout << "#3\n";
        }
        newNode = nullptr;
        std::cout << "?2\n";
        for(int i = 0; i < linkedList.size(); ++i){
            linkedList[i]->targetPosition = {200.f + i * 150.f, 300.f};
        }
        std::cout << "?3\n";
        drawLinkedList(window);
        if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        if(isCalculatingHistoryV1 || checkNextStepV1(0.5f)){
            linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color::Green);
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
        std::cout << "?4\n";
    }
    else if(step.type == StepTypeV1::DELETE_1){
        for(int i = 0; i < linkedList.size(); ++i){
            if(i == scriptV1[currentStepIdxV1].focusNodeIdx){
                linkedList[i]->setFillColor(sf::Color::Red);
                linkedList[i]->targetPosition = {200.f + i * 150.f, 450.f};
                continue;
            }
            linkedList[i]->targetPosition = {200.f + i * 150.f, 300.f};
        }
        drawLinkedList(window);
        if(scriptV1[currentStepIdxV1].focusNodeIdx > 0)
                if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx - 1] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx - 1));
        if(isCalculatingHistoryV1 || checkNextStepV1(0.5f)){
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::DELETE_2){
        if(!newNode){
            newNode = linkedList[scriptV1[currentStepIdxV1].focusNodeIdx];
            linkedList.erase(linkedList.begin() + scriptV1[currentStepIdxV1].focusNodeIdx);
            if(scriptV1[currentStepIdxV1].focusNodeIdx < linkedList.size())
                linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color::Green);
        }
        for(int i = 0; i < linkedList.size(); ++i){
            linkedList[i]->targetPosition = {200.f + i * 150.f, 300.f};
        }
        drawLinkedList(window);
        newNode -> move(dtV1, isCalculatingHistoryV1);
        if(!isCalculatingHistoryV1) newNode -> draw(window);
        if(scriptV1[currentStepIdxV1].focusNodeIdx < linkedList.size()){
                sf::Vector2f direction = linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() - newNode -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if(!isCalculatingHistoryV1) drawArrow(window, newNode -> center() + direction / length * newNode -> getRadius(), linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() - direction / length * linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> getRadius(), edgeColor);
        }
        if(scriptV1[currentStepIdxV1].focusNodeIdx > 0)
            if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx - 1] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx - 1));
        if(isCalculatingHistoryV1 || checkNextStepV1(0.5f)){
            delete newNode;
            newNode = nullptr;
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::UPDATE){
        linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color(255, 127, 36));
        linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> initText(std::to_string(scriptV1[currentStepIdxV1].focusNodeVal), style4);
        drawLinkedList(window);
        if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        if(isCalculatingHistoryV1 || checkNextStepV1(0.6f)){
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::SEARCH_1){
        linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color::Green);
        drawLinkedList(window);
        if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        if(isCalculatingHistoryV1 || checkNextStepV1(0.5f)){
            if(!isCalculatingHistoryV1) setLog("Found node in index " + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx) + " with value " + std::to_string(scriptV1[currentStepIdxV1].focusNodeVal), sf::Color::Green);
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::SEARCH_2){
        drawLinkedList(window);
        if(isCalculatingHistoryV1 || checkNextStepV1(0.5f)){
            if(!isCalculatingHistoryV1) setLog("Not found!", sf::Color::Red);
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::HIGHLIGHT_1){ // new node
        drawLinkedList(window);
        if(!isCalculatingHistoryV1) newNode->draw(window);
        if(isCalculatingHistoryV1 || checkNextStepV1(0.5f)){
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::HIGHLIGHT_2){ // traverse, insert
        drawLinkedList(window);
        // if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        if(newNode)
            if(!isCalculatingHistoryV1) newNode -> draw(window);
        
        if(isCalculatingHistoryV1 || checkNextStepV1(0.6f)){
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::HIGHLIGHT_3){ // delete 1
        drawLinkedList(window);
        if(scriptV1[currentStepIdxV1].focusNodeIdx > 0)
                if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx - 1] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx - 1));
        if(isCalculatingHistoryV1 || checkNextStepV1(0.5f)){
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::HIGHLIGHT_4){ // update
        drawLinkedList(window);
        if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        if(isCalculatingHistoryV1 || checkNextStepV1(0.6f)){
            currentStepIdxV1++;
            if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV1::FINISH){
        std::cout << linkedList.size() << std::endl;
        if(!isCalculatingHistoryV1) historyV1[currentStepIdxV1]->pull(linkedList, newNode);
        choosePrevNextButton = 0;
        firstTime = true;
        // drawLinkedList(window);
        // drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
    }



    // ImGui::Text(step.log.c_str());
    
}


void performVisualization1(sf::RenderWindow& window){
    if(currentStepIdxV1 == -1 || currentStepIdxV1 >= scriptV1.size()) return;

    if(isCalculatingHistoryV1){
        resetRectangleMinMax();
        currentStepIdxV1 = 0;
        firstTime = true;
        while(currentStepIdxV1 + 1 < scriptV1.size()){
            historyV1.push_back(new cloneVisualization1(linkedList, newNode));
            for(auto node:linkedList) getRectangleMinMax(node -> getPosition());
            if(newNode) getRectangleMinMax(newNode -> getPosition());
            runV1(window);
        }
        if(currentStepIdxV1 + 1 == scriptV1.size()) {
            historyV1.push_back(new cloneVisualization1(linkedList, newNode));
            for(auto node:linkedList) getRectangleMinMax(node -> getPosition());
            if(newNode) getRectangleMinMax(newNode -> getPosition());
        }
        isCalculatingHistoryV1 = false;
        currentStepIdxV1 = 0;
        historyV1[0]->pull(linkedList, newNode);
        firstTime = true;

        std::cout << "????????????????\n";
        for(int i=0; i<scriptV1.size(); i++){
            std::cout << "step " << i << ": ";
            for(Block* node : historyV1[i]->linkedList) {
                std::cout << node->getLabel() << " ";
            }
            std::cout << std::endl;
            std::cout << "newNode: ";
            if(historyV1[i]->newNode) std::cout << historyV1[i]->newNode->getLabel() << "\n";
            else std::cout << "nullptr\n";
        }

    }


    if(isStepByStep) {
        if(choosePrevNextButton == -1 && currentStepIdxV1 > 0){
            currentStepIdxV1--;
            std::cout << "before pull: \n";
            historyV1[currentStepIdxV1]->pull(linkedList, newNode);
            std::cout << "after pull: \n";
            choosePrevNextButton = 0;
            firstTime = true;
        }
        else if(choosePrevNextButton == -1){
            choosePrevNextButton = 0;
        }
        else if(choosePrevNextButton == 1 && currentStepIdxV1 + 1 < scriptV1.size()){
            drawCodeHighlightPanel(1, currentStepIdxV1, isStepByStep, checkFinishedV1(), scriptV1[currentStepIdxV1].activeLines);
            runV1(window);
        }
        else if(choosePrevNextButton == 1){
            choosePrevNextButton = 0;
        }
        if(choosePrevNextButton == 0){
            if(currentStepIdxV1 > 0)    drawCodeHighlightPanel(1, currentStepIdxV1 - 1, isStepByStep, checkFinishedV1(), scriptV1[currentStepIdxV1 - 1].activeLines);
            else                        drawCodeHighlightPanel(1, currentStepIdxV1 - 1, isStepByStep, checkFinishedV1(), {});
            drawLinkedList(window);
        }
    }
    else{
        if(currentStepIdxV1 > 0 && checkFinishedV1()) drawCodeHighlightPanel(1, currentStepIdxV1, isStepByStep, checkFinishedV1(), scriptV1[currentStepIdxV1 - 1].activeLines);
        else drawCodeHighlightPanel(1, currentStepIdxV1, isStepByStep, checkFinishedV1(), scriptV1[currentStepIdxV1].activeLines);
        runV1(window);
    }

}
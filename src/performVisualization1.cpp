#include <math.h>
#include "performVisualization1.h"
#include "visualization1.h"
#include "draw.h"
#include "main.h"
#include "entity.h"

std::vector<AnimationStepV1> scriptV1;

int currentStepIdxV1 = -1;

bool checkFinishedV1(){
    if(currentStepIdxV1 == -1 || currentStepIdxV1 + 1>= scriptV1.size()) return true;
    return false;
}

void performVisualization1(sf::RenderWindow& window){
    if(currentStepIdxV1 == -1 || currentStepIdxV1 >= scriptV1.size()) return;

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
        newNode->draw(window);  
        if(checkNextStepV1(0.5f)){
            currentStepIdxV1++;
        }
    }
    else if(step.type == StepTypeV1::TRAVERSE){
        drawLinkedList(window);
        linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color::Yellow);
        drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        
        if(newNode)
            newNode -> draw(window);
        bool c = checkNextStepV1(0.6f);
        std::cout << scriptV1[currentStepIdxV1].focusNodeIdx << " " << scriptV1[currentStepIdxV1].focusNodeIdx << " " << c << "\n";
        if(c) currentStepIdxV1++;
    }
    else if(step.type == StepTypeV1::INSERT){
        if(newNode){
            linkedList.insert(linkedList.begin() + scriptV1[currentStepIdxV1].focusNodeIdx, newNode);
            linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color::Green);
        }
        newNode = nullptr;
        for(int i = 0; i < linkedList.size(); ++i){
            linkedList[i]->targetPosition = {200.f + i * 150.f, 300.f};
        }
        drawLinkedList(window);
        drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        if(checkNextStepV1(0.5f)){
            linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color::Green);
            currentStepIdxV1++;
        }
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
                drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx - 1] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx - 1));
        if(checkNextStepV1(0.5f)){
            currentStepIdxV1++;
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
        newNode -> move(dtV1);
        newNode -> draw(window);
        if(scriptV1[currentStepIdxV1].focusNodeIdx < linkedList.size()){
                sf::Vector2f direction = linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() - newNode -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                drawArrow(window, newNode -> center() + direction / length * newNode -> getRadius(), linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() - direction / length * linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> getRadius());
        }
        if(scriptV1[currentStepIdxV1].focusNodeIdx > 0)
            drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx - 1] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx - 1));
        if(checkNextStepV1(0.5f)){
            delete newNode;
            newNode = nullptr;
            currentStepIdxV1++;
        }
    }
    else if(step.type == StepTypeV1::UPDATE){
        linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color(255, 127, 36));
        linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> initText(std::to_string(scriptV1[currentStepIdxV1].focusNodeVal), style4);
        drawLinkedList(window);
        drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        if(checkNextStepV1(0.6f)){
            currentStepIdxV1++;
        }
    }
    else if(step.type == StepTypeV1::SEARCH_1){
        linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> setFillColor(sf::Color::Green);
        drawLinkedList(window);
        drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
        if(checkNextStepV1(0.5f)){
            setLog("Found node in index " + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx) + " with value " + std::to_string(scriptV1[currentStepIdxV1].focusNodeVal), sf::Color::Green);
            currentStepIdxV1++;
        }
    }
    else if(step.type == StepTypeV1::SEARCH_2){
        drawLinkedList(window);
        if(checkNextStepV1(0.5f)){
            setLog("Not found!", sf::Color::Red);
            currentStepIdxV1++;
        }
    }
    else if(step.type == StepTypeV1::FINISH){
        std::cout << linkedList.size() << std::endl;
        // drawLinkedList(window);
        // drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
    }



    // ImGui::Text(step.log.c_str());
}
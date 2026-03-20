#include <math.h>
#include "performVisualization1.h"
#include "visualization1.h"
#include "draw.h"
#include "main.h"
#include "entity.h"

std::vector<AnimationStep> script;

int currentStepIdx = -1;
int currentPointer = 0;
float animationTime = 30.0f / 60.f; // 60 FPS
Block* newNode = nullptr; // Node chờ thực hiện (dùng cho insert)


void performInsertVisualization1(sf::RenderWindow& window){
    if(currentStepIdx == -1 || currentStepIdx >= script.size()) return;

    AnimationStep& step = script[currentStepIdx];
    std::cout << "size: " <<  script.size() << "\n";
    if(step.type == StepType::NEW_NODE){
        std::cout << "NEW NODE\n";
    }
    if(step.type == StepType::INSERT){
        std::cout << "INSERT\n";
    }
    if(step.type == StepType::TRAVERSE){
        std::cout << "TRAVERSE\n";
    }
    if(step.type == StepType::FINISH){
        std::cout << "FINISH\n";
    }

    
    if(step.type == StepType::NEW_NODE){
        drawList(window);

        if(newNode == nullptr){
            newNode = new Block(CIRCLE, 30.f, std::to_string(script[currentStepIdx].focusNodeVal));
            newNode->currentPosition = {200.f, 150.0f};
            newNode->targetPosition = {newNode->currentPosition};
            newNode->setPosition(newNode->currentPosition);
        }
        newNode->draw(window);  
        if(checkNextStep(0.5f)){
            currentStepIdx++;
            if(script[currentStepIdx].type == StepType::TRAVERSE) currentPointer = 0;
        }
    }
    else if(step.type == StepType::TRAVERSE){
        drawList(window);
        currentPointer = script[currentStepIdx].focusNodeIdx;
        linkedList[currentPointer] -> setFillColor(sf::Color::Yellow);
        drawPointer(window, linkedList[currentPointer] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(currentPointer));
        
        if(newNode)
            newNode -> draw(window);
        bool c = checkNextStep(0.6f);
        std::cout << currentPointer << " " << script[currentStepIdx].focusNodeIdx << " " << c << "\n";
        if(c) currentStepIdx++;
    }
    else if(step.type == StepType::INSERT){
        if(newNode){
            linkedList.insert(linkedList.begin() + script[currentStepIdx].focusNodeIdx, newNode);
            linkedList[script[currentStepIdx].focusNodeIdx] -> setFillColor(sf::Color::Green);
        }
        newNode = nullptr;
        for(int i = 0; i < linkedList.size(); ++i){
            linkedList[i]->targetPosition = {200.f + i * 150.f, 300.f};
        }
        drawList(window);
        drawPointer(window, linkedList[script[currentStepIdx].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(script[currentStepIdx].focusNodeIdx));
        if(checkNextStep(0.5f)){
            linkedList[script[currentStepIdx].focusNodeIdx] -> setFillColor(sf::Color::Green);
            currentStepIdx++;
        }
    }
    else if(step.type == StepType::DELETE_1){
        for(int i = 0; i < linkedList.size(); ++i){
            if(i == script[currentStepIdx].focusNodeIdx){
                linkedList[i]->setFillColor(sf::Color::Red);
                linkedList[i]->targetPosition = {200.f + i * 150.f, 450.f};
                continue;
            }
            linkedList[i]->targetPosition = {200.f + i * 150.f, 300.f};
        }
        drawList(window);
        if(script[currentStepIdx].focusNodeIdx > 0)
                drawPointer(window, linkedList[script[currentStepIdx].focusNodeIdx - 1] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(script[currentStepIdx].focusNodeIdx - 1));
        if(checkNextStep(0.5f)){
            currentStepIdx++;
        }
    }
    else if(step.type == StepType::DELETE_2){
        if(!newNode){
            newNode = linkedList[script[currentStepIdx].focusNodeIdx];
            linkedList.erase(linkedList.begin() + script[currentStepIdx].focusNodeIdx);
            if(script[currentStepIdx].focusNodeIdx < linkedList.size())
                linkedList[script[currentStepIdx].focusNodeIdx] -> setFillColor(sf::Color::Green);
        }
        for(int i = 0; i < linkedList.size(); ++i){
            linkedList[i]->targetPosition = {200.f + i * 150.f, 300.f};
        }
        drawList(window);
        newNode -> move(dt);
        newNode -> draw(window);
        if(script[currentStepIdx].focusNodeIdx < linkedList.size()){
                sf::Vector2f direction = linkedList[script[currentStepIdx].focusNodeIdx] -> center() - newNode -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                drawArrow(window, newNode -> center() + direction / length * newNode -> getRadius(), linkedList[script[currentStepIdx].focusNodeIdx] -> center() - direction / length * linkedList[script[currentStepIdx].focusNodeIdx] -> getRadius());
        }
        if(script[currentStepIdx].focusNodeIdx > 0)
            drawPointer(window, linkedList[script[currentStepIdx].focusNodeIdx - 1] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(script[currentStepIdx].focusNodeIdx - 1));
        if(checkNextStep(0.5f)){
            delete newNode;
            newNode = nullptr;
            currentStepIdx++;
        }
    }
    else if(step.type == StepType::UPDATE){
        linkedList[script[currentStepIdx].focusNodeIdx] -> setFillColor(sf::Color(255, 127, 36));
        linkedList[script[currentStepIdx].focusNodeIdx] -> initText(std::to_string(script[currentStepIdx].focusNodeVal), style4);
        drawList(window);
        drawPointer(window, linkedList[script[currentStepIdx].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(script[currentStepIdx].focusNodeIdx));
        if(checkNextStep(0.6f)){
            currentStepIdx++;
        }
    }
    else if(step.type == StepType::SEARCH_1){
        linkedList[script[currentStepIdx].focusNodeIdx] -> setFillColor(sf::Color::Green);
        drawList(window);
        drawPointer(window, linkedList[script[currentStepIdx].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(script[currentStepIdx].focusNodeIdx));
        if(checkNextStep(0.5f)){
            setLog("Found node in index " + std::to_string(script[currentStepIdx].focusNodeIdx) + " with value " + std::to_string(script[currentStepIdx].focusNodeVal), sf::Color::Green);
            currentStepIdx++;
        }
    }
    else if(step.type == StepType::SEARCH_2){
        drawList(window);
        if(checkNextStep(0.5f)){
            setLog("Not found!", sf::Color::Red);
            currentStepIdx++;
        }
    }
    else if(step.type == StepType::FINISH){
        std::cout << linkedList.size() << std::endl;
        // drawList(window);
        // drawPointer(window, linkedList[script[currentStepIdx].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(script[currentStepIdx].focusNodeIdx));
    }



    // ImGui::Text(step.log.c_str());
}
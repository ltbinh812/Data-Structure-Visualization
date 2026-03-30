#include "math.h"
#include "visualization4.h"
#include "performVisualization4.h"
#include "main.h"
#include "entity.h"
#include "draw.h"


std::vector<AnimationStepV4> scriptV4;
int currentStepIdxV4 = -1;


void performVisualization4(sf::RenderWindow& window) {
    if(currentStepIdxV4 == -1 || currentStepIdxV4 >= scriptV4.size()) return;
    AnimationStepV4 step = scriptV4[currentStepIdxV4];


    if(step.type == StepTypeV4::INITIALIZE) {
        std::cout << "initialize\n";
    }
    if(step.type == StepTypeV4::NEW_NODE) {
        std::cout << "new node\n";
    }
    if(step.type == StepTypeV4::INSERT) {
        std::cout << "insert\n";
    }
    if(step.type == StepTypeV4::DELETE) {
        std::cout << "delete\n";
    }
    if(step.type == StepTypeV4::TRAVERSE) {
        std::cout << "traverse\n";
    }
    if(step.type == StepTypeV4::FINISH) {
        std::cout << "finish\n";
    }

    std::cout << scriptV4.size() << " " << currentStepIdxV4 << std::endl;

    if(step.type == StepTypeV4::TRAVERSE){
        if(!newNode){
            step.focusNode -> setFillColor(sf::Color::Yellow);
            step.focusNode -> value += step.focusNodeVal;
            if(step.isFinished) step.focusNode -> height++;
            std::cout << "************" << step.isFinished << " " << step.focusNode -> getLabel() << " " << step.focusNode -> height << std::endl;
            newNode = rootV4;
        }
        drawTrie(rootV4, window);
        if(checkNextStepV4(0.5f)){
            newNode = nullptr;
            currentStepIdxV4++;
        }
    }
    else if(step.type == StepTypeV4::INSERT){
        if(!newNode){
            newNode = step.focusNode;
            newNode -> value = 1;
            step.focusAnotherNode -> children[newNode -> getLabel()[0]] = newNode;
            calculateTrieGapWidth(rootV4);
            calculateTriePos(rootV4, WINDOW_WIDTH / 2.f, 300.f);
            newNode -> currentPosition = newNode -> targetPosition + sf::Vector2f(0.f, 50.f); 
            newNode -> setPosition(newNode -> currentPosition);
            newNode -> setFillColor(sf::Color::Green);
            if(step.isFinished) newNode -> height++;
        }
        drawTrie(rootV4, window);
        if(checkNextStepV4(0.5f)){
            newNode = nullptr;
            currentStepIdxV4++;
        }
    }
    else if(step.type == StepTypeV4::DELETE){
        if(!newNode){
            newNode = rootV4;
            step.focusNode -> value --;
            if(step.isFinished) step.focusNode -> height--;
            if(step.focusNode -> value == 0)    step.focusNode -> setFillColor(sf::Color::Red);
            else                                 step.focusNode -> setFillColor(sf::Color(255, 165, 0));
        }
        drawTrie(rootV4, window);
        drawTrie(step.focusNode, window);
        if(checkNextStepV4(0.5f)){
            newNode = nullptr;
            if(step.focusNode -> value == 0){
                if(step.focusAnotherNode)
                    step.focusAnotherNode -> children.erase(step.focusNode -> getLabel()[0]);\
                delete step.focusNode;
            }
            currentStepIdxV4++;
        }
    }
    else if(step.type == StepTypeV4::SEARCH){
        if(!newNode){
            newNode = rootV4;
            if(step.focusNodeVal == 1) setLog("Found!", sf::Color::Green);
            else setLog("Not found!", sf::Color::Red);
        }
        drawTrie(rootV4, window);
        if(checkNextStepV4(0.5f)){
            newNode = nullptr;
            currentStepIdxV4++;
        }
    }
    else if(step.type == StepTypeV4::FINISH){

    }

}

bool checkFinishedV4() {
    return currentStepIdxV4 == -1 || currentStepIdxV4 + 1== scriptV4.size();
}


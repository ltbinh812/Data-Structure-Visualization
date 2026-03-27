#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include "entity.h"
#include "performVisualization3.h"
#include "visualization3.h"
#include "draw.h"

int currentStepIdxV3 = -1;
std::vector<AnimationStepV3> scriptV3;


bool checkFinishedV3(){
    if(currentStepIdxV3 == -1 || currentStepIdxV3 + 1>= scriptV3.size()) return true;
    return false;
}

void performVisualization3(sf::RenderWindow& window){
    if(currentStepIdxV3 == -1 || currentStepIdxV3 >= scriptV3.size()) return;
    AnimationStepV3 step = scriptV3[currentStepIdxV3];

    if(step.type == StepTypeV3::INITIALIZE){
        std::cout << "initialize\n";
    }
    if(step.type == StepTypeV3::NEW_NODE){
        std::cout << "new node\n";
    }
    if(step.type == StepTypeV3::INSERT){
        std::cout << "insert\n";
    }
    if(step.type == StepTypeV3::DELETE){
        std::cout << "delete 1\n";
    }
    if(step.type == StepTypeV3::TRAVERSE){
        std::cout << "traverse\n";
    }
    if(step.type == StepTypeV3::UPDATE){
        std::cout << "update\n";
    }
    if(step.type == StepTypeV3::ROTATE_LEFT_LEFT){
        std::cout << "rotate left left\n";
    }
    if(step.type == StepTypeV3::ROTATE_RIGHT_RIGHT){
        std::cout << "rotate right right\n";
    }
    if(step.type == StepTypeV3::FINISH){
        std::cout << "finish\n";
    }
    std::cout << scriptV3.size() << " " << currentStepIdxV3 << std::endl;
    static sf::Color temp_color = sf::Color::White;
    if(step.type == StepTypeV3::TRAVERSE){
        if(!newNode){
            newNode = rootV3;
            temp_color = step.focusNode -> getFillColor();
            if(step.focusNodeVal == 1) step.focusNode -> setFillColor(sf::Color(255, 140, 105));
            else if(step.focusNodeVal == 2) step.focusNode -> setFillColor(sf::Color(0, 191, 255));
            else if(step.focusNodeVal == 3) step.focusNode -> setFillColor(sf::Color::Red);
            else if(step.focusNodeVal == 4) step.focusNode -> setFillColor(sf::Color::Blue);
            else if(step.focusNodeVal == 5) step.focusNode -> setFillColor(sf::Color::Green);
            else step.focusNode -> setFillColor(sf::Color::Yellow);
        }
        drawAVLTree(rootV3, window);
        if(checkNextStepV3(0.5f)){
            if(step.focusNodeVal != 3 && step.focusNodeVal != 4 && step.focusNodeVal != 5){
                step.focusNode -> setFillColor(temp_color);
                temp_color = sf::Color::White;
            }
            newNode = nullptr;
            currentStepIdxV3++;
        }
    }
    else if(step.type == StepTypeV3::NEW_NODE){
        std::cout << "?1\n"; 
        if(!newNode){
            newNode = step.focusNode;
            newNode -> setFillColor(sf::Color::Green);
            newNode -> currentPosition = {100.f, 300.f};
            newNode -> targetPosition = {100.f, 300.f};
            newNode -> setPosition(newNode -> currentPosition);
            std::cout << "new node: " << newNode -> currentPosition.x << " " << newNode -> currentPosition.y << std::endl;
        }
        
        // std::cout << "?2\n"; 
        drawAVLTree(rootV3, window);
        // std::cout << "?3\n"; 
        newNode->draw(window);
        // std::cout << "?4\n"; 
        std::cout << "new node: " << newNode -> currentPosition.x << " " << newNode -> currentPosition.y << std::endl;
        if(checkNextStepV3(0.5f)){
            currentStepIdxV3++;
        }
    }
    else if(step.type == StepTypeV3::INSERT){
        std::cout << "insert 1\n";
        if(newNode){
            if(step.focusNodeVal == -1){
                std::cout << step.focusNode -> getLabel() << "->" << newNode -> getLabel() << std::endl;
                step.focusNode -> pLeft = newNode;
            }
            else if(step.focusNodeVal == 1)                std::cout << step.focusNode -> getLabel() << "->" << newNode -> getLabel() << std::endl,step.focusNode -> pRight = newNode;
            else rootV3 = newNode;
            newNode = nullptr;
       }
        calculateAllPos(rootV3, getHeight(rootV3), 1, WINDOW_WIDTH / 2, 300.f);
        drawAVLTree(rootV3, window);
        if(checkNextStepV3(0.5f)){
            currentStepIdxV3++;
        }
    }
    else if(step.type == StepTypeV3::ROTATE_LEFT_LEFT){
        //       prenode
        //         |        
        //         |          
        //       node1
        //       /
        //      /
        //   node2
        //    / \
        //   /   \
        //  .    node3
        if(!newNode){
            std::cout << "rotate left left 1\n";
            newNode = rootV3;

            Block* prenode = step.focusAnotherNode;
            Block* node1 = step.focusNode;
            Block* node2 = node1 -> pLeft;
            Block* node3 = node2 -> pRight;

            if(!prenode) rootV3 = node2;
            else if(step.focusNodeVal == -1) prenode -> pLeft = node2;
            else if( step.focusNodeVal == 1) prenode -> pRight = node2;
            node1 -> pLeft = node3;
            node2 -> pRight = node1;
            calculateAllPos(rootV3, getHeight(rootV3), 1, WINDOW_WIDTH / 2, 300.f);
        }
        drawAVLTree(rootV3, window);
        if(checkNextStepV3(0.75f)){
            newNode = nullptr;
            currentStepIdxV3++;
        }
    }
    else if(step.type == StepTypeV3::ROTATE_RIGHT_RIGHT){
        //           prenode
        //               \
        //                \
        //                node1
        //                  \
        //                   \
        //                   node2
        //                    /\
        //                   /  \
        //                 node3 .
        if(!newNode){
            std::cout << "rotate right right 1\n";
            newNode = rootV3;
            Block* prenode = step.focusAnotherNode;
            Block* node1 = step.focusNode;
            Block* node2 = node1 -> pRight;
            Block* node3 = node2 -> pLeft;

            if(!prenode) rootV3 = node2;
            else if(step.focusNodeVal == -1) prenode -> pLeft = node2;
            else if( step.focusNodeVal == 1) prenode -> pRight = node2;
            node1 -> pRight = node3;
            node2 -> pLeft = node1;
            calculateAllPos(rootV3, getHeight(rootV3), 1, WINDOW_WIDTH / 2, 300.f);
        }
        drawAVLTree(rootV3, window);
        if(checkNextStepV3(0.75f)){
            newNode = nullptr;
            currentStepIdxV3++;
        }
    }
    else if(step.type == StepTypeV3::DELETE){
        if(!newNode){
            newNode = rootV3;
            Block* replaceNode = step.deletedNode;
            Block* prenode = step.focusAnotherNode;
            Block* node = step.focusNode;
            Block* child = (node -> pLeft == nullptr) ? node -> pRight : node -> pLeft;
            if(prenode){
                std::cout << "..";
                if(step.focusNodeVal == -1) prenode -> pLeft = child;
                else if(step.focusNodeVal == 1) prenode -> pRight = child;
            }
            else
                rootV3 = child;

            
            if(replaceNode){
                std::cout << "..",
                node -> pLeft = replaceNode -> pLeft;
                node -> pRight = replaceNode -> pRight;
                *replaceNode = *node;
            }
            else
                std::cout << "?";

            delete node;
            node = nullptr;
            if(rootV3 == replaceNode) std::cout << "..";
            calculateAllPos(rootV3, getHeight(rootV3), 1, WINDOW_WIDTH / 2, 300.f);
        }
        std::cout << ">\n";
        drawAVLTree(rootV3, window);
        std::cout << ">\n";
        if(checkNextStepV3(0.75f)){
            newNode = nullptr;
            currentStepIdxV3++;
        }
    }
    else if(step.type == StepTypeV3::SEARCH){
        if(step.focusNodeVal == 1){
            step.focusNode -> setFillColor(sf::Color::Green);
            setLog("Found", sf::Color::Green);
        }
        else if(step.focusNodeVal == -1){
            setLog("Not found", sf::Color::Red);
        }
        drawAVLTree(rootV3, window);
        if(checkNextStepV3(0.5f)){
            currentStepIdxV3++;
        }
    }
    else if(step.type == StepTypeV3::FINISH){
        drawAVLTree(rootV3, window);
        std::cout << "finish\n";
    }



}
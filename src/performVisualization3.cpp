#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include "entity.h"
#include "performVisualization3.h"
#include "visualization3.h"
#include "draw.h"
#include "theme.h"
#include "highlight.h"

int currentStepIdxV3 = -1;
std::vector<AnimationStepV3> scriptV3;
bool isCalculatingHistoryV3 = false;
std::vector<cloneVisualization3*> historyV3;     

std::vector<Block*> dummySetV3;
std::map<Block*, Block*> liveToDummyMapV3;

Block* cloneVisualization3::cloneNodeV3(Block* originalNode, std::map<Block*, Block*>& visited) {
    if (originalNode == nullptr) return nullptr;
    if (visited.find(originalNode) != visited.end()) {
        return visited[originalNode];
    }
    Block* clonedNode = new Block(*originalNode);
    mapping[clonedNode] = originalNode;
    visited[originalNode] = clonedNode;
    
    clonedNode -> pLeft = cloneNodeV3(originalNode->pLeft, visited);
    clonedNode -> pRight = cloneNodeV3(originalNode->pRight, visited);
    std::cout << "clone node: " << clonedNode -> getLabel() << "\n";
    if(clonedNode -> pLeft) std::cout << clonedNode -> getLabel() << " left: " << clonedNode -> pLeft -> getLabel() << "\n";
    if(clonedNode -> pRight) std::cout << clonedNode -> getLabel() << " right: " << clonedNode -> pRight -> getLabel() << "\n";
    return clonedNode;
}
cloneVisualization3::cloneVisualization3(Block* root, Block* newNode, const AnimationStepV3& stepV3) {    
    std::map<Block*, Block*> visited;
    std::cout << "-------------------------\n";

    this->rootV3 = cloneNodeV3(root, visited);
    this->newNode = cloneNodeV3(newNode, visited);
    if(stepV3.focusNode){
        cloneNodeV3(stepV3.focusNode, visited);
    }
    if(stepV3.focusAnotherNode){
        cloneNodeV3(stepV3.focusAnotherNode, visited);
    }
    if(stepV3.deletedNode){
        cloneNodeV3(stepV3.deletedNode, visited);
    }
    std::cout << "*********************************\n";
}

void cloneVisualization3::pull(std::vector<Block*>& dummySet, 
                               std::map<Block*, Block*>& liveToDummyMap) 
{   
    // dummy node
    for (Block* dummy : dummySet) {
        delete dummy;
    }
    dummySet.clear();
    liveToDummyMap.clear();
    std::map<Block*, Block*> cloneToDummy; 

    // clone -> dummy
    // live -> dummy
    std::cout << "$$$$$$$$$$$$pull:\n";
    for (auto const& [cloneNode, liveNode] : this->mapping) {
        Block* dummyNode = new Block(*cloneNode);
        std::cout << "dummy node: " << dummyNode -> getLabel() << "\n";
        dummySet.push_back(dummyNode);
        cloneToDummy[cloneNode] = dummyNode;         
        if (liveNode != nullptr) {
            liveToDummyMap[liveNode] = dummyNode; 
        }
    }

    
    // relationship
    for (auto const& [cloneNode, dummyNode] : cloneToDummy) {
        if (cloneNode->pLeft != nullptr) {
            dummyNode->pLeft = cloneToDummy[cloneNode->pLeft];
        } else {
            dummyNode->pLeft = nullptr;
        }

        if (cloneNode->pRight != nullptr) {
            dummyNode->pRight = cloneToDummy[cloneNode->pRight];
        } else {
            dummyNode->pRight = nullptr;
        }
    }


    this -> outDummyRoot = (this->rootV3 != nullptr) ? cloneToDummy[this->rootV3] : nullptr;
    this -> outDummyNewNode = (this->newNode != nullptr) ? cloneToDummy[this->newNode] : nullptr;
    std::cout << "outDummyNewNode: " << (this -> outDummyNewNode ? this -> outDummyNewNode -> getLabel() : "null") << "\n";
    std::cout << "outDummyRoot: " << (this -> outDummyRoot ? this -> outDummyRoot -> getLabel() : "null") << "\n";
}

cloneVisualization3::~cloneVisualization3() {
    // clone to dummy
    for (auto const& pair : mapping) {
        delete pair.first; // clone node
    }
    mapping.clear();
}

bool checkFinishedV3(){
    if(currentStepIdxV3 == -1 || currentStepIdxV3 + 1>= scriptV3.size()) return true;
    return false;
}

void runV3(sf::RenderWindow& window){
    if(currentStepIdxV3 == -1 || currentStepIdxV3 >= scriptV3.size()) return;
    AnimationStepV3 step = scriptV3[currentStepIdxV3];
    if(currentStepIdxV3 > 0 && firstTime && scriptV3[currentStepIdxV3 - 1].setWhiteNode){
        std::cout << "??]?\n";
        if(isCalculatingHistoryV3) scriptV3[currentStepIdxV3 - 1].setWhiteNode -> setFillColor(scriptV3[currentStepIdxV3 - 1].historyColor);
        else liveToDummyMapV3[scriptV3[currentStepIdxV3 - 1].setWhiteNode] -> setFillColor(scriptV3[currentStepIdxV3 - 1].historyColor);
    }
    std::cout << "???" <<  currentStepIdxV3 << " " << firstTime <<  std::endl;
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
        Block* node1 = step.focusNode;
        Block* node2 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyNewNode : newNode;
        Block* node3 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyRoot : rootV3;
        if(!isCalculatingHistoryV3 && step.focusNode) node1 = liveToDummyMapV3[step.focusNode];
        
        if(firstTime){
            firstTime = false;
            std::cout << "TRAVERSE " << node1 -> getLabel() << "\n";
            temp_color = node1 -> getFillColor();
            if(isCalculatingHistoryV3) scriptV3[currentStepIdxV3].historyColor = node1 -> getFillColor();
            if(step.focusNodeVal == 1) node1 -> setFillColor(sf::Color(255, 140, 105));
            else if(step.focusNodeVal == 2) node1 -> setFillColor(sf::Color(0, 191, 255));
            else if(step.focusNodeVal == 3) node1 -> setFillColor(sf::Color::Red);
            else if(step.focusNodeVal == 4) node1 -> setFillColor(sf::Color::Blue);
            else if(step.focusNodeVal == 5) node1 -> setFillColor(sf::Color::Green);
            else node1 -> setFillColor(sf::Color::Yellow);
        }
        drawAVLTree(node3, window);


        if(isCalculatingHistoryV3 || checkNextStepV3(0.5f, node3, node2)){
            if(step.focusNodeVal != 3 && step.focusNodeVal != 4 && step.focusNodeVal != 5){
                // node1 -> setFillColor(temp_color);
                temp_color = sf::Color::White;
            }
            currentStepIdxV3++;
            if(!isCalculatingHistoryV3) historyV3[currentStepIdxV3]->pull(dummySetV3, liveToDummyMapV3);
            choosePrevNextButton = 0;
            firstTime = true;
            
        }
    }
    else if(step.type == StepTypeV3::NEW_NODE){
        Block* node1 = step.focusNode;
        Block* &node2 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyNewNode : newNode;
        Block* node3 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyRoot : rootV3;
        if(!isCalculatingHistoryV3 && step.focusNode) node1 = liveToDummyMapV3[step.focusNode];

        if(firstTime){
            firstTime = false;
            node2 = step.focusNode;
            // if(isCalculatingHistoryV3) newNode = step.focusNode;
            node1 -> setFillColor(sf::Color::Green);
            node1 -> currentPosition = {100.f, 300.f};
            node1 -> targetPosition = {100.f, 300.f};
            node1 -> setPosition(node1 -> currentPosition);
        }
        
        // std::cout << "?2\n"; 
        drawAVLTree(node3, window);
        
        if(!isCalculatingHistoryV3) node1->draw(window);

        if(isCalculatingHistoryV3 || checkNextStepV3(0.5f, node3, node2)){
            currentStepIdxV3++;
            if(!isCalculatingHistoryV3) historyV3[currentStepIdxV3]->pull(dummySetV3, liveToDummyMapV3);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV3::INSERT){
        std::cout << "insert 1\n";
        Block* node1 = step.focusNode;
        Block* &node2 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyNewNode : newNode;
        Block* &node3 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyRoot : rootV3;
        if(!isCalculatingHistoryV3 && step.focusNode) node1 = liveToDummyMapV3[step.focusNode];

        if(firstTime){
            firstTime = false;
            if(step.focusNodeVal == -1){
                // std::cout << step.focusNode -> getLabel() << "->" << newNode -> getLabel() << std::endl;
                node1 -> pLeft = node2;
            }
            else if(step.focusNodeVal == 1){
                // std::cout << step.focusNode -> getLabel() << "->" << newNode -> getLabel() << std::endl;
                node1 -> pRight = node2;
            }
            else{
                node3 = node2;
                // if(isCalculatingHistoryV3) rootV3 = newNode;
            }
            node2 = nullptr;
            // if(isCalculatingHistoryV3) newNode = nullptr;
       }
        calculateAllPos(node3, getHeight(node3), 1, WINDOW_WIDTH / 2, 300.f);
        drawAVLTree(node3, window);
        if( isCalculatingHistoryV3 || checkNextStepV3(0.5f, node3, node2)){
            currentStepIdxV3++;
            if(!isCalculatingHistoryV3) historyV3[currentStepIdxV3]->pull(dummySetV3, liveToDummyMapV3);
            choosePrevNextButton = 0;
            firstTime = true;
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
        Block* cloneNode5 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyNewNode : newNode;
        Block* &cloneNode6 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyRoot : rootV3;

        if(firstTime){
            std::cout << "rotate left left 1\n";
            firstTime = false;

            Block* prenode = nullptr;
            Block* node1 = nullptr;
            Block* node2 = nullptr;
            Block* node3 = nullptr;
            Block* clonePrenode = nullptr;
            Block* cloneNode1 = nullptr;
            Block* cloneNode2 = nullptr;
            Block* cloneNode3 = nullptr;

            
            if(!isCalculatingHistoryV3){
                clonePrenode = (step.focusAnotherNode) ? liveToDummyMapV3[step.focusAnotherNode] : nullptr;
                cloneNode1 = (step.focusNode) ? liveToDummyMapV3[step.focusNode] : nullptr;
                cloneNode2 = cloneNode1 -> pLeft;
                cloneNode3 = cloneNode2 -> pRight;
            }
            else{
                prenode = step.focusAnotherNode;
                node1 = step.focusNode;
                node2 = node1 -> pLeft;
                node3 = node2 -> pRight;

                clonePrenode = prenode;
                cloneNode1 = node1;
                cloneNode2 = node2;
                cloneNode3 = node3;
            }

            if(!clonePrenode){
                cloneNode6 = cloneNode2;
                // if(isCalculatingHistoryV3) rootV3 = node2;
            }
            else if(step.focusNodeVal == -1) clonePrenode -> pLeft = cloneNode2;
            else if( step.focusNodeVal == 1) clonePrenode -> pRight = cloneNode2;
            cloneNode1 -> pLeft = cloneNode3;
            cloneNode2 -> pRight = cloneNode1;
            calculateAllPos(cloneNode6, getHeight(cloneNode6), 1, WINDOW_WIDTH / 2, 300.f);
        }
        drawAVLTree(cloneNode6, window);
        if( isCalculatingHistoryV3 || checkNextStepV3(0.75f, cloneNode6, cloneNode5)){
            currentStepIdxV3++;
            if(!isCalculatingHistoryV3) historyV3[currentStepIdxV3]->pull(dummySetV3, liveToDummyMapV3);
            choosePrevNextButton = 0;
            firstTime = true;
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
        Block* cloneNode5 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyNewNode : newNode;
        Block* &cloneNode6 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyRoot : rootV3;

        if(firstTime){
            std::cout << "rotate right right 1\n";
            firstTime = false;

            Block* prenode = nullptr;
            Block* node1 = nullptr;
            Block* node2 = nullptr;
            Block* node3 = nullptr;
            Block* clonePrenode = nullptr;
            Block* cloneNode1 = nullptr;
            Block* cloneNode2 = nullptr;
            Block* cloneNode3 = nullptr;

            if(!isCalculatingHistoryV3){
                clonePrenode = ( step.focusAnotherNode == nullptr ? nullptr : liveToDummyMapV3[step.focusAnotherNode]);
                cloneNode1 = liveToDummyMapV3[step.focusNode];
                cloneNode2 = cloneNode1 -> pRight;
                cloneNode3 = cloneNode2 -> pLeft;
            }
            else{
                prenode = step.focusAnotherNode;
                node1 = step.focusNode;
                node2 = node1 -> pRight;
                node3 = node2 -> pLeft;

                clonePrenode = prenode;
                cloneNode1 = node1;
                cloneNode2 = node2;
                cloneNode3 = node3;
            }




            std::cout << "rotate right right 1.5\n";
            if(!clonePrenode){
                cloneNode6 = cloneNode2;
                // if(isCalculatingHistoryV3) rootV3 = node2;
            }
            else if(step.focusNodeVal == -1) clonePrenode -> pLeft = cloneNode2;
            else if( step.focusNodeVal == 1) clonePrenode -> pRight = cloneNode2;
            cloneNode1 -> pRight = cloneNode3;
            cloneNode2 -> pLeft = cloneNode1;
            calculateAllPos(cloneNode6, getHeight(cloneNode6), 1, WINDOW_WIDTH / 2, 300.f);
        }
        drawAVLTree(cloneNode6, window);
        if( isCalculatingHistoryV3 || checkNextStepV3(0.75f, cloneNode6, cloneNode5)){
            currentStepIdxV3++;
            if(!isCalculatingHistoryV3) historyV3[currentStepIdxV3]->pull(dummySetV3, liveToDummyMapV3);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV3::DELETE){
        Block* node5 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyNewNode : newNode;
        Block* &node6 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyRoot : rootV3;

        
        if(firstTime){
            firstTime = false;

            Block* replaceNode = nullptr;
            Block* prenode = nullptr;
            Block* node = nullptr;
            Block* child = nullptr;
            Block* cloneReplaceNode = nullptr;
            Block* clonePrenode = nullptr;
            Block* cloneNode = nullptr;
            Block* cloneChild = nullptr;

            if(!isCalculatingHistoryV3){
                cloneReplaceNode = liveToDummyMapV3[step.deletedNode];
                clonePrenode = ( step.focusAnotherNode == nullptr ? nullptr : liveToDummyMapV3[step.focusAnotherNode]);                
                cloneNode = liveToDummyMapV3[step.focusNode];
                cloneChild = (cloneNode -> pLeft == nullptr) ? cloneNode -> pRight : cloneNode -> pLeft;
            }
            else{
                replaceNode = step.deletedNode;
                prenode = step.focusAnotherNode;
                node = step.focusNode;
                child = (node -> pLeft == nullptr) ? node -> pRight : node -> pLeft;
                
                cloneReplaceNode = replaceNode;
                clonePrenode = prenode;
                cloneNode = node;
                cloneChild = child;
            }


            if(clonePrenode){
                if(step.focusNodeVal == -1) clonePrenode -> pLeft = cloneChild;
                else if(step.focusNodeVal == 1) clonePrenode -> pRight = cloneChild;
            }
            else{
                node6 = cloneChild;
                // if(isCalculatingHistoryV3) rootV3 = child;
            }

            
            if(cloneReplaceNode){
                cloneNode -> pLeft = cloneReplaceNode -> pLeft;
                cloneNode -> pRight = cloneReplaceNode -> pRight;
                *cloneReplaceNode = *cloneNode;
            }
            else
                std::cout << "?";

            if(isCalculatingHistoryV3){
                garbageV3.push_back(cloneNode);
            }
            cloneNode = nullptr;

            calculateAllPos(node6, getHeight(node6), 1, WINDOW_WIDTH / 2, 300.f);
        }
        std::cout << ">\n";
        drawAVLTree(node6, window);
        std::cout << ">\n";
        if(isCalculatingHistoryV3 || checkNextStepV3(0.75f, node6, node5)){
            currentStepIdxV3++;
            if(!isCalculatingHistoryV3) historyV3[currentStepIdxV3]->pull(dummySetV3, liveToDummyMapV3);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV3::SEARCH){
        Block* node1 = step.focusNode;
        Block* node2 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyNewNode : newNode;
        Block* node3 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyRoot : rootV3;
        if(!isCalculatingHistoryV3 && step.focusNode) node1 = liveToDummyMapV3[step.focusNode];
        if(firstTime){
            firstTime = false;
        }
        if(step.focusNodeVal == 1){
            node1 -> setFillColor(sf::Color::Green);
            if(!isCalculatingHistoryV3) setLog("Found", sf::Color::Green);
        }
        else if(step.focusNodeVal == -1){
            if(!isCalculatingHistoryV3) setLog("Not found", sf::Color::Red);
        }
        drawAVLTree(node3, window);
        if( isCalculatingHistoryV3 || checkNextStepV3(0.5f, node2, node3)){
            currentStepIdxV3++;
            if(!isCalculatingHistoryV3) historyV3[currentStepIdxV3]->pull(dummySetV3, liveToDummyMapV3);
            choosePrevNextButton = 0;
            firstTime = true;

        }
    }
    else if(step.type == StepTypeV3::HIGHLIGHT_1){
        Block* node2 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyNewNode : newNode;
        Block* node3 = (!isCalculatingHistoryV3) ? historyV3[currentStepIdxV3] -> outDummyRoot : rootV3;

        if(firstTime){
            firstTime = false;
        }
        drawAVLTree(node3, window);
        if( isCalculatingHistoryV3 || checkNextStepV3(0.5f, node2, node3)){
            currentStepIdxV3++;
            if(!isCalculatingHistoryV3) historyV3[currentStepIdxV3]->pull(dummySetV3, liveToDummyMapV3);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV3::FINISH){
        // drawAVLTree(rootV3, window);
        choosePrevNextButton = 0;
        firstTime = true;
        std::cout << "finish\n";
    }
}

void getPosV3(Block* node) {
    if(node == nullptr) return;
    getPosV3(node -> pLeft);
    getRectangleMinMax(node -> getPosition());
    getPosV3(node -> pRight);
}

void performVisualization3(sf::RenderWindow& window) {
    if(currentStepIdxV3 == -1 || currentStepIdxV3 >= scriptV3.size()) return;
    if(isCalculatingHistoryV3){
        resetRectangleMinMax();
        currentStepIdxV3 = 0;
        firstTime = true;
        while(currentStepIdxV3 + 1 < scriptV3.size()){
            historyV3.push_back(new cloneVisualization3(rootV3, newNode, scriptV3[currentStepIdxV3]));
            getPosV3(rootV3);
            if(newNode) getRectangleMinMax(newNode -> getPosition());
            runV3(window);
        }
        if(currentStepIdxV3 + 1 == scriptV3.size()) {
            historyV3.push_back(new cloneVisualization3(rootV3, newNode, scriptV3[currentStepIdxV3]));
            getPosV3(rootV3);
            if(newNode) getRectangleMinMax(newNode -> getPosition());
        }
        isCalculatingHistoryV3 = false;
        currentStepIdxV3 = 0;
        historyV3[0]->pull(dummySetV3, liveToDummyMapV3);
        firstTime = true;
    }
    std::cout << "currentStepIdxV3: " << currentStepIdxV3 << " " << scriptV3.size() << std::endl;
    if(isStepByStep) {
        if(choosePrevNextButton == -1 && currentStepIdxV3 > 0){
            currentStepIdxV3--;
            std::cout << "before pull: \n";
            historyV3[currentStepIdxV3]->pull(dummySetV3, liveToDummyMapV3);
            std::cout << "after pull: \n";
            choosePrevNextButton = 0;
            firstTime = true;
        }
        else if(choosePrevNextButton == -1){
            choosePrevNextButton = 0;
        }
        else if(choosePrevNextButton == 1 && currentStepIdxV3 + 1 < scriptV3.size()){
            drawCodeHighlightPanel(3, currentStepIdxV3, isStepByStep, checkFinishedV3(), scriptV3[currentStepIdxV3].activeLines);
            runV3(window);
        }
        else if(choosePrevNextButton == 1){
            choosePrevNextButton = 0;
        }
        if(choosePrevNextButton == 0){
            // std::cout << currentStepIdxV3;
            // std::cout << ": outDummyRoot: " << ((historyV3[currentStepIdxV3]->outDummyRoot) ? historyV3[currentStepIdxV3]->outDummyRoot->getLabel() : "null");
            // std::cout << " outDummyNewNode: " << ((historyV3[currentStepIdxV3]->outDummyNewNode) ? historyV3[currentStepIdxV3]->outDummyNewNode->getLabel() : "null") << std::endl; 
            if(currentStepIdxV3 > 0)    drawCodeHighlightPanel(3, currentStepIdxV3 - 1, isStepByStep, checkFinishedV3(), scriptV3[currentStepIdxV3 - 1].activeLines);
            else                        drawCodeHighlightPanel(3, currentStepIdxV3 - 1, isStepByStep, checkFinishedV3(), {});
            drawAVLTree(historyV3[currentStepIdxV3]->outDummyRoot, window);
            if(historyV3[currentStepIdxV3]->outDummyNewNode) historyV3[currentStepIdxV3]->outDummyNewNode -> draw(window);
        }
    }
    else{
        if(currentStepIdxV3 > 0 && checkFinishedV3()) drawCodeHighlightPanel(3, currentStepIdxV3, isStepByStep, checkFinishedV3(), scriptV3[currentStepIdxV3 - 1].activeLines);
        else drawCodeHighlightPanel(3, currentStepIdxV3, isStepByStep, checkFinishedV3(), scriptV3[currentStepIdxV3].activeLines);
        runV3(window);
    }
}
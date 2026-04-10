#include "math.h"
#include "visualization4.h"
#include "performVisualization4.h"
#include "main.h"
#include "entity.h"
#include "draw.h"
#include "highlight.h"


std::vector<AnimationStepV4> scriptV4;
int currentStepIdxV4 = -1;
bool isCalculatingHistoryV4 = false;
std::vector<cloneVisualization4*> historyV4;     

std::vector<Block*> dummySetV4;
std::map<Block*, Block*> liveToDummyMapV4;

Block* cloneVisualization4::cloneNodeV4(Block* originalNode, std::map<Block*, Block*>& visited) {
    if (originalNode == nullptr) return nullptr;
    if (visited.find(originalNode) != visited.end()) {
        return visited[originalNode];
    }
    Block* clonedNode = new Block(*originalNode);
    mapping[clonedNode] = originalNode;
    visited[originalNode] = clonedNode;
    std::cout << "clone node: " << clonedNode -> getLabel() << "\n";
    
    for(auto const& child : originalNode->children) {
        std::cout << "children: " << child.second -> getLabel() << "\n";
        clonedNode->children[child.first] = cloneNodeV4(child.second, visited);
    }
    return clonedNode;
}
cloneVisualization4::cloneVisualization4(Block* root, Block* newNode, const AnimationStepV4& stepV4) {    
    std::map<Block*, Block*> visited;
    std::cout << "-------------------------\n";
    std::cout << "root:\n";
    this->rootV4 = cloneNodeV4(root, visited);
    std::cout << "newNode:\n";
    this->newNode = cloneNodeV4(newNode, visited);
    if(stepV4.focusNode){
        std::cout << "focusNode:\n";
        cloneNodeV4(stepV4.focusNode, visited);
    }
    if(stepV4.focusAnotherNode){
        std::cout << "focusAnotherNode:\n";
        cloneNodeV4(stepV4.focusAnotherNode, visited);
    }

    std::cout << "*********************************\n";
}

void cloneVisualization4::pull(std::vector<Block*>& dummySet, 
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
        for(auto child : cloneNode->children) {
            dummyNode->children[child.first] = cloneToDummy[child.second];
        }
    }


    this -> outDummyRoot = (this->rootV4 != nullptr) ? cloneToDummy[this->rootV4] : nullptr;
    this -> outDummyNewNode = (this->newNode != nullptr) ? cloneToDummy[this->newNode] : nullptr;
    std::cout << "outDummyNewNode: " << (this -> outDummyNewNode ? this -> outDummyNewNode -> getLabel() : "null") << "\n";
    std::cout << "outDummyRoot: " << (this -> outDummyRoot ? this -> outDummyRoot -> getLabel() : "null") << "\n";
}

cloneVisualization4::~cloneVisualization4() {
    // clone to dummy
    for (auto const& pair : mapping) {
        delete pair.first; // clone node
    }
    mapping.clear();
}


void runV4(sf::RenderWindow& window) {
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
        Block* node1 = step.focusNode;
        Block* node2 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyNewNode : newNode;
        Block* node3 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyRoot : rootV4;
        if(!isCalculatingHistoryV4 && step.focusNode) node1 = liveToDummyMapV4[step.focusNode];

        if(firstTime){
            firstTime = false;
            node1 -> setFillColor(sf::Color::Yellow);
            node1 -> value += step.focusNodeVal;
            if(step.isFinished) node1 -> height++;
        }
        drawTrie(node3, window, node3);
        if(isCalculatingHistoryV4 || checkNextStepV4(0.5f, node3, node2)){
            currentStepIdxV4++;
            if(!isCalculatingHistoryV4) historyV4[currentStepIdxV4]->pull(dummySetV4, liveToDummyMapV4);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV4::NEW_NODE){
        Block* &node1 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyNewNode : newNode;
        Block* node2 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyRoot : rootV4;
        Block* node3 = step.focusNode;
        Block* node4 = step.focusAnotherNode;
        if(!isCalculatingHistoryV4 && step.focusNode) node3 = liveToDummyMapV4[step.focusNode];
        if(!isCalculatingHistoryV4 && step.focusAnotherNode) node4 = liveToDummyMapV4[step.focusAnotherNode];


        if(firstTime){
            firstTime = false;
            node1 = node3;
            node1 -> value = 1;
            node4 -> children[node1 -> getLabel()[0]] = node1;
            calculateTrieGapWidth(node2);
            calculateTriePos(node2, WINDOW_WIDTH / 2.f, 300.f);
            node4 -> children.erase(node1 -> getLabel()[0]);
            node1 -> currentPosition = node1 -> targetPosition + sf::Vector2f(0.f, 50.f); 
            node1 -> setPosition(node1 -> currentPosition);
            node1 -> targetPosition = node1 -> currentPosition;
            node1 -> setFillColor(sf::Color::Green);
        }
        drawTrie(node2, window, node2);
        if(!isCalculatingHistoryV4 && node1) node1 -> draw(window);
        std::cout << "end new node\n";
        if(isCalculatingHistoryV4 || checkNextStepV4(0.5f, node2, node1)){
            currentStepIdxV4++;
            if(!isCalculatingHistoryV4) historyV4[currentStepIdxV4]->pull(dummySetV4, liveToDummyMapV4);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV4::INSERT){
        Block* &node1 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyNewNode : newNode;
        Block* node2 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyRoot : rootV4;
        Block* node3 = step.focusNode;
        Block* node4 = step.focusAnotherNode;
        if(!isCalculatingHistoryV4 && step.focusNode) node3 = liveToDummyMapV4[step.focusNode];
        if(!isCalculatingHistoryV4 && step.focusAnotherNode) node4 = liveToDummyMapV4[step.focusAnotherNode];


        if(firstTime){
            firstTime = false;
            node4 -> children[node1 -> getLabel()[0]] = node1;
            calculateTrieGapWidth(node2);
            calculateTriePos(node2, WINDOW_WIDTH / 2.f, 300.f);
            node1 -> currentPosition = node1 -> targetPosition + sf::Vector2f(0.f, 50.f); 
            node1 -> setPosition(node1 -> currentPosition);
            node1 -> setFillColor(sf::Color::Green);
            if(step.isFinished) node1 -> height++;
            node1 = nullptr;
        }
        drawTrie(node2, window, node2);
        if(isCalculatingHistoryV4 || checkNextStepV4(0.5f, node2, node1)){
            currentStepIdxV4++;
            if(!isCalculatingHistoryV4) historyV4[currentStepIdxV4]->pull(dummySetV4, liveToDummyMapV4);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV4::DELETE){
        Block* &node1 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyNewNode : newNode;
        Block* node2 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyRoot : rootV4;
        Block* node3 = step.focusNode;
        Block* node4 = step.focusAnotherNode;
        if(!isCalculatingHistoryV4 && step.focusNode) node3 = liveToDummyMapV4[step.focusNode];
        if(!isCalculatingHistoryV4 && step.focusAnotherNode) node4 = liveToDummyMapV4[step.focusAnotherNode];

        if(firstTime){
            firstTime = false;
            node3 -> value --;
            if(step.isFinished) node3 -> height--;
            if(node3 -> value == 0)    node3 -> setFillColor(sf::Color::Red);
            else                       node3 -> setFillColor(sf::Color(255, 165, 0));
        }
        drawTrie(node2, window, node2);
        drawTrie(node3, window, node2);
        if(isCalculatingHistoryV4 || checkNextStepV4(0.5f, node2, node1)){
            if(node3 -> value == 0){
                if(node4)
                    node4 -> children.erase(node3 -> getLabel()[0]);
                // delete step.focusNode;
                std::cout << "##### delete step.focusNode\n";
                std::cout << node3 << "\n";
                std::cout << node3 -> getLabel() << std::endl;
                if(isCalculatingHistoryV4) garbageV4.push_back(node3);
            }
            currentStepIdxV4++;
            if(!isCalculatingHistoryV4) historyV4[currentStepIdxV4]->pull(dummySetV4, liveToDummyMapV4);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV4::SEARCH){
        Block* node1 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyNewNode : newNode;
        Block* node2 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyRoot : rootV4;
        if(firstTime){
            firstTime = false;
            if(!isCalculatingHistoryV4)
                if(step.focusNodeVal == 1) setLog("Found!", sf::Color::Green);
                else setLog("Not found!", sf::Color::Red);
        }
        drawTrie(node2, window, node2);
        if(isCalculatingHistoryV4 || checkNextStepV4(0.5f, node2, node1)){
            currentStepIdxV4++;
            if(!isCalculatingHistoryV4) historyV4[currentStepIdxV4]->pull(dummySetV4, liveToDummyMapV4);
            choosePrevNextButton = 0;
            firstTime = true;
        }
    }
    else if(step.type == StepTypeV4::HIGHLIGHT_1){
        Block* node1 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyNewNode : newNode;
        Block* node2 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyRoot : rootV4;
        if(firstTime){
            firstTime = false;
        }
        drawTrie(node2, window, node2);
        if(isCalculatingHistoryV4 || checkNextStepV4(0.5f, node2, node1)){
            currentStepIdxV4++;
            if(!isCalculatingHistoryV4) historyV4[currentStepIdxV4]->pull(dummySetV4, liveToDummyMapV4);
            choosePrevNextButton = 0;
            firstTime = true;
        }        
    }
    else if(step.type == StepTypeV4::HIGHLIGHT_2){
        Block* node1 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyNewNode : newNode;
        Block* node2 = (!isCalculatingHistoryV4) ? historyV4[currentStepIdxV4] -> outDummyRoot : rootV4;
        Block* node3 = step.focusNode;
        if(!isCalculatingHistoryV4 && step.focusNode) node3 = liveToDummyMapV4[step.focusNode];
        if(firstTime){
            firstTime = false;
        }
        drawTrie(node2, window, node2);
        drawTrie(node3, window, node2);
        if(isCalculatingHistoryV4 || checkNextStepV4(0.5f, node2, node1)){
            currentStepIdxV4++;
            if(!isCalculatingHistoryV4) historyV4[currentStepIdxV4]->pull(dummySetV4, liveToDummyMapV4);
            choosePrevNextButton = 0;
            firstTime = true;
        }    
    }
    else if(step.type == StepTypeV4::FINISH){
        choosePrevNextButton = 0;
        firstTime = true;

    }


}

void getPosV4(Block* node) {
    if(node == nullptr) return;
    getRectangleMinMax(node -> getPosition());
    for(auto &child:node -> children) getPosV4(child.second);
}

void performVisualization4(sf::RenderWindow& window) {
    if(currentStepIdxV4 == -1 || currentStepIdxV4 >= scriptV4.size()) return;
    if(isCalculatingHistoryV4){
        std::cout << "---" << minX << " " << maxX << " " << minY << " " << maxY << std::endl;
        resetRectangleMinMax();
        currentStepIdxV4 = 0;
        firstTime = true;
        while(currentStepIdxV4 + 1 < scriptV4.size()){
            historyV4.push_back(new cloneVisualization4(rootV4, newNode, scriptV4[currentStepIdxV4]));
            getPosV4(rootV4);
            if(newNode) getRectangleMinMax(newNode -> getPosition());
            runV4(window);
        }
        if(currentStepIdxV4 + 1 == scriptV4.size()) {
            historyV4.push_back(new cloneVisualization4(rootV4, newNode, scriptV4[currentStepIdxV4]));
            getPosV4(rootV4);
            if(newNode) getRectangleMinMax(newNode -> getPosition());
        }
        isCalculatingHistoryV4 = false;
        currentStepIdxV4 = 0;
        historyV4[0]->pull(dummySetV4, liveToDummyMapV4);
        firstTime = true;
        std::cout << "---" << minX << " " << maxX << " " << minY << " " << maxY << std::endl;
    }
    std::cout << "currentStepIdxV4: " << currentStepIdxV4 << " " << scriptV4.size() << std::endl;
    if(isStepByStep) {
        if(choosePrevNextButton == -1 && currentStepIdxV4 > 0){
            currentStepIdxV4--;
            std::cout << "before pull: \n";
            historyV4[currentStepIdxV4]->pull(dummySetV4, liveToDummyMapV4);
            std::cout << "after pull: \n";
            choosePrevNextButton = 0;
            firstTime = true;
        }
        else if(choosePrevNextButton == -1){
            choosePrevNextButton = 0;
        }
        else if(choosePrevNextButton == 1 && currentStepIdxV4 + 1 < scriptV4.size()){
            drawCodeHighlightPanel(4, currentStepIdxV4, isStepByStep, checkFinishedV4(), scriptV4[currentStepIdxV4].activeLines);
            runV4(window);
        }
        else if(choosePrevNextButton == 1){
            choosePrevNextButton = 0;
        }
        if(choosePrevNextButton == 0){
            // std::cout << currentStepIdxV4;
            // std::cout << ": outDummyRoot: " << ((historyV4[currentStepIdxV4]->outDummyRoot) ? historyV4[currentStepIdxV4]->outDummyRoot->getLabel() : "null");
            // std::cout << " outDummyNewNode: " << ((historyV4[currentStepIdxV4]->outDummyNewNode) ? historyV4[currentStepIdxV4]->outDummyNewNode->getLabel() : "null") << std::endl; 
            if(currentStepIdxV4 > 0)    drawCodeHighlightPanel(4, currentStepIdxV4 - 1, isStepByStep, checkFinishedV4(), scriptV4[currentStepIdxV4 - 1].activeLines);
            else                        drawCodeHighlightPanel(4, currentStepIdxV4 - 1, isStepByStep, checkFinishedV4(), {});
            drawTrie(historyV4[currentStepIdxV4]->outDummyRoot, window, historyV4[currentStepIdxV4]->outDummyRoot);
            if(historyV4[currentStepIdxV4]->outDummyNewNode) historyV4[currentStepIdxV4]->outDummyNewNode -> draw(window);
            if(scriptV4[currentStepIdxV4].type == StepTypeV4::DELETE || scriptV4[currentStepIdxV4].type == StepTypeV4::HIGHLIGHT_2){
                Block* node2 = historyV4[currentStepIdxV4]->outDummyRoot;
                Block* node3 = scriptV4[currentStepIdxV4].focusNode;
                node3 = liveToDummyMapV4[node3];
                if(node3 -> value == 1) drawTrie(node3, window, node2);
            }
        }
    }
    else{
        if(currentStepIdxV4 > 0 && checkFinishedV4()) drawCodeHighlightPanel(4, currentStepIdxV4, isStepByStep, checkFinishedV4(), scriptV4[currentStepIdxV4 - 1].activeLines);
        else drawCodeHighlightPanel(4, currentStepIdxV4, isStepByStep, checkFinishedV4(), scriptV4[currentStepIdxV4].activeLines);
        runV4(window);
    }
}

bool checkFinishedV4() {
    std::cout << currentStepIdxV4 << " " << scriptV4.size() << std::endl;
    return currentStepIdxV4 == -1 || currentStepIdxV4 + 1== scriptV4.size();
}


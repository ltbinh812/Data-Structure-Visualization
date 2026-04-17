#include <math.h>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include "entity.h"
#include "performVisualization3.h"
#include "visualization3.h"
#include "draw.h"
#include "theme.h"

//AVL tree
float dtV3 = 1.0f;
Block *rootV3 = nullptr;

bool isWaitingV3 = false;
float delayTimerV3 = 0;


Block* targetDeleteNodeV3 = nullptr;
std::vector<Block*> garbageV3;


void clearALVTree(Block* node) {
    if (node == nullptr) return;
    clearALVTree(node->pLeft);
    clearALVTree(node->pRight);
    delete node;
}


void initStatus3() {
    // visualization3.cpp
    dtV3 = 1.0f;
    clearALVTree(rootV3);
    rootV3 = nullptr;
    isWaitingV3 = false;
    delayTimerV3 = 0;
    targetDeleteNodeV3 = nullptr;
    for(auto& garbage:garbageV3) delete garbage;
    garbageV3.clear();

    // performVisualization3.cpp
    scriptV3.clear();
    scriptV3.push_back({{}, nullptr, -1, "", StepTypeV3::FINISH});
    currentStepIdxV3 = 0;
    isCalculatingHistoryV3 = false;
    for(auto& clone:dummySetV3)
        delete clone;
    dummySetV3.clear();
    liveToDummyMapV3.clear();
    historyV3.clear();
    historyV3.push_back(new cloneVisualization3(rootV3, newNode, scriptV3[0]));

    // draw.cpp
    Log = nullptr;
    delayLog = 0;
    o = INITIALIZE;
    isStepByStep = false;
    choosePrevNextButton = 0;
    if (newNode) {
        delete newNode;
        newNode = nullptr;
    }
    firstTime = true;

    // main.cpp
    resetRectangleMinMax();
}

int getHeight(Block* node) {
    if (node == nullptr) return 0;
    return 1 + std::max(getHeight(node->pLeft), getHeight(node->pRight));
}

void calculateAllPos(Block* node, int heightTree, int level,  float x, float y) {
    std::cout << ".\n" << std::endl;
    if (node == nullptr) return;
    std::cout << ".\n";
    node->targetPosition = sf::Vector2f(x, y);
    std::cout << "{" << x << "," << y << "}" << " " << level << " " << heightTree << std::endl;
    std::cout << ".\n";

    float minGap = 80.f; // Khoảng cách nhỏ nhất ở tầng cuối (Level = heightTree)
    float vGap = 150.f;  // Khoảng cách đứng giữa các tầng

    float hOffset = 0;
    std::cout << ".\n";
    hOffset = minGap * std::pow(2.f, static_cast<float>(heightTree - 2 - level));
    std::cout << ".\n";

    calculateAllPos(node->pLeft, heightTree, level + 1, x - hOffset, y + vGap);
    std::cout << ".\n";
    calculateAllPos(node->pRight, heightTree, level + 1,  x + hOffset, y + vGap);
    std::cout << ".\n";
}


struct cloneNode{
    int value;
    int height;
    int balance;
    cloneNode* left;
    cloneNode* right;
    Block* realNode;
    cloneNode(Block* realNode, int value = 0, int height = -1, int balance = 0) {
        this -> realNode = realNode;
        this -> value = value;
        this -> height = height;
        this -> balance = balance;
        left = nullptr;
        right = nullptr;
    }
};
cloneNode *fakerootV3 = nullptr;

int getHeight(cloneNode* node) {
    if (node == nullptr) return 0;
    int leftH = (node->left) ? node->left->height : 0;
    int rightH = (node->right) ? node->right->height : 0;
    return 1 + std::max(leftH, rightH);
}

int getBalance(cloneNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right); 
}

cloneNode* cloneStructure(Block* realNode) {
    if (!realNode) return nullptr;
    cloneNode* newNode = new cloneNode(realNode, std::stoi(realNode -> getLabel()));
    newNode->left = cloneStructure(realNode->pLeft);
    newNode->right = cloneStructure(realNode->pRight);
    int heightLeft = 0;
    int heightRight = 0;
    if(newNode->left) heightLeft = newNode->left -> height;
    if(newNode->right) heightRight = newNode->right -> height;
    newNode -> height = std::max(heightLeft, heightRight) + 1;
    newNode -> balance = heightLeft - heightRight;
    return newNode;
}

void calcRotateLLcase(cloneNode* fakenode, cloneNode* prefakenode, int direction, bool fixRealNode = 0) {
    cloneNode* temp = fakenode -> left;
    fakenode -> left = temp -> right;                       if(fixRealNode) fakenode -> realNode -> pLeft = temp -> realNode -> pRight;
    temp -> right = fakenode;                               if(fixRealNode) temp -> realNode -> pRight = fakenode -> realNode;
    if(prefakenode){
        if(direction == -1){ prefakenode -> left = temp;    if(fixRealNode) prefakenode -> realNode -> pLeft = temp -> realNode;}
        else{ prefakenode -> right = temp;                  if(fixRealNode) prefakenode -> realNode -> pRight = temp -> realNode;}
    }
    fakenode -> height = getHeight(fakenode);
    temp -> height = getHeight(temp);

}

void calcRotateRRcase(cloneNode* fakenode, cloneNode* prefakenode, int direction, bool fixRealNode = 0) {
    cloneNode* temp = fakenode -> right;
    fakenode -> right = temp -> left;                       if(fixRealNode) fakenode -> realNode -> pRight = temp -> realNode -> pLeft;
    temp -> left = fakenode;                                if(fixRealNode) temp -> realNode -> pLeft = fakenode -> realNode;
    if(prefakenode){
        if(direction == -1) {prefakenode -> left = temp;    if(fixRealNode) prefakenode -> realNode -> pLeft = temp -> realNode;}
        else {prefakenode -> right = temp;                  if(fixRealNode) prefakenode -> realNode -> pRight = temp -> realNode;}
    }
    fakenode -> height = getHeight(fakenode);
    temp -> height = getHeight(temp);
}

RotationType checkRotationType(cloneNode* node) {
    if (node == nullptr) return RotationType::NONE;

    int balance = getBalance(node);
    std::cout << node -> value << ": " << balance << " ";
    std::cout << (node -> left ? node -> left -> height : 0) << " " << (node -> right ? node -> right -> height : 0) << "\n";
    if (balance > 1) {
        if (getBalance(node->left) >= 0)
            return RotationType::LEFT_LEFT; // Trường hợp LL
        else
            return RotationType::LEFT_RIGHT; // Trường hợp LR
    }

    // 2. Lệch Phải (Right heavy)
    if (balance < -1) {
        if (getBalance(node->right) <= 0)
            return RotationType::RIGHT_RIGHT; // Trường hợp RR
        else
            return RotationType::RIGHT_LEFT; // Trường hợp RL
    }

    return RotationType::NONE;
}

void clearFakeNode(cloneNode* node) {
    if (node == nullptr) return;
    clearFakeNode(node->left);
    clearFakeNode(node->right);
    delete node;
}

void insertScriptV3(Block* node, cloneNode* fakenode, int value, int direction, Block* prenode, cloneNode* prefakenode = nullptr) {
    if(node == nullptr) return;
    scriptV3.push_back({{42}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
    std::cout << "node: " << fakenode -> value << std::endl;
    if(value < std::stoi(node -> getLabel())){
        if(node -> pLeft == nullptr){
            newNode = new Block(CIRCLE, 30.f, std::to_string(value));
            scriptV3.push_back({{45}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
            scriptV3.push_back({{46, 42, 43}, newNode, -1, "", StepTypeV3::NEW_NODE, node});
            scriptV3.push_back({{46}, node, -1, "", StepTypeV3::INSERT});
            fakenode -> left = new cloneNode(newNode, value, 0, 0);
            fakenode -> left -> height = 1;
            newNode = nullptr;
        }
        else{
            scriptV3.push_back({{45}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
            scriptV3.push_back({{46, 42}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
        }
        insertScriptV3(node -> pLeft, fakenode -> left, value, -1, node, fakenode);
    } 
    else if(value >= std::stoi(node -> getLabel())) {
        if(node -> pRight == nullptr){
            newNode = new Block(CIRCLE, 30.f, std::to_string(value));
            scriptV3.push_back({{47}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
            scriptV3.push_back({{48, 42, 43}, newNode, 1, "", StepTypeV3::NEW_NODE, node});
            scriptV3.push_back({{48}, node, 1, "", StepTypeV3::INSERT});
            fakenode -> right = new cloneNode(newNode, value, 0, 0);
            fakenode -> right -> height = 1;
            newNode = nullptr;
        }
        else{
            scriptV3.push_back({{47}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
            scriptV3.push_back({{48, 42}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;            
        }
        insertScriptV3(node -> pRight, fakenode -> right, value, 1, node, fakenode);
    }
    fakenode -> height = 1;
    if(fakenode -> left) fakenode -> height = std::max(fakenode -> height, fakenode -> left -> height + 1);
    if(fakenode -> right) fakenode -> height = std::max(fakenode -> height, fakenode -> right -> height + 1);

    RotationType RoTa = checkRotationType(fakenode);
    if(RoTa == RotationType::LEFT_LEFT) {
        scriptV3.push_back({{50,51}, node, 1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;    
        scriptV3.push_back({{53, 22, 23, 24, 25, 26, 27, 28, 29, 30}, node, direction, "", StepTypeV3::ROTATE_LEFT_LEFT, prenode});   
        scriptV3.push_back({{53}, fakenode -> left -> realNode, 2, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
        calcRotateLLcase(fakenode, prefakenode, direction);
    }
    else if(RoTa == RotationType::RIGHT_RIGHT) {
        scriptV3.push_back({{50,51}, node, 1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
        scriptV3.push_back({{54, 32, 33, 34, 35, 36, 37, 38, 39, 40}, node, direction, "", StepTypeV3::ROTATE_RIGHT_RIGHT, prenode});    
        scriptV3.push_back({{54}, fakenode -> right -> realNode, 2, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
        calcRotateRRcase(fakenode, prefakenode, direction);
    }
    else if(RoTa == RotationType::LEFT_RIGHT) {
        scriptV3.push_back({{50,51}, node, 1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
        scriptV3.push_back({{55, 56, 32, 33, 34, 35, 36, 37, 38, 39, 40}, node -> pLeft, -1, "", StepTypeV3::ROTATE_RIGHT_RIGHT, node});        
        scriptV3.push_back({{61, 22, 23, 24, 25, 26, 27, 28, 29, 30}, node, direction, "", StepTypeV3::ROTATE_LEFT_LEFT, prenode});        
        scriptV3.push_back({{61}, fakenode -> left -> right -> realNode, 2, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
        calcRotateRRcase(fakenode -> left, fakenode, -1);
        calcRotateLLcase(fakenode, prefakenode, direction);
    }
    else if(RoTa == RotationType::RIGHT_LEFT) {
        scriptV3.push_back({{50,51}, node, 1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
        scriptV3.push_back({{59, 60, 22, 23, 24, 25, 26, 27, 28, 29, 30}, node -> pRight, 1, "", StepTypeV3::ROTATE_LEFT_LEFT, node});
        scriptV3.push_back({{61, 32, 33, 34, 35, 36, 37, 38, 39, 40}, node, direction, "", StepTypeV3::ROTATE_RIGHT_RIGHT, prenode});        
        scriptV3.push_back({{61}, fakenode -> right -> left -> realNode, 2, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
        calcRotateLLcase(fakenode -> right, fakenode, 1);
        calcRotateRRcase(fakenode, prefakenode, direction);
    }
    else{
        scriptV3.push_back({{50,51}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
        scriptV3.push_back({{63}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = scriptV3.back().focusNode;
    }
}

cloneNode* findMaximumV3(cloneNode* fakenode) {
    if(fakenode -> right == nullptr) return targetDeleteNodeV3 = fakenode -> realNode, fakenode;
    return findMaximumV3(fakenode -> right);
}

cloneNode* deleteScriptV3(Block* node, cloneNode* fakenode, int value, int direction, cloneNode* prefakenode, Block* deletenode = nullptr) {
    if(fakenode == nullptr) return nullptr;
    std::cout << "&&&&&&&&&&&&&&" << fakenode -> value << " " << value << "\n";
    scriptV3.push_back({{72}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = node;
    if(value == std::stoi(node -> getLabel()) && (targetDeleteNodeV3 == nullptr || targetDeleteNodeV3 == node)) {
        if(deletenode == nullptr)   scriptV3.push_back({{79}, node, 3, "", StepTypeV3::TRAVERSE});
        else                        scriptV3.push_back({{79}, node, 4, "", StepTypeV3::TRAVERSE});    
        if(fakenode -> left == nullptr || fakenode -> right == nullptr) {
            scriptV3.push_back({{80, 81}, nullptr, -1, "", StepTypeV3::HIGHLIGHT_1});
            cloneNode *temp = fakenode -> left ? fakenode -> left : fakenode -> right;
            if(temp == nullptr){
                targetDeleteNodeV3 = nullptr;
                scriptV3.push_back({{83, 84}, node, direction, "", StepTypeV3::DELETE, (prefakenode ? prefakenode -> realNode : nullptr), deletenode});
                delete fakenode;
                fakenode = nullptr;
            }
            else {
                targetDeleteNodeV3 = nullptr;
                std::cout << fakenode -> realNode -> getLabel() << " is deleted\n";
                scriptV3.push_back({{82, 84}, node, direction, "", StepTypeV3::DELETE, (prefakenode ? prefakenode -> realNode : nullptr), deletenode});
                *fakenode = *temp;
                delete temp;
            }
        }
        else{
            scriptV3.push_back({{85}, nullptr, -1, "", StepTypeV3::HIGHLIGHT_1});
            scriptV3.push_back({{86, 66, 67, 68, 69, 70, 87, 88}, nullptr, -1, "", StepTypeV3::HIGHLIGHT_1});
            cloneNode* temp = findMaximumV3(fakenode -> left);
            fakenode -> value = temp -> value;
            fakenode -> left = deleteScriptV3(node -> pLeft, fakenode -> left, temp -> value, -1, fakenode, node); 
        }
    }
    else if(value < std::stoi(node -> getLabel())){
        scriptV3.push_back({{75, 76}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = node;
        fakenode -> left = deleteScriptV3(node -> pLeft, fakenode -> left, value, -1, fakenode, deletenode);
    }
    else{
        scriptV3.push_back({{77, 78}, node, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = node;
        fakenode -> right = deleteScriptV3(node -> pRight, fakenode -> right, value, 1, fakenode, deletenode);
    }
    if(fakenode == nullptr) return nullptr;
    fakenode -> height = getHeight(fakenode);

    std::cout << "check rotation type: \n";
    RotationType RoTa = checkRotationType(fakenode);
    if(RoTa == RotationType::LEFT_LEFT) {
        cloneNode* temp = fakenode -> left;
        scriptV3.push_back({{93, 94}, fakenode -> realNode, 1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> realNode;
        scriptV3.push_back({{96, 22, 23, 24, 25, 26, 27, 28, 29, 30}, fakenode -> realNode, direction, "", StepTypeV3::ROTATE_LEFT_LEFT, (prefakenode ? prefakenode -> realNode : nullptr)});   
        scriptV3.push_back({{96}, fakenode -> left -> realNode, 2, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> left -> realNode;
        calcRotateLLcase(fakenode, prefakenode, direction);
        return temp;
    }
    else if(RoTa == RotationType::RIGHT_RIGHT) {
        cloneNode* temp = fakenode -> right;
        scriptV3.push_back({{93, 94}, fakenode -> realNode, 1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> realNode;
        scriptV3.push_back({{97, 32, 33, 34, 35, 36, 37, 38, 39, 40}, fakenode -> realNode, direction, "", StepTypeV3::ROTATE_RIGHT_RIGHT, (prefakenode ? prefakenode -> realNode : nullptr)});    
        scriptV3.push_back({{97}, fakenode -> right -> realNode, 2, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> right -> realNode;
        calcRotateRRcase(fakenode, prefakenode, direction);
        return temp;
    }
    else if(RoTa == RotationType::LEFT_RIGHT) {
        cloneNode* temp = fakenode -> left -> right;
        scriptV3.push_back({{93, 94}, fakenode -> realNode, 1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> realNode;
        scriptV3.push_back({{98, 99, 32, 33, 34, 35, 36, 37, 38, 39, 40}, fakenode -> realNode -> pLeft, -1, "", StepTypeV3::ROTATE_RIGHT_RIGHT, fakenode -> realNode});        
        scriptV3.push_back({{100, 22, 23, 24, 25, 26, 27, 28, 29, 30}, fakenode -> realNode, direction, "", StepTypeV3::ROTATE_LEFT_LEFT, (prefakenode ? prefakenode -> realNode : nullptr)});        
        scriptV3.push_back({{100}, fakenode -> left -> right -> realNode, 2, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> left -> right -> realNode;
        calcRotateRRcase(fakenode -> left, fakenode, -1);
        calcRotateLLcase(fakenode, prefakenode, direction);
        return temp;
    }
    else if(RoTa == RotationType::RIGHT_LEFT) {
        cloneNode* temp = fakenode -> right -> left;
        scriptV3.push_back({{93, 94}, fakenode -> realNode, 1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> realNode;
        scriptV3.push_back({{102, 103, 22, 23, 24, 25, 26, 27, 28, 29, 30}, fakenode -> realNode -> pRight, 1, "", StepTypeV3::ROTATE_LEFT_LEFT, fakenode -> realNode});
        scriptV3.push_back({{104, 32, 33, 34, 35, 36, 37, 38, 39, 40}, fakenode -> realNode, direction, "", StepTypeV3::ROTATE_RIGHT_RIGHT, (prefakenode ? prefakenode -> realNode : nullptr)});        
        scriptV3.push_back({{104}, fakenode -> right -> left -> realNode, 2, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> right -> left -> realNode;
        calcRotateLLcase(fakenode -> right, fakenode, 1);
        calcRotateRRcase(fakenode, prefakenode, direction);
        return temp;
    }
    else{
        scriptV3.push_back({{93, 94}, fakenode -> realNode, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> realNode;
        scriptV3.push_back({{106}, fakenode -> realNode, -1, "", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = fakenode -> realNode;
    }
    return fakenode;
}

cloneNode* initV3(cloneNode* fakenode, Block* node, cloneNode* prefakenode, int value, int direction) {
    if(fakenode == nullptr){
        node = new Block(CIRCLE, 30.f, std::to_string(value));
        fakenode = new cloneNode(node, value, 0, 0);
        fakenode -> height = 1;
        return fakenode;
    }
    if(value < std::stoi(fakenode -> realNode -> getLabel())){
        std::cout << fakenode -> value << " to left\n";
        fakenode -> left = initV3(fakenode -> left, node -> pLeft, fakenode, value, -1);
        node -> pLeft = fakenode -> left -> realNode;
    }
    else if(value >= std::stoi(fakenode -> realNode -> getLabel())){    
        std::cout << fakenode -> value << " to right\n";
        fakenode -> right = initV3(fakenode -> right, node -> pRight, fakenode, value, 1);
        node -> pRight = fakenode -> right -> realNode;
    }
    fakenode -> height = getHeight(fakenode);
    RotationType RoTa = checkRotationType(fakenode);
    if(RoTa == RotationType::LEFT_LEFT){
        cloneNode* temp = fakenode -> left;
        calcRotateLLcase(fakenode, prefakenode, direction, true);
        return temp;
    }
    else if(RoTa == RotationType::RIGHT_RIGHT) {
        cloneNode* temp = fakenode -> right;
        calcRotateRRcase(fakenode, prefakenode, direction, true);
        return temp;
    }
    else if(RoTa == RotationType::LEFT_RIGHT) {
        cloneNode* temp = fakenode -> left -> right;
        calcRotateRRcase(fakenode -> left, fakenode, -1, true);
        calcRotateLLcase(fakenode, prefakenode, direction, true);
        return temp;
    }
    else if(RoTa == RotationType::RIGHT_LEFT) {
        cloneNode* temp = fakenode -> right -> left;
        calcRotateLLcase(fakenode -> right, fakenode, 1, true);
        calcRotateRRcase(fakenode, prefakenode, direction, true);
        return temp;
    }
    return fakenode;
}

void setColorALVTree(Block* node, sf::Color color) {
    if (node == nullptr) return;
    node->setFillColor(color);
    setColorALVTree(node->pLeft, color);
    setColorALVTree(node->pRight, color);
}


void initVisualization3(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color,"Initialize a AVL tree:");
    ImGui::Spacing();

    static char inputBuffer[256] = "";
    bool temp = false;

    ImGui::Text("Enter the values to initialize: A[] = "); 
    ImGui::SameLine(); 
    ImGui::SetNextItemWidth(-1.0f); 
    ImGui::InputTextWithHint("##array_input", "Example: 1 2 3 4 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();
    ImGui::SameLine(540.f);
    if ((isStepByStep || checkFinishedV3()) && ImGui::Button("Random", ImVec2(100.0f, 30)))
    {
        int n = rand() % 15;
        std::string data = "";
        for(int i = 0; i < n; i++){
            int value = rand() % 500;
            data += std::to_string(value) + " ";
        }
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV3()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {

        std::string data(inputBuffer);
        std::stringstream ss(data);
        int value;
        std::vector<int> newElements;
        while (ss >> data) newElements.push_back(std::stoi(data));

        if (!newElements.empty()) {
            scriptV3.clear();
            currentStepIdxV3 = 0;

            clearALVTree(rootV3);
            clearFakeNode(fakerootV3);
            rootV3 = nullptr;
            fakerootV3 = nullptr;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            historyV3.clear();
            for(auto& garbage:garbageV3) delete garbage;
            garbageV3.clear();


            for(auto value:newElements){
                fakerootV3 = initV3(fakerootV3, rootV3, nullptr, value, 0);    
                rootV3 = fakerootV3 -> realNode;
            }
            calculateAllPos(rootV3, getHeight(rootV3), 1, WINDOW_WIDTH / 2, 300.f);
            scriptV3.push_back({{116, 117, 118, 119, 123, 120, 121}, nullptr, -1, "", StepTypeV3::HIGHLIGHT_1});
            scriptV3.push_back({{}, rootV3, -1, "", StepTypeV3::FINISH});
            isCalculatingHistoryV3 = true;
            firstTime = true;
            drawAVLTree(rootV3, window);
        }
    }

    ImGui::Spacing();
    ImGui::Text("Clear the AVL tree:");
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV3()) && ImGui::Button("Clear", ImVec2(100.0f, 30))) {
        scriptV3.clear();
        scriptV3.push_back({{}, nullptr, -1, "", StepTypeV3::FINISH});
        currentStepIdxV3 = 0;
        clearALVTree(rootV3);
        clearFakeNode(fakerootV3);
        rootV3 = nullptr;
        fakerootV3 = nullptr;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        for(auto& garbage:garbageV3) delete garbage;
        garbageV3.clear();
        historyV3.clear();
        isCalculatingHistoryV3 = true;
        firstTime = true;
    }
}

void insertVisualization3(sf::RenderWindow& window){
    ImGui::TextColored(title1Color, "Insert an element into the AVL tree:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the value to insert:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##insert_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::SameLine(); 



    if((isStepByStep || checkFinishedV3()) && ImGui::Button("Random", ImVec2(100.0f, 30))){
        int value = rand() % 500;
        std::string data = std::to_string(value);
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if((isStepByStep || checkFinishedV3()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))){
        std::string data(inputBuffer);

        if(data != ""){
            int value = std::stoi(data);
            scriptV3.clear();
            currentStepIdxV3 = 0;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            historyV3.clear();
            for(auto& garbage:garbageV3) delete garbage;
            garbageV3.clear();

            setColorALVTree(rootV3, sf::Color::White);
            clearFakeNode(fakerootV3);
            fakerootV3 = cloneStructure(rootV3);
            
            if(rootV3 != nullptr) {
                scriptV3.push_back({{124, 42}, nullptr, -1, "", StepTypeV3::HIGHLIGHT_1});
                insertScriptV3(rootV3, fakerootV3, value, 0, nullptr);
            }
            else{
                scriptV3.push_back({{124, 42}, nullptr, -1, "", StepTypeV3::HIGHLIGHT_1});
                newNode = new Block(CIRCLE, 30.f, std::to_string(value));
                scriptV3.push_back({{42, 43}, newNode, value, "Đã tạo xong AVL Tree!", StepTypeV3::NEW_NODE});
                scriptV3.push_back({{124}, nullptr, 0, "", StepTypeV3::INSERT});
                newNode = nullptr;
            }            
            scriptV3.push_back({{}, rootV3, value, "Đã tạo xong AVL Tree!", StepTypeV3::FINISH});
            
            for(int i = 0; i < scriptV3.size(); i++){
                AnimationStepV3 step = scriptV3[i];
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
                    std::cout << "delete\n";
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
            }

            isCalculatingHistoryV3 = true;
            firstTime = true;
        }
    }
}



void searchVisualization3(sf::RenderWindow& window){
    ImGui::TextColored(title1Color, "Search an element in the AVL tree:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the value to search:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##update_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::SameLine();

    if((isStepByStep || checkFinishedV3()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {

        std::vector<std::string> vec;
        auto get = [&](auto self, Block* node, std::vector<std::string> &vec) -> void {
            if(node == nullptr) return;
            std::cout << node -> getLabel() << std::endl;
            self(self, node -> pLeft, vec);
            vec.push_back(node -> getLabel());
            self(self, node -> pRight, vec);
            return;
        };
        get(get, rootV3, vec);
        if(!vec.empty()){
            std::string value = vec[rand() % vec.size()];
            strcpy(inputBuffer, value.c_str());
            temp = true;
        }
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV3()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)) && rootV3 != nullptr)) {
        std::string valueStr(inputBuffer);
        int value;
        if(valueStr != ""){
            value = std::stoi(valueStr);
            scriptV3.clear();
            currentStepIdxV3 = 0;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            historyV3.clear();
            for(auto& garbage:garbageV3) delete garbage;
            garbageV3.clear();

            setColorALVTree(rootV3, sf::Color::White);
            Block* node = rootV3;
            bool found = false;
            scriptV3.push_back({{132}, nullptr, -1, "", StepTypeV3::HIGHLIGHT_1});
            while(node){
                scriptV3.push_back({{109}, node, -1, "Đã tạo xong AVL Tree!", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = node;
                if(std::stoi(node -> getLabel()) == value){
                    scriptV3.push_back({{110}, node, -1, "Đã tạo xong AVL Tree!", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = node;
                    found = true;
                    break;
                }
                if(std::stoi(node -> getLabel()) > value){
                    scriptV3.push_back({{111}, node, -1, "Đã tạo xong AVL Tree!", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = node;
                    node = node -> pLeft;
                }
                else{
                    scriptV3.push_back({{112}, node, -1, "Đã tạo xong AVL Tree!", StepTypeV3::TRAVERSE}); scriptV3.back().setWhiteNode = node;
                    node = node -> pRight;
                }
            }
            if(found)   scriptV3.push_back({{132}, node, 1, "Đã tạo xong AVL Tree!", StepTypeV3::SEARCH});
            else        scriptV3.push_back({{132}, nullptr, -1, "Đã tạo xong AVL Tree!", StepTypeV3::SEARCH});
            scriptV3.push_back({{}, rootV3, value, "Đã tạo xong AVL Tree!", StepTypeV3::FINISH});
            isCalculatingHistoryV3 = true;
            firstTime = true;
        }
    }
} 
void deleteVisualization3(sf::RenderWindow& window){
    ImGui::TextColored(title1Color, "Delete an element in AVL tree:");
    ImGui::Spacing();
    
    static char valueBuffer[256] = "";
    bool temp = false;

    ImGui::Text("Enter the value to delete:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##delete_input", "Example: 5", valueBuffer, IM_ARRAYSIZE(valueBuffer));
    ImGui::SameLine();

    if((isStepByStep || checkFinishedV3()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        std::vector<std::string> vec;
        auto get = [&](auto self, Block* node, std::vector<std::string> &vec) -> void {
            if(node == nullptr) return;
            std::cout << node -> getLabel() << std::endl;
            self(self, node -> pLeft, vec);
            vec.push_back(node -> getLabel());
            self(self, node -> pRight, vec);
            return;
        };
        get(get, rootV3, vec);
        if(!vec.empty()){
            std::string value = vec[rand() % vec.size()];
            strcpy(valueBuffer, value.c_str());
            temp = true;
        }

    }
    ImGui::SameLine();
    if((isStepByStep || checkFinishedV3()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        std::string data(valueBuffer);
        if(data == "") return;

        int value = std::stoi(data);
        scriptV3.clear();
        currentStepIdxV3 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        historyV3.clear();
        for(auto& garbage:garbageV3) delete garbage;
        garbageV3.clear();
        setColorALVTree(rootV3, sf::Color::White);
        clearFakeNode(fakerootV3);
        fakerootV3 = cloneStructure(rootV3);

        scriptV3.push_back({{132, 72}, nullptr, -1, "", StepTypeV3::HIGHLIGHT_1});
        fakerootV3 = deleteScriptV3(rootV3, fakerootV3, value, 0, nullptr);
        scriptV3.push_back({{}, nullptr, -1, "Deleted!", StepTypeV3::FINISH});
        isCalculatingHistoryV3 = true;
        firstTime = true;

        if(fakerootV3 == nullptr){
            std::cout << "nubacachi~" << std::endl;
        }
        else{
            std::cout << "chicabanu~" << std::endl;
        }

        for(int i = 0; i < scriptV3.size(); i++){
            AnimationStepV3 step = scriptV3[i];
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
                std::cout << "delete\n";
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
        }
    }
}


bool DFScheckMoveV3(Block* node) {
    if(!node) return false;
    if(checkMove(node)) return true;
    return DFScheckMoveV3(node -> pLeft) || DFScheckMoveV3(node -> pRight);
}

bool checkNextStepV3(float limitTime, Block* rootV3, Block* newNode) {
    isWaitingV3 = true;
    
    if (newNode && checkMove(newNode)) isWaitingV3 = false;

    if(DFScheckMoveV3(rootV3)) isWaitingV3 = false;

    if(isWaitingV3){ 
        delayTimerV3 += dealtaTime.asSeconds() * dtV3; 
        std::cout << "**********************************" << delayTimerV3 << "\n";
        if (delayTimerV3 >= limitTime) { 
            isWaitingV3 = false;
            delayTimerV3 = 0;
            return true; 
        }
    }
    
    return false;
}

void drawAVLTree(Block* node, sf::RenderWindow& window) {
    if(!node) return;
    drawAVLTree(node -> pLeft, window);
    drawAVLTree(node -> pRight, window);
    node -> move(dtV3, isCalculatingHistoryV3);
    if(!isCalculatingHistoryV3) node -> draw(window);
    node -> height = 1;    
    int heightL = (node -> pLeft) ? node -> pLeft -> height : 0;
    int heightR = (node -> pRight) ? node -> pRight -> height : 0;
    if(node -> pLeft){
            sf::Vector2f direction = node -> pLeft -> center() - node -> center();
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if(length > eps)
                if(!isCalculatingHistoryV3) drawArrow(window, node -> center() + direction / length * node -> getRadius(), node -> pLeft -> center() - direction / length * node -> pLeft -> getRadius(), edgeColor);
    }
    if(node -> pRight){
            sf::Vector2f direction = node -> pRight -> center() - node -> center();
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if(length > eps)
                if(!isCalculatingHistoryV3) drawArrow(window, node -> center() + direction / length * node -> getRadius(), node -> pRight -> center() - direction / length * node -> pRight -> getRadius(), edgeColor);
    }
    node -> height = std::max(heightL, heightR) + 1;
    int balance = heightL - heightR;
    Text textBF(std::to_string(balance), style6);
    if(balance > 1 || balance < -1) textBF.setFillColor(sf::Color(191, 62, 255));
    textBF.setPosition(node -> center() - sf::Vector2f(0, 55));
    Text textH(std::to_string(node -> height), style6);
    textH.setFillColor(sf::Color(0, 139, 139));
    textH.setPosition(node -> center() + sf::Vector2f(36, 36));
    if(!isCalculatingHistoryV3) textBF.draw(window);
    if(!isCalculatingHistoryV3) textH.draw(window);
}
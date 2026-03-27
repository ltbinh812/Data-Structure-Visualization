#include <math.h>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include "entity.h"
#include "performVisualization3.h"
#include "visualization3.h"
#include "draw.h"

//AVL tree
float dtV3 = 7.0f;
Block *rootV3 = nullptr;

bool isWaitingV3 = false;
float delayTimerV3 = 0;


Block* targetDeleteNodeV3 = nullptr;

void initStatus3() {
}

int getHeight(Block* node) {
    if (node == nullptr) return 0;
    return 1 + std::max(getHeight(node->pLeft), getHeight(node->pRight));
}

// int getHeightBranch(Block* node, int value) {
//     if(node == nullptr) return 1;
//     if(value < std::stoi(node->getLabel())) return 1 + getHeightBranch(node->pLeft, value);
//     else return 1 + getHeightBranch(node->pRight, value);
// }

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

void clearALVTree(Block* node) {
    if (node == nullptr) return;
    clearALVTree(node->pLeft);
    clearALVTree(node->pRight);
    delete node;
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
    scriptV3.push_back({-1, node, -1, "", StepTypeV3::TRAVERSE});
    std::cout << "node: " << fakenode -> value << std::endl;
    if(value < std::stoi(node -> getLabel())){
        insertScriptV3(node -> pLeft, fakenode -> left, value, -1, node, fakenode);
        if(node -> pLeft == nullptr){
            newNode = new Block(CIRCLE, 30.f, std::to_string(value));
            scriptV3.push_back({-1, newNode, -1, "", StepTypeV3::NEW_NODE, node});
            scriptV3.push_back({-1, node, -1, "", StepTypeV3::INSERT});
            fakenode -> left = new cloneNode(newNode, value, 0, 0);
            fakenode -> left -> height = 1;
            newNode = nullptr;
        }
    } 
    else if(value >= std::stoi(node -> getLabel())) {
        insertScriptV3(node -> pRight, fakenode -> right, value, 1, node, fakenode);
        if(node -> pRight == nullptr){
            newNode = new Block(CIRCLE, 30.f, std::to_string(value));
            scriptV3.push_back({-1, newNode, 1, "", StepTypeV3::NEW_NODE, node});
            scriptV3.push_back({-1, node, 1, "", StepTypeV3::INSERT});
            fakenode -> right = new cloneNode(newNode, value, 0, 0);
            fakenode -> right -> height = 1;
            newNode = nullptr;
        }
    }
    fakenode -> height = 1;
    if(fakenode -> left) fakenode -> height = std::max(fakenode -> height, fakenode -> left -> height + 1);
    if(fakenode -> right) fakenode -> height = std::max(fakenode -> height, fakenode -> right -> height + 1);

    RotationType RoTa = checkRotationType(fakenode);
    scriptV3.push_back({-1, node, -1, "", StepTypeV3::TRAVERSE});
    if(RoTa == RotationType::LEFT_LEFT) {
        scriptV3.push_back({-1, node, 1, "", StepTypeV3::TRAVERSE});    
        scriptV3.push_back({-1, node, direction, "", StepTypeV3::ROTATE_LEFT_LEFT, prenode});   
        scriptV3.push_back({-1, fakenode -> left -> realNode, 2, "", StepTypeV3::TRAVERSE});     
        calcRotateLLcase(fakenode, prefakenode, direction);
    }
    else if(RoTa == RotationType::RIGHT_RIGHT) {
        scriptV3.push_back({-1, node, 1, "", StepTypeV3::TRAVERSE});    
        scriptV3.push_back({-1, node, direction, "", StepTypeV3::ROTATE_RIGHT_RIGHT, prenode});    
        scriptV3.push_back({-1, fakenode -> right -> realNode, 2, "", StepTypeV3::TRAVERSE});    
        calcRotateRRcase(fakenode, prefakenode, direction);
    }
    else if(RoTa == RotationType::LEFT_RIGHT) {
        scriptV3.push_back({-1, node, 1, "", StepTypeV3::TRAVERSE});    
        scriptV3.push_back({-1, node -> pLeft, -1, "", StepTypeV3::ROTATE_RIGHT_RIGHT, node});        
        scriptV3.push_back({-1, node, direction, "", StepTypeV3::ROTATE_LEFT_LEFT, prenode});        
        scriptV3.push_back({-1, fakenode -> left -> right -> realNode, 2, "", StepTypeV3::TRAVERSE});
        calcRotateRRcase(fakenode -> left, fakenode, -1);
        calcRotateLLcase(fakenode, prefakenode, direction);
    }
    else if(RoTa == RotationType::RIGHT_LEFT) {
        scriptV3.push_back({-1, node, 1, "", StepTypeV3::TRAVERSE});    
        scriptV3.push_back({-1, node -> pRight, 1, "", StepTypeV3::ROTATE_LEFT_LEFT, node});
        scriptV3.push_back({-1, node, direction, "", StepTypeV3::ROTATE_RIGHT_RIGHT, prenode});        
        scriptV3.push_back({-1, fakenode -> right -> left -> realNode, 2, "", StepTypeV3::TRAVERSE});
        calcRotateLLcase(fakenode -> right, fakenode, 1);
        calcRotateRRcase(fakenode, prefakenode, direction);
    }
}

cloneNode* findMaximumV3(cloneNode* fakenode) {
    if(fakenode -> right == nullptr) return targetDeleteNodeV3 = fakenode -> realNode, fakenode;
    return findMaximumV3(fakenode -> right);
}

cloneNode* deleteScriptV3(Block* node, cloneNode* fakenode, int value, int direction, cloneNode* prefakenode, Block* deletenode = nullptr) {
    if(fakenode == nullptr) return nullptr;
    std::cout << "&&&&&&&&&&&&&&" << fakenode -> value << " " << value << "\n";
    scriptV3.push_back({-1, node, -1, "", StepTypeV3::TRAVERSE});
    if(value == std::stoi(node -> getLabel()) && (targetDeleteNodeV3 == nullptr || targetDeleteNodeV3 == node)) {
        if(deletenode == nullptr)   scriptV3.push_back({-1, node, 3, "", StepTypeV3::TRAVERSE});
        else                        scriptV3.push_back({-1, node, 4, "", StepTypeV3::TRAVERSE});    
        if(fakenode -> left == nullptr || fakenode -> right == nullptr) {
            cloneNode *temp = fakenode -> left ? fakenode -> left : fakenode -> right;
            if(temp == nullptr){
                targetDeleteNodeV3 = nullptr;
                scriptV3.push_back({-1, node, direction, "", StepTypeV3::DELETE, (prefakenode ? prefakenode -> realNode : nullptr), deletenode});
                delete fakenode;
                fakenode = nullptr;
            }
            else {
                targetDeleteNodeV3 = nullptr;
                scriptV3.push_back({-1, node, direction, "", StepTypeV3::DELETE, (prefakenode ? prefakenode -> realNode : nullptr), deletenode});
                *fakenode = *temp;
                delete temp;
            }
        }
        else{
            cloneNode* temp = findMaximumV3(fakenode -> left);
            fakenode -> value = temp -> value;
            std::cout << value << " ?\n"; 
            fakenode -> left = deleteScriptV3(node -> pLeft, fakenode -> left, temp -> value, -1, fakenode, node); 
        }
    }
    else if(value < std::stoi(node -> getLabel()))  fakenode -> left = deleteScriptV3(node -> pLeft, fakenode -> left, value, -1, fakenode, deletenode);
    else                                            fakenode -> right = deleteScriptV3(node -> pRight, fakenode -> right, value, 1, fakenode, deletenode);

    if(fakenode == nullptr) return nullptr;
    fakenode -> height = getHeight(fakenode);

    std::cout << "check rotation type: \n";
    RotationType RoTa = checkRotationType(fakenode);
    scriptV3.push_back({-1, node, -1, "", StepTypeV3::TRAVERSE});
    if(RoTa == RotationType::LEFT_LEFT) {
        cloneNode* temp = fakenode -> left;
        scriptV3.push_back({-1, fakenode -> realNode, 1, "", StepTypeV3::TRAVERSE});    
        scriptV3.push_back({-1, fakenode -> realNode, direction, "", StepTypeV3::ROTATE_LEFT_LEFT, (prefakenode ? prefakenode -> realNode : nullptr)});   
        scriptV3.push_back({-1, fakenode -> left -> realNode, 2, "", StepTypeV3::TRAVERSE});     
        calcRotateLLcase(fakenode, prefakenode, direction);
        return temp;
    }
    else if(RoTa == RotationType::RIGHT_RIGHT) {
        cloneNode* temp = fakenode -> right;
        scriptV3.push_back({-1, fakenode -> realNode, 1, "", StepTypeV3::TRAVERSE});    
        scriptV3.push_back({-1, fakenode -> realNode, direction, "", StepTypeV3::ROTATE_RIGHT_RIGHT, (prefakenode ? prefakenode -> realNode : nullptr)});    
        scriptV3.push_back({-1, fakenode -> right -> realNode, 2, "", StepTypeV3::TRAVERSE});    
        calcRotateRRcase(fakenode, prefakenode, direction);
        return temp;
    }
    else if(RoTa == RotationType::LEFT_RIGHT) {
        cloneNode* temp = fakenode -> left -> right;
        scriptV3.push_back({-1, fakenode -> realNode, 1, "", StepTypeV3::TRAVERSE});    
        scriptV3.push_back({-1, fakenode -> realNode -> pLeft, -1, "", StepTypeV3::ROTATE_RIGHT_RIGHT, fakenode -> realNode});        
        scriptV3.push_back({-1, fakenode -> realNode, direction, "", StepTypeV3::ROTATE_LEFT_LEFT, (prefakenode ? prefakenode -> realNode : nullptr)});        
        scriptV3.push_back({-1, fakenode -> left -> right -> realNode, 2, "", StepTypeV3::TRAVERSE});
        calcRotateRRcase(fakenode -> left, fakenode, -1);
        calcRotateLLcase(fakenode, prefakenode, direction);
        return temp;
    }
    else if(RoTa == RotationType::RIGHT_LEFT) {
        cloneNode* temp = fakenode -> right -> left;
        scriptV3.push_back({-1, fakenode -> realNode, 1, "", StepTypeV3::TRAVERSE});    
        scriptV3.push_back({-1, fakenode -> realNode -> pRight, 1, "", StepTypeV3::ROTATE_LEFT_LEFT, fakenode -> realNode});
        scriptV3.push_back({-1, fakenode -> realNode, direction, "", StepTypeV3::ROTATE_RIGHT_RIGHT, (prefakenode ? prefakenode -> realNode : nullptr)});        
        scriptV3.push_back({-1, fakenode -> right -> left -> realNode, 2, "", StepTypeV3::TRAVERSE});
        calcRotateLLcase(fakenode -> right, fakenode, 1);
        calcRotateRRcase(fakenode, prefakenode, direction);
        return temp;
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
    ImGui::TextColored(ImVec4(0, 255, 0, 255),"Initialize a AVL tree:");
    ImGui::Spacing();

    static char inputBuffer[256] = "";
    bool temp = false;

    ImGui::Text("Enter the values to initialize: A[] = "); 
    ImGui::SameLine(); 
    ImGui::SetNextItemWidth(-1.0f); 
    ImGui::InputTextWithHint("##array_input", "Example: 1 2 3 4 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();
    ImGui::SameLine(540.f);
    if (checkFinishedV3() && ImGui::Button("Random", ImVec2(100.0f, 30)))
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
    if (checkFinishedV3() && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {

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

            for(auto value:newElements){
                std::cout << value << std::endl;
                fakerootV3 = initV3(fakerootV3, rootV3, nullptr, value, 0);    
                rootV3 = fakerootV3 -> realNode;
            }
            calculateAllPos(rootV3, getHeight(rootV3), 1, WINDOW_WIDTH / 2, 300.f);
            scriptV3.push_back({-1, rootV3, -1, "", StepTypeV3::FINISH});
        }
    }

    ImGui::Spacing();
    ImGui::Text("Clear the AVL tree:");
    ImGui::SameLine();
    if (checkFinishedV3() && ImGui::Button("Clear", ImVec2(100.0f, 30))) {
        scriptV3.clear();
        currentStepIdxV3 = 0;
        clearALVTree(rootV3);
        clearFakeNode(fakerootV3);
        rootV3 = nullptr;
        fakerootV3 = nullptr;
    }
}

void insertVisualization3(sf::RenderWindow& window){
    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Insert an element into the AVL tree:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the value to insert:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##insert_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::SameLine(); 



    if(!temp && checkFinishedV3() && ImGui::Button("Random", ImVec2(100.0f, 30))){
        int value = rand() % 500;
        std::string data = std::to_string(value);
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if(checkFinishedV3() && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))){
        std::string data(inputBuffer);

        if(data != ""){
            int value = std::stoi(data);
            scriptV3.clear();
            currentStepIdxV3 = 0;

            setColorALVTree(rootV3, sf::Color::White);
            clearFakeNode(fakerootV3);
            fakerootV3 = cloneStructure(rootV3);
            
            if(rootV3 != nullptr) 
                std::cout << "." << std::endl,
                insertScriptV3(rootV3, fakerootV3, value, 0, nullptr);
            else{
                std::cout << ".." << std::endl,
                newNode = new Block(CIRCLE, 30.f, std::to_string(value));
                scriptV3.push_back({-1, newNode, value, "Đã tạo xong AVL Tree!", StepTypeV3::NEW_NODE});
                scriptV3.push_back({-1, nullptr, 0, "", StepTypeV3::INSERT});
                newNode = nullptr;
            }            
            scriptV3.push_back({-1, rootV3, value, "Đã tạo xong AVL Tree!", StepTypeV3::FINISH});
            
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
}



void searchVisualization3(sf::RenderWindow& window){
    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Search an element in the AVL tree:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the value to search:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##update_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::SameLine();

    if(checkFinishedV3() && ImGui::Button("Random", ImVec2(100.0f, 30))) {

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
    if (checkFinishedV3() && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)) && rootV3 != nullptr)) {
        std::string valueStr(inputBuffer);
        int value;
        if(valueStr != ""){
            value = std::stoi(valueStr);
            scriptV3.clear();
            currentStepIdxV3 = 0;
            setColorALVTree(rootV3, sf::Color::White);
            Block* node = rootV3;
            bool found = false;
            while(node){
                scriptV3.push_back({-1, node, -1, "Đã tạo xong AVL Tree!", StepTypeV3::TRAVERSE});
                if(std::stoi(node -> getLabel()) == value){
                    found = true;
                    break;
                }
                if(std::stoi(node -> getLabel()) > value) node = node -> pLeft;
                else node = node -> pRight;
            }
            if(found)   scriptV3.push_back({-1, node, 1, "Đã tạo xong AVL Tree!", StepTypeV3::SEARCH});
            else        scriptV3.push_back({-1, nullptr, -1, "Đã tạo xong AVL Tree!", StepTypeV3::SEARCH});
            scriptV3.push_back({-1, rootV3, value, "Đã tạo xong AVL Tree!", StepTypeV3::FINISH});
        }
    }
} 
void deleteVisualization3(sf::RenderWindow& window){
    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Delete an element in AVL tree:");
    ImGui::Spacing();
    
    static char valueBuffer[256] = "";
    bool temp = false;

    ImGui::Text("Enter the value to delete:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##delete_input", "Example: 5", valueBuffer, IM_ARRAYSIZE(valueBuffer));
    ImGui::SameLine();

    if(checkFinishedV3() && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        int n = rand() % 100;
        std::string data = std::to_string(n);
        strcpy(valueBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if(checkFinishedV3() && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        std::string data(valueBuffer);
        if(data == "") return;

        int value = std::stoi(data);
        scriptV3.clear();
        currentStepIdxV3 = 0;
        
        setColorALVTree(rootV3, sf::Color::White);
        clearFakeNode(fakerootV3);
        fakerootV3 = cloneStructure(rootV3);

        fakerootV3 = deleteScriptV3(rootV3, fakerootV3, value, 0, nullptr);
        scriptV3.push_back({-1, nullptr, -1, "Deleted!", StepTypeV3::FINISH});


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


bool DFScheckMove(Block* node) {
    if(!node) return false;
    if(checkMove(node)) return true;
    return DFScheckMove(node -> pLeft) || DFScheckMove(node -> pRight);
}

bool checkNextStepV3(float limitTime) {
    isWaitingV3 = true;
    std::cout << "checkNextStepV3 1\n";
    if (newNode && checkMove(newNode)) isWaitingV3 = false;

    std::cout << "checkNextStepV3 2\n";
    if(DFScheckMove(rootV3)) isWaitingV3 = false;
    std::cout << "checkNextStepV3 3\n";

    if(isWaitingV3){ 
        delayTimerV3 += dealtaTime.asSeconds(); 
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
    node -> move(dtV3);
    node -> draw(window);
    node -> height = 1;
    int heightL = (node -> pLeft) ? node -> pLeft -> height : 0;
    int heightR = (node -> pRight) ? node -> pRight -> height : 0;
    if(node -> pLeft){
            sf::Vector2f direction = node -> pLeft -> center() - node -> center();
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if(length > eps)
                drawArrow(window, node -> center() + direction / length * node -> getRadius(), node -> pLeft -> center() - direction / length * node -> pLeft -> getRadius());
    }
    if(node -> pRight){
            sf::Vector2f direction = node -> pRight -> center() - node -> center();
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if(length > eps)
                drawArrow(window, node -> center() + direction / length * node -> getRadius(), node -> pRight -> center() - direction / length * node -> pRight -> getRadius());
    }
    node -> height = std::max(heightL, heightR) + 1;
    int balance = heightL - heightR;
    Text textBF(std::to_string(balance), style6);
    if(balance > 1 || balance < -1) textBF.setFillColor(sf::Color(191, 62, 255));
    textBF.setPosition(node -> center() - sf::Vector2f(0, 55));
    Text textH(std::to_string(node -> height), style6);
    textH.setFillColor(sf::Color(0, 139, 139));
    textH.setPosition(node -> center() + sf::Vector2f(36, 36));
    textBF.draw(window);
    textH.draw(window);
}
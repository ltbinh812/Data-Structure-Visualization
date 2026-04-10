#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <math.h>
#include "visualization2.h"
#include "performVisualization2.h"
#include "entity.h"
#include "draw.h"
#include "theme.h"

float dtV2 = 1.0f;
std::vector<Block*> heapList;
bool isWaitingV2 = false;
float delayTimerV2 = 0;
bool isMaxHeap = true;
int root;
std::vector<Block*> garbageV2;

void initStatus2() {
    // visualization2.cpp
    dtV2 = 1.0f;
    for(auto node:heapList) {
        delete node;
    }
    heapList.clear();
    isWaitingV2 = false;
    delayTimerV2 = 0;
    isMaxHeap = true;
    for(auto &node:garbageV2) delete node;
    garbageV2.clear();

    // performVisualization2.cpp
    currentStepIdxV2 = 0;
    scriptV2.clear();
    scriptV2.push_back({{}, -1, -1, "", StepTypeV2::FINISH});
    root = -1;
    isCalculatingHistoryV2 = false;
    for(auto& clone:dummySetV2)
        delete clone;
    dummySetV2.clear();
    liveToDummyMapV2.clear();
    historyV2.clear();
    historyV2.push_back(new cloneVisualization2(newNode, scriptV2[0]));

    // draw.cpp
    Log = nullptr;
    delayLog = 0;
    o = INITIALIZE;
    isStepByStep = false;
    choosePrevNextButton = 0;
    if(newNode) {
        delete newNode;
        newNode = nullptr;
    }
    firstTime = true;

    // main.cpp
    resetRectangleMinMax();
}

sf::Vector2f calculatePos(int i, int n) {
    // 1. Xác định vị trí logic
    int level = static_cast<int>(std::log2(i + 1));
    int nodesInLevel = 1 << level;
    int posInLevel = i - (nodesInLevel - 1);

    // 2. Xác định tầng sâu nhất hiện tại (H - 1)
    int maxLevel = static_cast<int>(std::log2(n)); 

    // 3. Cấu hình khoảng cách tối thiểu ở tầng đáy
    float minGap = 80.f;   // Khoảng cách nhỏ nhất giữa 2 node hàng xóm ở tầng cuối
    float vGap = 150.f;    // Khoảng cách đứng giữa các tầng

    // 4. Công thức "Nở ngược": Tầng càng thấp (level nhỏ), gap càng to
    float gapAtLevel = minGap * std::pow(2, maxLevel - level);

    // 5. Tính X dựa trên tâm 0
    float midPoint = (nodesInLevel - 1) / 2.0f;
    float x = WINDOW_WIDTH / 2.0f + (posInLevel - midPoint) * gapAtLevel;
    float y = 300.f + level * vGap;

    return { x, y };
}


void siftDownScriptV2(std::vector<Block*>& heapList, int i, int n) {
    root = i;    
    int swapIdx = i;
    int child = 2 * i + 1;
    if(child < n){

        scriptV2.push_back({{30},  i, -1, "So sánh cha và con trái", StepTypeV2::COMPARE, child }); scriptV2.back().setWhiteNodeIdx = child;
        if (isMaxHeap && std::stoi(heapList[swapIdx]->getLabel()) < std::stoi(heapList[child]->getLabel())) {
            scriptV2.push_back({{31}, swapIdx, -1, "Sifting down", StepTypeV2::HIGHLIGHT_1, child }); 
            swapIdx = child;
        }
        if(!isMaxHeap && std::stoi(heapList[swapIdx]->getLabel()) > std::stoi(heapList[child]->getLabel())) {
            scriptV2.push_back({{31}, swapIdx, -1, "Sifting down", StepTypeV2::HIGHLIGHT_1, child }); 
            swapIdx = child;
        }
    }
    if (child + 1 < n) {
        scriptV2.push_back({ {32}, swapIdx, -1, "So sánh với con phải", StepTypeV2::COMPARE, child + 1 }); scriptV2.back().setWhiteNodeIdx = child + 1;
        if (isMaxHeap && std::stoi(heapList[swapIdx]->getLabel()) < std::stoi(heapList[child + 1]->getLabel())) {
            scriptV2.push_back({ {33}, swapIdx, -1, "So sánh với con phải", StepTypeV2::HIGHLIGHT_1, child + 1 });
            swapIdx = child + 1;
        }
        if(!isMaxHeap && std::stoi(heapList[swapIdx]->getLabel()) > std::stoi(heapList[child + 1]->getLabel())) {
            scriptV2.push_back({ {33}, swapIdx, -1, "So sánh với con phải", StepTypeV2::HIGHLIGHT_1, child + 1 });
            swapIdx = child + 1;
        }
    }

    if(swapIdx == i){
        scriptV2.push_back({ {38}, swapIdx, -1, "So sánh với con phải", StepTypeV2::HIGHLIGHT_1, child + 1 });        
        return;
    }
    scriptV2.push_back({ {36, 37}, i, -1, "Đổi chỗ cha con (Max Heap)", StepTypeV2::SWAP_DOWN, swapIdx }); scriptV2.back().setWhiteNodeIdx = i;
    std::swap(heapList[i], heapList[swapIdx]); // Tráo con trỏ trong mảng logic
    siftDownScriptV2(heapList, swapIdx, n);
}

void buildMaxHeapScriptV2(std::vector<Block*>& heapList) {
    int n = heapList.size();
    root = -1;
    for (int i = n / 2 - 1; i >= 0; i--) {
        scriptV2.push_back({ {44}, i, -1, "Sifting down", StepTypeV2::TRAVERSE, root});
        scriptV2.push_back({ {45, 23}, i, -1, "Sifting down", StepTypeV2::HIGHLIGHT_1, root});
        siftDownScriptV2(heapList, i, n);
    }
}

void heapifyScriptV2(std::vector<Block*>& heapList, int i) {
    if(i == 0) return;
    root = i;
    int parent = (i - 1) / 2;
    scriptV2.push_back({{16}, i, -1, "COMPARE", StepTypeV2::COMPARE, parent}); scriptV2.back().setWhiteNodeIdx = parent;
    if(isMaxHeap && std::stoi(heapList[parent]->getLabel()) < std::stoi(heapList[i]->getLabel())) {
        std::swap(heapList[parent], heapList[i]);
        scriptV2.push_back({{17}, i, -1, "Sifting up", StepTypeV2::SWAP_UP, parent}); scriptV2.back().setWhiteNodeIdx = i;
        scriptV2.push_back({{18}, i, -1, "Sifting up", StepTypeV2::HIGHLIGHT_1, parent}); 
        heapifyScriptV2(heapList, parent);
    }
    if(!isMaxHeap && std::stoi(heapList[parent]->getLabel()) > std::stoi(heapList[i]->getLabel())) {
        std::swap(heapList[parent], heapList[i]);
        scriptV2.push_back({{17}, i, -1, "Sifting up", StepTypeV2::SWAP_UP, parent}); scriptV2.back().setWhiteNodeIdx = i;
        scriptV2.push_back({{18}, i, -1, "Sifting up", StepTypeV2::HIGHLIGHT_1, parent}); 
        heapifyScriptV2(heapList, parent);
    }
}
void insertScriptV2(std::vector<Block*>& heapList, int value) {
    root = -1;
    heapList.push_back(new Block(CIRCLE, 30.f, std::to_string(value)));
    scriptV2.push_back({{{58,13}}, (int)heapList.size() - 1, -1, "Đã tạo xong node!", StepTypeV2::TRAVERSE});
    heapifyScriptV2(heapList, heapList.size() - 1);
}

// Heap
void initVisualization2(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color,"Initialize a heap array:");
    ImGui::Spacing();
    ImGui::Text("Choose the type of heap:");
    ImGui::SameLine();
    bool temp = false;
    static char inputBuffer[256] = ""; 
    // --- NÚT MAXX HEAP ---
    bool pushedMax = false;
    if (isMaxHeap) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.13f, 0.55f, 0.31f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.65f, 0.38f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.23f, 0.75f, 0.45f, 1.0f));
        pushedMax = true; 
    }
    if (ImGui::Button("Max Heap", ImVec2(120, 40)) && !isMaxHeap && (isStepByStep || checkFinishedV2())) {
        isMaxHeap = true;
        std::string data = "";
        for(auto node:heapList) {
            data += node->getLabel();
            data += " ";
        }
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    if (pushedMax) ImGui::PopStyleColor(3); 
    ImGui::SameLine();
    // --- NÚT MIN HEAP ---
    bool pushedMin = false;
    if (!isMaxHeap) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.4f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.5f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.6f, 0.4f, 1.0f));
        pushedMin = true;
    }
    if (ImGui::Button("Min Heap", ImVec2(120, 40)) && isMaxHeap && (isStepByStep || checkFinishedV2())) {
        isMaxHeap = false;
        std::string data = "";
        for(auto node:heapList) {
            data += node->getLabel();
            data += " ";
        }
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    if (pushedMin) ImGui::PopStyleColor(3);
    ImGui::Spacing();

    ImGui::Text("A[] = "); 
    ImGui::SameLine(); 



    ImGui::SetNextItemWidth(-1.0f); 
    ImGui::InputTextWithHint("##array_input", "Example: 1 2 3 4 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();

    ImGui::SameLine(90.0f);
    if (temp == false && (isStepByStep || checkFinishedV2()) && ImGui::Button("Random", ImVec2(100.0f, 30)))
    {
        int n = rand() % 8;
        std::string data = "";
        for(int i = 0; i < n; i++){
            int value = rand() % 100;
            data += std::to_string(value) + " ";
        }
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV2()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {

        std::string data(inputBuffer);
        std::stringstream ss(data);
        int value;
        std::vector<int> newElements;
        while (ss >> value) newElements.push_back(value);

        if (!newElements.empty()) {
            for(auto node:heapList){
                delete node;
            }
            heapList.clear();
            scriptV2.clear();
            currentStepIdxV2 = 0;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            historyV2.clear();
            for(auto& garbage:garbageV2) delete garbage;
            garbageV2.clear();
            isCalculatingHistoryV2 = true;
            firstTime = true;


            std::vector<Block*> tempList;
            scriptV2.push_back({{53, 42}, -1, -1, "Tạo mảng", StepTypeV2::HIGHLIGHT_1});
            for(int i = 0; i < newElements.size(); i++){
                heapList.push_back(new Block(CIRCLE, 30.f, std::to_string(newElements[i])));
                heapList[i] -> currentPosition = calculatePos(i, newElements.size());
                heapList[i] -> targetPosition = heapList[i] -> currentPosition;
                heapList[i] -> setPosition(heapList[i] -> currentPosition);
                tempList.push_back(heapList[i]);
            }            
            // scriptV2.push_back({-1, -1, -1, "Đã tạo xong Heap!", StepTypeV2::INITIALIZE});
            buildMaxHeapScriptV2(tempList);
            scriptV2.push_back({{}, -1, -1, "Đã tạo xong Max Heap!", StepTypeV2::FINISH}); 
        }
    }

    ImGui::Spacing();
    ImGui::Text("Clear the heap: ");
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV2()) && ImGui::Button("Clear", ImVec2(100.0f, 30))) {
        for(auto node:heapList){
            delete node;
        }
        heapList.clear();
        scriptV2.clear();
        scriptV2.push_back({{}, -1, -1, "", StepTypeV2::FINISH});
        currentStepIdxV2 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        historyV2.clear();
        for(auto& garbage:garbageV2) delete garbage;
        garbageV2.clear();
        isCalculatingHistoryV2 = true;
        firstTime = true;
    }
}

void insertVisualization2(sf::RenderWindow& window){
    ImGui::TextColored(title1Color, "Insert an element into the heap:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the value to insert:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##insert_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::SameLine(); 

    if((isStepByStep || checkFinishedV2()) && ImGui::Button("Random", ImVec2(100.0f, 30))){
        int value = rand() % 100;
        std::string data = std::to_string(value);
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if((isStepByStep || checkFinishedV2()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))){
        std::string data(inputBuffer);
        if(data == "") return;
        int value = std::stoi(data);
        scriptV2.clear();
        currentStepIdxV2 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        historyV2.clear();
        for(auto& garbage:garbageV2) delete garbage;
        garbageV2.clear();
        isCalculatingHistoryV2 = true;
        firstTime = true;


        std::vector<Block*> tempList = heapList;

        for(auto node:heapList){
            node -> setFillColor(sf::Color::White);
        }

        scriptV2.push_back({{57}, (int)heapList.size(), value, "Đã tạo xong Heap!", StepTypeV2::NEW_NODE});
        scriptV2.push_back({{57}, (int)heapList.size(), -1, "Sifting up", StepTypeV2::INSERT});
        insertScriptV2(tempList, value);
        scriptV2.push_back({{19}, -1, -1, "Đã tạo xong Heap!", StepTypeV2::HIGHLIGHT_1});
        scriptV2.push_back({{}, -1, -1, "Đã tạo xong Heap!", StepTypeV2::FINISH});
    }
}


void updateVisualization2(sf::RenderWindow& window){
    ImGui::TextColored(title1Color, "Update an element in the heap:");
    ImGui::Spacing();
    static char posBuffer[256], valueBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the position to update:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##update_input", "Example: 5", posBuffer, IM_ARRAYSIZE(posBuffer));
    ImGui::SameLine();

    ImGui::Text("The new value:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##new_value_input", "Example: 10", valueBuffer, IM_ARRAYSIZE(valueBuffer));
    ImGui::Spacing();

    if((isStepByStep || checkFinishedV2()) && ImGui::Button("Random", ImVec2(100.0f, 30)) && heapList.size() > 0) {
        int n = rand() % heapList.size();
        std::string data = std::to_string(n);
        strcpy(posBuffer, data.c_str());
        int value = rand() % 100;
        data = std::to_string(value);
        strcpy(valueBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV2()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        int pos, value;
        std::string posStr(posBuffer);
        std::string valueStr(valueBuffer);

        if(posStr != "" && valueStr != ""){
            if(std::stoi(posStr) >= heapList.size()){
                if(!Log){
                    setLog("Indexed out of range");
                    return;
                }
            }
            pos = std::stoi(posStr);
            value = std::stoi(valueStr);
            for(auto node:heapList) node->setFillColor(sf::Color::White);
            scriptV2.clear();
            currentStepIdxV2 = 0;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            historyV2.clear();
            for(auto& garbage:garbageV2) delete garbage;
            garbageV2.clear();
            isCalculatingHistoryV2 = true;
            firstTime = true;


            std::vector<Block*> tempList;
            for(auto node:heapList){
                tempList.push_back(new Block(*node));
            }

            tempList[pos] -> initText(std::to_string(value), style4);
            scriptV2.push_back({{63}, pos, -1, "Đang duyệt tới vị trí " + std::to_string(pos), StepTypeV2::TRAVERSE});
            scriptV2.push_back({{64}, pos, value, "Update", StepTypeV2::UPDATE});
            if(pos > 0){
                if(isMaxHeap){
                    if(std::stoi(tempList[(pos - 1) / 2]->getLabel()) < std::stoi(tempList[pos]->getLabel())){
                        scriptV2.push_back({{66, 13}, pos, -1, "compare", StepTypeV2::COMPARE, (pos - 1)/2}); scriptV2.back().setWhiteNodeIdx = (pos - 1)/2;
                        heapifyScriptV2(tempList, pos);
                    }
                    else{
                        scriptV2.push_back({{66}, pos, -1, "compare", StepTypeV2::COMPARE, (pos - 1)/2}); scriptV2.back().setWhiteNodeIdx = (pos - 1)/2;
                        scriptV2.push_back({{67, 23}, -1, -1, "", StepTypeV2::HIGHLIGHT_1});
                        siftDownScriptV2(tempList, pos, tempList.size());
                    }
                }
                else{
                    if(std::stoi(tempList[(pos - 1) / 2]->getLabel()) > std::stoi(tempList[pos]->getLabel())){
                        scriptV2.push_back({{66, 13}, pos, -1, "compare", StepTypeV2::COMPARE, (pos - 1)/2}); scriptV2.back().setWhiteNodeIdx = (pos - 1)/2;
                        heapifyScriptV2(tempList, pos);
                    }
                    else{
                        scriptV2.push_back({{66}, pos, -1, "compare", StepTypeV2::COMPARE, (pos - 1)/2}); scriptV2.back().setWhiteNodeIdx = (pos - 1)/2;
                        scriptV2.push_back({{67, 23}, -1, -1, "", StepTypeV2::HIGHLIGHT_1});
                        siftDownScriptV2(tempList, pos, tempList.size());
                    }
                }
            } 
            else
                siftDownScriptV2(tempList, pos, tempList.size());
            scriptV2.push_back({{}, -1, -1, "Đã tạo xong Heap!", StepTypeV2::FINISH}); 
            for(auto node:tempList){
                delete node;
            }
        }
    }
} 
void getTopVisualization2(sf::RenderWindow& window){
    ImGui::TextColored(title1Color, "Get the max element in the heap:");
    ImGui::Spacing();
    if((isStepByStep || checkFinishedV2()) && ImGui::Button("Get", ImVec2(125.0f, 30)) && heapList.size() > 0) {
        scriptV2.clear();
        currentStepIdxV2 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        historyV2.clear();
        for(auto& garbage:garbageV2) delete garbage;
        garbageV2.clear();
        isCalculatingHistoryV2 = true;
        firstTime = true;

        std::vector<Block*> tempList = heapList;

        for(auto node:heapList){
            node -> setFillColor(sf::Color::White);
        }

        if(heapList.empty()){
            scriptV2.push_back({{71}, 0, -1, "Đã tạo xong Heap!", StepTypeV2::HIGHLIGHT_1});
            scriptV2.push_back({{}, -1, -1, "Đã tạo xong Heap!", StepTypeV2::FINISH});
            return;
        }

        scriptV2.push_back({{72}, 0, -1, "Đã tạo xong Heap!", StepTypeV2::TRAVERSE});
        scriptV2.push_back({{73}, 0, -1, "Sifting up", StepTypeV2::MOVE, (int)heapList.size() - 1});
        scriptV2.push_back({{74}, (int)heapList.size() - 1, -1, "Đã tạo xong Heap!", StepTypeV2::DELETE_1, (int)heapList.size() - 1});
        scriptV2.push_back({{74}, (int)heapList.size() - 1, -1, "Đã tạo xong Heap!", StepTypeV2::DELETE_2, (int)heapList.size() - 1});
        if(heapList.size() > 1){
            scriptV2.push_back({{75, 23}, 0, -1, "Sifting up", StepTypeV2::HIGHLIGHT_1});
            std::swap(tempList[0], tempList[tempList.size() - 1]);
            tempList.pop_back();
            siftDownScriptV2(tempList, 0, tempList.size());
        }
        scriptV2.push_back({{}, -1, -1, "Đã tạo xong Heap!", StepTypeV2::FINISH});
    }
}


bool checkNextStepV2(float limitTime, Block* newNode, std::vector<Block*>& heapList) {
    isWaitingV2 = true;
    if (newNode && checkMove(newNode)) isWaitingV2 = false;
    for (Block* node : heapList) {
        if (checkMove(node)) {
            isWaitingV2 = false;
            break;  
        }
    }
    // std::cout << isWaitingV2 << std::endl;
    if(isWaitingV2){ 
        delayTimerV2 += dealtaTime.asSeconds() * dtV2; 
        std::cout << "**********************************" << delayTimerV2 << "\n";
        if (delayTimerV2 >= limitTime) { 
            isWaitingV2 = false;
            delayTimerV2 = 0;
            // std::cout << "return true\n";
            return true; 
        }
    }
    
    return false;
}


void drawHeapList(sf::RenderWindow& window, std::vector<Block*>& heapList) {
    for (int i=0; i<heapList.size(); i++) {
        heapList[i]->move(dtV2, isCalculatingHistoryV2);
        if(!isCalculatingHistoryV2) heapList[i]->draw(window);
        Text text(std::to_string(i), style6);
        text.setPosition(heapList[i]->center() + sf::Vector2f(0, 50));
        if(!isCalculatingHistoryV2) text.draw(window);
    }
    if(!isCalculatingHistoryV2)
        for(int i=0; i<heapList.size(); i++) {
            if(2 * i + 1 < heapList.size()) {
                sf::Vector2f direction = heapList[2 * i + 1] -> center() - heapList[i] -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if(length > eps)
                    drawArrow(window, heapList[i] -> center() + direction / length * heapList[i] -> getRadius(), heapList[2 * i + 1] -> center() - direction / length * heapList[2 * i + 1] -> getRadius());
            }
            if(2 * i + 2 < heapList.size()) {
                sf::Vector2f direction = heapList[2 * i + 2] -> center() - heapList[i] -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if(length > eps)
                    drawArrow(window, heapList[i] -> center() + direction / length * heapList[i] -> getRadius(), heapList[2 * i + 2] -> center() - direction / length * heapList[2 * i + 2] -> getRadius());
            }
        }
}

#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "visualization1.h"
#include "performVisualization1.h"
#include "main.h"
#include "draw.h"
#include "theme.h"

float dtV1 = 1.0f;
std::vector<Block*> linkedList;
bool isWaitingV1 = false;
float delayTimerV1 = 0;


void initStatus1(){
    // visualization1.cpp
    dtV1 = 1.0f;
    for(auto node:linkedList){
        delete node;
    }
    linkedList.clear();
    isWaitingV1 = false;
    delayTimerV1 = 0;

    // performVisualization1.cpp
    scriptV1.clear();
    scriptV1.push_back({{}, -1, -1, "", StepTypeV1::FINISH}); 
    currentStepIdxV1 = 0;
    isCalculatingHistoryV1 = false;
    historyV1.clear();
    historyV1.push_back(new cloneVisualization1(linkedList, newNode));

    // draw.cpp
    Log = nullptr;
    delayLog = 0;
    o = INITIALIZE;
    isStepByStep = false;
    choosePrevNextButton = 0;
    if(newNode){
        delete newNode;
        newNode = nullptr;
    }
    firstTime = true;

    // main.cpp
    resetRectangleMinMax();
}

void initVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color,"Initialize a linked list:");
    ImGui::Spacing();
    bool temp = false;

    ImGui::Text("A[] = "); 
    ImGui::SameLine(); 

    static char inputBuffer[256] = ""; 

    ImGui::SetNextItemWidth(-1.0f); 
    ImGui::InputTextWithHint("##array_input", "Example: 1 2 3 4 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();

    ImGui::SameLine(90.0f);
    if ((checkFinishedV1() || isStepByStep) && ImGui::Button("Random", ImVec2(100.0f, 30)))
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
    if ((checkFinishedV1() || isStepByStep) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {

        std::string data(inputBuffer);
        std::stringstream ss(data);
        int value;
        std::vector<int> newElements;
        while (ss >> value) newElements.push_back(value);

        if (!newElements.empty()) {
            for(auto node:linkedList){
                delete node;
            }
            linkedList.clear();
            scriptV1.clear();
            historyV1.clear();
            currentStepIdxV1 = 0;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }

            scriptV1.push_back({{21}, -1, newElements[0], "", StepTypeV1::NEW_NODE});
            scriptV1.push_back({{22}, 0, -1, "", StepTypeV1::INSERT});
            for (int i = 1; i < newElements.size(); i++) {
                std::cout << newElements[i] << " ";
                scriptV1.push_back({{24}, -1, newElements[i], "", StepTypeV1::NEW_NODE});
                scriptV1.push_back({{25}, i, -1, "", StepTypeV1::INSERT});
            }
            scriptV1.push_back({{}, (int)newElements.size() - 1, -1, "", StepTypeV1::FINISH});
            isCalculatingHistoryV1 = true;
            firstTime = true;
        }
    }
    ImGui::Spacing();
    ImGui::Text("Clear the linked list:");
    ImGui::SameLine();
    if ((checkFinishedV1() || isStepByStep) && ImGui::Button("Clear", ImVec2(100.0f, 30))) {
        for(auto node:linkedList){
            delete node;
        }
        linkedList.clear();
        scriptV1.clear();
        scriptV1.push_back({{}, -1, -1, "", StepTypeV1::FINISH});
        currentStepIdxV1 = 0;
        historyV1.clear();
        isCalculatingHistoryV1 = true;
        firstTime = true;
    }
}

void insertVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Insert an element into the linked list:");
    ImGui::Spacing();

    static char positionBuffer[256] = ""; 
    static char valueBuffer[256] = "";
    static char headValueBuffer[256] = "";
    static char tailValueBuffer[256] = "";
    bool temp = false;

    ImGui::TextColored(title2Color, "Insert a value to the top of the linked list:");
    ImGui::SameLine();  
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##insert_head_input", "Example: 5", headValueBuffer, IM_ARRAYSIZE(headValueBuffer));
    if((checkFinishedV1() || isStepByStep) && ImGui::Button("Random##Head", ImVec2(100.0f, 30))){
        int value = rand() % 100;
        std::string data = std::to_string(value);
        strcpy(headValueBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((checkFinishedV1() || isStepByStep) && (temp || ImGui::Button("Confirm##Head", ImVec2(125.0f, 30)))) {
        temp = false;
        int value = std::stoi(headValueBuffer);
        int pos = 0;
        historyV1.back()->pull(linkedList, newNode);
        for(auto node:linkedList) node->setFillColor(sf::Color::White);
        scriptV1.clear();
        historyV1.clear();
        currentStepIdxV1 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        scriptV1.push_back({{31}, -1, value, "Khởi tạo Node mới với giá trị " + std::to_string(value), StepTypeV1::NEW_NODE});
        scriptV1.push_back({{34, 35}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::INSERT});
        scriptV1.push_back({{}, pos, -1, "Hoàn tất thao tác chèn!", StepTypeV1::FINISH});
        isCalculatingHistoryV1 = true;
        firstTime = true;
    }
    ImGui::Spacing();
    ImGui::TextColored(title2Color, "Insert a value to the end of the linked list:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##insert_tail_input", "Example: 5", tailValueBuffer, IM_ARRAYSIZE(tailValueBuffer));
    if((checkFinishedV1() || isStepByStep) && ImGui::Button("Random##Tail", ImVec2(100.0f, 30))){
        int value = rand() % 100;
        std::string data = std::to_string(value);
        strcpy(tailValueBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((checkFinishedV1() || isStepByStep) && (temp || ImGui::Button("Confirm##Tail", ImVec2(125.0f, 30)))) {
        temp = false;
        int value = std::stoi(tailValueBuffer);
        int pos = linkedList.size();
        historyV1.back()->pull(linkedList, newNode);
        for(auto node:linkedList) node->setFillColor(sf::Color::White);
        scriptV1.clear();
        historyV1.clear();
        currentStepIdxV1 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }

        scriptV1.push_back({{31}, -1, value, "Khởi tạo Node mới với giá trị " + std::to_string(value), StepTypeV1::NEW_NODE});
        for (int i = 0; i < pos; i++){
            scriptV1.push_back({{40}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::TRAVERSE});            
            scriptV1.push_back({{41}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::HIGHLIGHT_2});            
        }
        if(pos > 0){
            scriptV1.push_back({{44,45}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::INSERT});
            scriptV1.push_back({{}, pos, -1, "Hoàn tất thao tác chèn!", StepTypeV1::FINISH});
        }
        else{
            scriptV1.push_back({{34,35}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::INSERT});
            scriptV1.push_back({{}, pos, -1, "Hoàn tất thao tác chèn!", StepTypeV1::FINISH});
        }        
        isCalculatingHistoryV1 = true;
        firstTime = true;
    }   

    ImGui::Spacing();
    ImGui::TextColored(title2Color, "Insert at a specific position:");
    ImGui::Text("Position to insert (0-based index):");
    ImGui::SameLine(); 
    ImGui::SetNextItemWidth(200.0f); 
    ImGui::InputTextWithHint("##insert_input", "Example: 2", positionBuffer, IM_ARRAYSIZE(positionBuffer));
    ImGui::SameLine();
    ImGui::Text("Value:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##value_input", "Example: 5", valueBuffer, IM_ARRAYSIZE(valueBuffer));
    // ImGui::Spacing();

    // ImGui::SameLine(90.0f);
    printf("currentStepIdxV1: %d\n", currentStepIdxV1);
    if ((checkFinishedV1() || isStepByStep) && ImGui::Button("Random##Pos", ImVec2(100.0f, 30))){
        int position = rand() % (linkedList.size() + 1);
        std::string positionStr = std::to_string(position);
        std::string valueStr = std::to_string(rand() % 100);
        strcpy(positionBuffer, positionStr.c_str());
        strcpy(valueBuffer, valueStr.c_str());
        int value = rand() % 1000;
        std::string headValueStr = std::to_string(value);
        std::string tailValueStr = std::to_string(value);
        strcpy(headValueBuffer, headValueStr.c_str());
        strcpy(tailValueBuffer, tailValueStr.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((checkFinishedV1() || isStepByStep) && (temp || ImGui::Button("Confirm##Pos", ImVec2(125.0f, 30)))) {
        temp = false;
        std::string positionStr(positionBuffer);
        std::string valueStr(valueBuffer);
        historyV1.back()->pull(linkedList, newNode);
            
        if (positionStr != "" && valueStr != "") {
            if(std::stoi(positionStr) > linkedList.size()){
                if(!Log){
                    setLog("Indexed out of range");
                    delayLog = 0;
                }
                return;
            }
            int value = std::stoi(valueStr);
            int pos = std::stoi(positionStr);
            

            for(auto node:linkedList) node->setFillColor(sf::Color::White);
            scriptV1.clear();
            historyV1.clear();
            currentStepIdxV1 = 0;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            scriptV1.push_back({{31}, -1, value, "Khởi tạo Node mới với giá trị " + std::to_string(value), StepTypeV1::NEW_NODE});
            for (int i = 0; i < pos; i++){
                scriptV1.push_back({{40}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::TRAVERSE});            
                scriptV1.push_back({{41}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::HIGHLIGHT_2});            
            }
            if(pos > 0){
                scriptV1.push_back({{44,45}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::INSERT});
                scriptV1.push_back({{}, pos, -1, "Hoàn tất thao tác chèn!", StepTypeV1::FINISH});
            }
            else{
                scriptV1.push_back({{34,35}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::INSERT});
                scriptV1.push_back({{}, pos, -1, "Hoàn tất thao tác chèn!", StepTypeV1::FINISH});
            }        
            isCalculatingHistoryV1 = true;
            firstTime = true;
        }
    }   
}


void deleteVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Delete an element from the linked list:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the position to delete:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##delete_input", "Example: 2", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();
    if ((checkFinishedV1() || isStepByStep) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        historyV1.back()->pull(linkedList, newNode);
        if(linkedList.size() == 0) return;
        int n = rand() % linkedList.size();
        std::string data = std::to_string(n);
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((checkFinishedV1() || isStepByStep) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        int pos;
        temp = false;
        std::string data(inputBuffer);
        historyV1.back()->pull(linkedList, newNode);
            
        if (data != "") {
            if(std::stoi(data) >= linkedList.size()){
                if(!Log){
                    setLog("Indexed out of range");
                    return;
                }
            }
            pos = std::stoi(data);
            for(auto node:linkedList) node->setFillColor(sf::Color::White);
            scriptV1.clear();
            historyV1.clear();
            currentStepIdxV1 = 0;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            for(int i=0; i<pos; i++){
                scriptV1.push_back({{59}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::TRAVERSE});
                scriptV1.push_back({{60}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::HIGHLIGHT_2});
            }
            if(pos > 0){
                scriptV1.push_back({{65}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::DELETE_1});
                scriptV1.push_back({{66, 67}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::DELETE_2});
            }
            else{
                scriptV1.push_back({{{52}}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::DELETE_1});
                scriptV1.push_back({{53, 54}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::DELETE_2});                
            }
            scriptV1.push_back({{}, pos - 1, -1, "Hoàn tất thao tác xóa!", StepTypeV1::FINISH});
            isCalculatingHistoryV1 = true;
            firstTime = true;
            firstTime = true;
        }
    }

    static char inputBufferVal[256] = "";

    ImGui::Spacing();
    ImGui::Text("Enter the value to delete:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##delete_input_value", "Example: 2", inputBufferVal, IM_ARRAYSIZE(inputBufferVal));
    ImGui::Spacing();
    if ((checkFinishedV1() || isStepByStep) && ImGui::Button("Random##value", ImVec2(100.0f, 30))) {
        historyV1.back()->pull(linkedList, newNode);
        if(linkedList.size() == 0) return;
        int n = rand() % 100;
        std::string data = std::to_string(n);
        strcpy(inputBufferVal, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((checkFinishedV1() || isStepByStep) && (temp || ImGui::Button("Confirm##value", ImVec2(125.0f, 30)))) {
        int value;
        std::string data(inputBufferVal);
        historyV1.back()->pull(linkedList, newNode);
            
        if (data != "") {
            value = std::stoi(data);
            for(auto node:linkedList) node->setFillColor(sf::Color::White);
            scriptV1.clear();
            historyV1.clear();
            currentStepIdxV1 = 0;
            isCalculatingHistoryV1 = true;
            firstTime = true;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            if(linkedList.empty()){
                scriptV1.push_back({{71}, -1, -1, "Khoâng tìm thấy vị trí cô với giá trị bằng " + std::to_string(value), StepTypeV1::HIGHLIGHT_1});
                scriptV1.push_back({{}, -1, -1, "Khoâng tìm thấy vị trí cô với giá trị bằng " + std::to_string(value), StepTypeV1::FINISH});
                return;
            }
            int pos = -1;
            for(int i=0; i<linkedList.size(); i++) if(linkedList[i]->getLabel() == std::to_string(value)){pos = i; break;}
            if(pos == -1){
                std::cout << "HEREEEEEEEEE\n";
                for(int i = 0; i<linkedList.size(); i++){
                    scriptV1.push_back({{79}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::TRAVERSE});
                    scriptV1.push_back({{80}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::HIGHLIGHT_2});                    
                }
                scriptV1.push_back({{}, - 1, -1, "Hoàn tất thao tác xóa!", StepTypeV1::FINISH});
                return;
            }
            for(int i=0; i<pos; i++){
                scriptV1.push_back({{79}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::TRAVERSE});
                scriptV1.push_back({{80}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::HIGHLIGHT_2});
            }
            if(pos > 0){
                scriptV1.push_back({{83}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::DELETE_1});
                scriptV1.push_back({{84,85}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::DELETE_2});
            }
            else{
                scriptV1.push_back({{{72}}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::DELETE_1});
                scriptV1.push_back({{73,74,75}, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::DELETE_2});                
            }
            scriptV1.push_back({{}, pos - 1, -1, "Hoàn tất thao tác xóa!", StepTypeV1::FINISH});
        }
    }

}

void updateVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Update an element in the linked list:");
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

    if((checkFinishedV1() || isStepByStep) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        historyV1.back()->pull(linkedList, newNode);
        if(linkedList.size() == 0) return;
        int n = rand() % linkedList.size();
        std::string data = std::to_string(n);
        strcpy(posBuffer, data.c_str());
        int value = rand() % 100;
        data = std::to_string(value);
        strcpy(valueBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((checkFinishedV1() || isStepByStep) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        int pos, value;
        std::string posStr(posBuffer);
        std::string valueStr(valueBuffer);
        historyV1.back()->pull(linkedList, newNode);
            
        if(posStr != "" && valueStr != ""){
            if(std::stoi(posStr) >= linkedList.size()){
                if(!Log){
                    setLog("Indexed out of range");
                    return;
                }
            }
            pos = std::stoi(posStr);
            value = std::stoi(valueStr);
            for(auto node:linkedList) node->setFillColor(sf::Color::White);
            scriptV1.clear();
            historyV1.clear();
            currentStepIdxV1 = 0;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            for(int i=0; i<pos; i++){
                scriptV1.push_back({{91}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::TRAVERSE});
                scriptV1.push_back({{96}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::HIGHLIGHT_2});
            }
            scriptV1.push_back({{91}, pos, -1, "Đang duyệt tới vị trí " + std::to_string(pos), StepTypeV1::TRAVERSE});
            scriptV1.push_back({{92}, pos, -1, "Đang duyệt tới vị trí " + std::to_string(pos), StepTypeV1::HIGHLIGHT_2});
        
            scriptV1.push_back({{93}, pos, value, "Thực hiện thay đổi liên kết (Next pointer)", StepTypeV1::UPDATE});
            scriptV1.push_back({{}, pos, -1, "Hoàn tất thao tác cập nhật!", StepTypeV1::FINISH});
            isCalculatingHistoryV1 = true;
            firstTime = true;
        }
    }
}

void searchVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Search for an element in the linked list:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the value to search (example: 5):");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##search_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();


    if((checkFinishedV1() || isStepByStep) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        historyV1.back()->pull(linkedList, newNode);
        if(linkedList.size() == 0) return;
        std::string data = linkedList[rand() % linkedList.size()]->getLabel();
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((checkFinishedV1() || isStepByStep) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        std::string data(inputBuffer);

        if (data != "") {
            historyV1.back()->pull(linkedList, newNode);
            for(int i=0; i<linkedList.size(); i++) linkedList[i]->setFillColor(sf::Color::White);
            scriptV1.clear();
            historyV1.clear();
            currentStepIdxV1 = 0;
            if(newNode){
                delete newNode;
                newNode = nullptr;
            }
            int i = 0;
            for(i=0; i<linkedList.size(); i++){
                scriptV1.push_back({{102}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::TRAVERSE});
                if(linkedList[i]->getLabel() == data) break;
                scriptV1.push_back({{104}, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepTypeV1::HIGHLIGHT_2});
            }
            if(i < linkedList.size()){
                scriptV1.push_back({{103}, i, std::stoi(data), "Tìm thấy lớp hướng với bội giải thức", StepTypeV1::SEARCH_1});
                scriptV1.push_back({{}, i, -1, "Hoàn tất thao tác tìm kiếm!", StepTypeV1::FINISH});
            }
            else{
                scriptV1.push_back({{106}, -1, -1, "Không tìm thấy lớp hướng với bội giải thức", StepTypeV1::SEARCH_2});
                scriptV1.push_back({{}, -1, -1, "Hoàn tất thao tác tìm kiếm!", StepTypeV1::FINISH});
            }
            isCalculatingHistoryV1 = true;
            firstTime = true;
        }
    }
}



bool checkNextStepV1(float limitTime) {
    isWaitingV1 = true;
    if (newNode && checkMove(newNode)) isWaitingV1 = false;
    // std::cout << isWaitingV1 << std::endl;
    for (Block* node : linkedList) {
        if (checkMove(node)) {
            isWaitingV1 = false;
            break;  
        }
    }
    // std::cout << isWaitingV1 << std::endl;
    if(isWaitingV1){ 
        delayTimerV1 += dealtaTime.asSeconds() * dtV1; 
        std::cout << "**********************************" << delayTimerV1 << "\n";
        if (delayTimerV1 >= limitTime) { 
            isWaitingV1 = false;
            delayTimerV1 = 0;
            // std::cout << "return true\n";
            return true; 
        }
    }
    
    return false;
}


void drawLinkedList(sf::RenderWindow& window) {
    std::cout << "check1\n";
    for (int i=0; i<linkedList.size(); i++) {
        linkedList[i]->move(dtV1, isCalculatingHistoryV1);
        if(!isCalculatingHistoryV1) linkedList[i]->draw(window);

        if( i < linkedList.size() - 1) {
                sf::Vector2f direction = linkedList[i + 1] -> center() - linkedList[i] -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if(length > eps)
                    if(!isCalculatingHistoryV1) drawArrow(window, linkedList[i] -> center() + direction / length * linkedList[i] -> getRadius(), linkedList[i + 1] -> center() - direction / length * linkedList[i + 1] -> getRadius(), edgeColor);

        }
    }

    if(newNode){
        newNode->move(dtV1, isCalculatingHistoryV1);
        if(!isCalculatingHistoryV1) newNode->draw(window);
    }

    if(linkedList.size() > 0) if(!isCalculatingHistoryV1) drawPointer(window, linkedList[0] -> center() + sf::Vector2f(0.f, +50.f), "Head/0");
    if(linkedList.size() > 1) if(!isCalculatingHistoryV1) drawPointer(window, linkedList[linkedList.size() - 1] -> center() + sf::Vector2f(0.f, +50.f), "Tail/" + std::to_string(linkedList.size() - 1));
    if(scriptV1.size() && checkFinishedV1() && scriptV1[currentStepIdxV1].focusNodeIdx != -1) if(!isCalculatingHistoryV1) drawPointer(window, linkedList[scriptV1[currentStepIdxV1].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(scriptV1[currentStepIdxV1].focusNodeIdx));
    
}



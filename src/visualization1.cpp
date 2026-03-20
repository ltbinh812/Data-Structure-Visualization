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


float dt = 1.0f;

std::vector<Block*> linkedList;
bool isWaiting = false;
float delayTimer = 0;

void initVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(ImVec4(0, 255, 0, 255),"Initialize a linked list:");
    ImGui::Spacing();
    bool temp = false;

    ImGui::Text("A[] = "); 
    ImGui::SameLine(); 


    static char inputBuffer[256] = ""; 

    ImGui::SetNextItemWidth(-1.0f); 
    ImGui::InputTextWithHint("##array_input", "Example: 1 2 3 4 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();

    ImGui::SameLine(90.0f);
    if (currentStepIdx + 1 == script.size() && ImGui::Button("Random", ImVec2(100.0f, 30)))
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
    if (currentStepIdx + 1 == script.size() && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {

        std::string data(inputBuffer);
        std::stringstream ss(data);
        int value;
        std::vector<int> newElements;
        while (ss >> value) newElements.push_back(value);

        if (!newElements.empty()) {
            linkedList.clear();
            script.clear();
            currentStepIdx = 0;

            for (int i = 0; i < newElements.size(); i++) {
                std::cout << newElements[i] << " ";
                script.push_back({1, -1, newElements[i], "", StepType::NEW_NODE});
                script.push_back({2, i, -1, "", StepType::INSERT});
            }
            script.push_back({3, (int)newElements.size() - 1, -1, "", StepType::FINISH});
        }
    }
}

void insertVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Insert an element into the linked list:");
    ImGui::Spacing();

    static char positionBuffer[256] = ""; 
    static char valueBuffer[256] = "";
    static char headValueBuffer[256] = "";
    static char tailValueBuffer[256] = "";
    bool temp = false;

    ImGui::TextColored(ImVec4(255, 255, 0, 255), "Insert a value to the top of the linked list:");
    ImGui::SameLine();  
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##insert_head_input", "Example: 5", headValueBuffer, IM_ARRAYSIZE(headValueBuffer));
    if(currentStepIdx + 1 == script.size() && ImGui::Button("Random##Head", ImVec2(100.0f, 30))){
        int value = rand() % 100;
        std::string data = std::to_string(value);
        strcpy(headValueBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if (currentStepIdx + 1 == script.size() && (temp || ImGui::Button("Confirm##Head", ImVec2(125.0f, 30)))) {
        int value = std::stoi(headValueBuffer);
        int pos = 0;
        for(auto node:linkedList) node->setFillColor(sf::Color::White);
        script.clear();
        currentStepIdx = 0;
        script.push_back({1, -1, value, "Khởi tạo Node mới với giá trị " + std::to_string(value), StepType::NEW_NODE});
        script.push_back({3, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepType::INSERT});
        script.push_back({4, pos, -1, "Hoàn tất thao tác chèn!", StepType::FINISH});
    }
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(255, 255, 0, 255), "Insert a value to the end of the linked list:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##insert_tail_input", "Example: 5", tailValueBuffer, IM_ARRAYSIZE(tailValueBuffer));
    if(currentStepIdx + 1 == script.size() && ImGui::Button("Random##Tail", ImVec2(100.0f, 30))){
        int value = rand() % 100;
        std::string data = std::to_string(value);
        strcpy(tailValueBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if (currentStepIdx + 1 == script.size() && (temp || ImGui::Button("Confirm##Tail", ImVec2(125.0f, 30)))) {
        int value = std::stoi(tailValueBuffer);
        int pos = linkedList.size();
        for(auto node:linkedList) node->setFillColor(sf::Color::White);
        script.clear();
        currentStepIdx = 0;
        script.push_back({1, -1, value, "Khởi tạo Node mới với giá trị " + std::to_string(value), StepType::NEW_NODE});
        for (int i = 0; i < pos; i++)  script.push_back({2, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepType::TRAVERSE});            
        script.push_back({3, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepType::INSERT});
        script.push_back({4, pos, -1, "Hoàn tất thao tác chèn!", StepType::FINISH});

    }   

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(255, 255, 0, 255), "Insert at a specific position:");
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
    printf("currentStepIdx: %d\n", currentStepIdx);
    if (currentStepIdx + 1 == script.size() && ImGui::Button("Random##Pos", ImVec2(100.0f, 30))){
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
    if (currentStepIdx + 1 == script.size() && (temp || ImGui::Button("Confirm##Pos", ImVec2(125.0f, 30)))) {
        printf("Insert\n");
        std::string positionStr(positionBuffer);
        std::string valueStr(valueBuffer);
        
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
            script.clear();
            currentStepIdx = 0;
            script.push_back({1, -1, value, "Khởi tạo Node mới với giá trị " + std::to_string(value), StepType::NEW_NODE});
            for (int i = 0; i < pos; i++)  script.push_back({2, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepType::TRAVERSE});            
            script.push_back({3, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepType::INSERT});
            script.push_back({4, pos, -1, "Hoàn tất thao tác chèn!", StepType::FINISH});
        }
    }   
}


void deleteVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Delete an element from the linked list:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the position to delete:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##delete_input", "Example: 2", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();
    if (currentStepIdx + 1 == script.size() && ImGui::Button("Random", ImVec2(100.0f, 30)) && linkedList.size() > 0) {
        int n = rand() % linkedList.size();
        std::string data = std::to_string(n);
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if (currentStepIdx + 1 == script.size() && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        int pos;
        std::string data(inputBuffer);
    
        if (data != "") {
            if(std::stoi(data) >= linkedList.size()){
                if(!Log){
                    setLog("Indexed out of range");
                    return;
                }
            }

            pos = std::stoi(data);
            for(auto node:linkedList) node->setFillColor(sf::Color::White);
            script.clear();
            currentStepIdx = 0;
            for(int i=0; i<pos; i++) script.push_back({2, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepType::TRAVERSE});
            script.push_back({3, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepType::DELETE_1});
            script.push_back({3, pos, -1, "Thực hiện thay đổi liên kết (Next pointer)", StepType::DELETE_2});
            script.push_back({4, pos - 1, -1, "Hoàn tất thao tác xóa!", StepType::FINISH});
        }
    }

}

void updateVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Update an element in the linked list:");
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

    if(currentStepIdx + 1 == script.size() && ImGui::Button("Random", ImVec2(100.0f, 30)) && linkedList.size() > 0) {
        int n = rand() % linkedList.size();
        std::string data = std::to_string(n);
        strcpy(posBuffer, data.c_str());
        int value = rand() % 100;
        data = std::to_string(value);
        strcpy(valueBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if (currentStepIdx + 1 == script.size() && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        int pos, value;
        std::string posStr(posBuffer);
        std::string valueStr(valueBuffer);

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
            script.clear();
            currentStepIdx = 0;
            
            for(int i=0; i<=pos; i++) script.push_back({2, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepType::TRAVERSE});
            script.push_back({3, pos, value, "Thực hiện thay đổi liên kết (Next pointer)", StepType::UPDATE});
            script.push_back({4, pos, -1, "Hoàn tất thao tác cập nhật!", StepType::FINISH});
        }
    }
}

void searchVisualization1(sf::RenderWindow& window) {
    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Search for an element in the linked list:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the value to search (example: 5):");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##search_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();


    if(currentStepIdx + 1 == script.size() && ImGui::Button("Random", ImVec2(100.0f, 30)) && linkedList.size() > 0) {
        int value = rand() % 100;
        std::string data = std::to_string(value);
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if (currentStepIdx + 1 == script.size() && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        std::string data(inputBuffer);

        if (data != "") {
            for(int i=0; i<linkedList.size(); i++) linkedList[i]->setFillColor(sf::Color::White);
            script.clear();
            currentStepIdx = 0;
            int i = 0;
            for(i=0; i<linkedList.size(); i++){
                script.push_back({2, i, -1, "Đang duyệt tới vị trí " + std::to_string(i), StepType::TRAVERSE});
                if(linkedList[i]->getLabel() == data) break;
            }
            if(i < linkedList.size()){
                script.push_back({3, i, std::stoi(data), "Tìm thấy lớp hướng với bội giải thức", StepType::SEARCH_1});
                script.push_back({5, i, -1, "Hoàn tất thao tác tìm kiếm!", StepType::FINISH});
            }
            else{
                script.push_back({4, -1, -1, "Không tìm thấy lớp hướng với bội giải thức", StepType::SEARCH_2});
                script.push_back({5, -1, -1, "Hoàn tất thao tác tìm kiếm!", StepType::FINISH});
            }
        }
    }
}


bool checkMove(Block *Node){
    sf::Vector2f direction = Node -> targetPosition - Node -> currentPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if(distance > 0.0001f) return true;
    return false;
}

bool checkNextStep(float limitTime) {
    isWaiting = true;
    if (newNode && checkMove(newNode)) isWaiting = false;
    // std::cout << isWaiting << std::endl;
    for (Block* node : linkedList) {
        if (checkMove(node)) {
            isWaiting = false;
            break;  
        }
    }
    // std::cout << isWaiting << std::endl;
    if(isWaiting){ 
        delayTimer += dealtaTime.asSeconds() * dt; 
        std::cout << "**********************************" << delayTimer << "\n";
        if (delayTimer >= limitTime) { 
            isWaiting = false;
            delayTimer = 0;
            // std::cout << "return true\n";
            return true; 
        }
    }
    
    return false;
}


void drawList(sf::RenderWindow& window) {
    for (int i=0; i<linkedList.size(); i++) {
        linkedList[i]->move(dt);
        linkedList[i]->draw(window);

        if( i < linkedList.size() - 1) {
                sf::Vector2f direction = linkedList[i + 1] -> center() - linkedList[i] -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if(length > eps)
                    drawArrow(window, linkedList[i] -> center() + direction / length * linkedList[i] -> getRadius(), linkedList[i + 1] -> center() - direction / length * linkedList[i + 1] -> getRadius());

        }
    }
    if(linkedList.size() > 0) drawPointer(window, linkedList[0] -> center() + sf::Vector2f(0.f, +50.f), "Head/0");
    if(linkedList.size() > 1) drawPointer(window, linkedList[linkedList.size() - 1] -> center() + sf::Vector2f(0.f, +50.f), "Tail/" + std::to_string(linkedList.size() - 1));

}


void drawLog(sf::RenderWindow& window) {
    std::cout << "???\n";
    if(Log){
        std::cout << "???" << delayLog << std::endl;
        if(checkMove(Log)){
            Log->move(8.0f);
            delayLog = 0;
        }
        else{
            delayLog += dealtaTime.asSeconds() * dt;
            if(delayLog >= 2.0f){
                delete Log;
                Log = nullptr;
                delayLog = 0;
                return;
            }
        }
        Log->draw(window);
    }
}
#include <math.h>
#include "vector"
#include "visualization4.h"
#include "performVisualization4.h"
#include "main.h"
#include "entity.h"
#include "draw.h"
#include "theme.h"

bool isWaitingV4 = false;
float delayTimerV4 = 0;
float dtV4 = 1.0f;
Block* rootV4 = nullptr;
std::vector<Block*> garbageV4;


void resetTrie(Block* node) {
    if(node == nullptr) return;
    for(auto& pair : node -> children){
        resetTrie(pair.second);
    }
    delete node;
}

void initStatus4(){
    // visualization4.cpp
    isWaitingV4 = false;
    delayTimerV4 = 0;
    dtV4 = 1.0f;

    resetTrie(rootV4);
    rootV4 = new Block(ShapeType::CIRCLE, 30.f, "root");
    rootV4 -> targetPosition = sf::Vector2f(WINDOW_WIDTH / 2, 300.f);
    rootV4 -> currentPosition = rootV4 -> targetPosition;
    rootV4 -> setPosition(rootV4 -> currentPosition);
    for(auto& node: garbageV4) delete node;
    garbageV4.clear();

    // performVisualization4.cpp
    scriptV4.clear();
    scriptV4.push_back({{}, nullptr, -1, "", StepTypeV4::FINISH});
    currentStepIdxV4 = 0;
    isCalculatingHistoryV4 = false;
    for(auto& clone:dummySetV4)
        delete clone;
    dummySetV4.clear();
    liveToDummyMapV4.clear();
    historyV4.clear();
    historyV4.push_back(new cloneVisualization4(rootV4, newNode, scriptV4[0]));

    // draw.cpp
    o = INITIALIZE;
    Log = nullptr;
    delayLog = 0;
    choosePrevNextButton = 0;
    isStepByStep = false;
    if(newNode){
        delete newNode;
        newNode = nullptr;
    }
    firstTime = true;

    // main.cpp
    resetRectangleMinMax();

}

float calculateTrieGapWidth(Block* node) {
    if (node == nullptr) return 0.f;
    if(node->children.empty()) return node -> gapWidth = 80.f;
    node -> gapWidth = 0.f;
    for (auto& pair : node->children) {
        Block* child = pair.second;
        node->gapWidth += calculateTrieGapWidth(child);
    }
    return node->gapWidth;
}
void calculateTriePos(Block* node, float x, float y) {
    if (node == nullptr) return;
    node->targetPosition = sf::Vector2f(x, y);
    std::cout << node -> getLabel() << " " << x << " " << y << std::endl;
    float vGap = 150.f;
    float currentX = x - (node->gapWidth / 2.f);
    for (auto& pair : node->children) {
        Block* child = pair.second;
        float childTargetX = currentX + (child->gapWidth / 2.f);
        calculateTriePos(child, childTargetX, y + vGap);
        currentX += child->gapWidth;
    }
}

void setColorTrie(Block* node, sf::Color color = sf::Color::White) {
    if (node == nullptr) return;
    node->setFillColor(color);
    for (auto& pair : node->children) {
        Block* child = pair.second;
        setColorTrie(child, color);
    }
}

void initVisualization4(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Initializing the Trie tree!");
    ImGui::Spacing();
    ImGui::Text("Enter a list of strings to insert into the Trie tree, separated by newline:");
    ImGui::Spacing();
    static char inputBuffer[1024] = "";
    bool temp = false;
    ImGui::SetNextItemWidth(400.0f);
    ImGui::InputTextMultiline("##init_input", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4), ImGuiInputTextFlags_AllowTabInput);
    ImGui::Spacing();

    if ((isStepByStep || checkFinishedV4()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        int test = rand() % 6 + 1;
        std::string data = "";
        while(test--){
            int n = rand() % 6 + 1;
            for(int i = 0; i < n; i++){
                int value = rand() % 26 + 'a';
                data += char(value);
            }
            data += "\n";
        }
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV4()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        std::string data(inputBuffer);
        if(data == "") return;
        std::vector<std::string> inputStrings;
        std::stringstream ss(data);
        std::string line;
        while (std::getline(ss, line)) {
            inputStrings.push_back(line);
        }
        scriptV4.clear();
        currentStepIdxV4 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        historyV4.clear();
        for(auto& node: garbageV4) delete node;
        garbageV4.clear();

        resetTrie(rootV4);
        rootV4 = new Block(ShapeType::CIRCLE, 30.f, "root");
        rootV4 -> targetPosition = sf::Vector2f(WINDOW_WIDTH / 2, 300.f);
        rootV4 -> currentPosition = rootV4 -> targetPosition;
        rootV4 -> setPosition(rootV4 -> currentPosition);

        for(auto s:inputStrings){
            Block* node = rootV4;
            for (int i = 0; i < s.length(); i++) {
                char c = s[i];
                if (node->children.find(c) == node->children.end()) {
                    Block* newNode = new Block(CIRCLE, 30.f, std::string(1, c));
                    newNode -> value = 1;
                    if(i + 1 == s.length()) newNode -> height = 1;
                    node->children[c] = newNode;
                    node = newNode;
                } else {
                    node = node->children[c];
                    node -> value++;
                    if(i + 1 == s.length()) node -> height++;
                }
            }
        }
        calculateTrieGapWidth(rootV4);
        calculateTriePos(rootV4, WINDOW_WIDTH / 2, 300.f);
        setColorTrie(rootV4);
        scriptV4.push_back({{26,27,28,29}, nullptr, -1, "", StepTypeV4::HIGHLIGHT_1});
        scriptV4.push_back({{}, nullptr, -1, "", StepTypeV4::FINISH});
        isCalculatingHistoryV4 = true;
        firstTime = true;
        drawTrie(rootV4, window, rootV4);
    }
    ImGui::Spacing();
    ImGui::Text("Clear the Trie tree:");
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV4()) && ImGui::Button("Clear", ImVec2(100.0f, 30))){
        scriptV4.clear();
        scriptV4.push_back({{22,23,24}, nullptr, -1, "", StepTypeV4::HIGHLIGHT_1});
        scriptV4.push_back({{}, nullptr, -1, "", StepTypeV4::FINISH});
        currentStepIdxV4 = 0;
        resetTrie(rootV4);
        rootV4 = new Block(ShapeType::CIRCLE, 30.f, "root");
        rootV4 -> targetPosition = sf::Vector2f(WINDOW_WIDTH / 2, 300.f);
        rootV4 -> currentPosition = rootV4 -> targetPosition;
        rootV4 -> setPosition(rootV4 -> currentPosition);
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        for(auto& node: garbageV4) delete node;
        garbageV4.clear();
        historyV4.clear();
        isCalculatingHistoryV4 = true;
        firstTime = true;
    }
}

void insertVisualization4(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Insert an element in the Trie tree:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the string to insert:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##insert_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::SameLine();
    if((isStepByStep || checkFinishedV4()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        int n = rand() % 8;
        std::string data = "";
        for(int i = 0; i < n; i++){
            int value = rand() % 26 + 'a';
            data += std::to_string(value);
        }
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if((isStepByStep || checkFinishedV4()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        std::string data(inputBuffer);
        if(data == "") return;
        scriptV4.clear();
        currentStepIdxV4 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        historyV4.clear();
        for(auto& node: garbageV4){
            std::cout << node << "\n";
            std::cout << "delete garbage: " << node -> getLabel() << "\n";
            delete node;
        }
        garbageV4.clear();
        setColorTrie(rootV4);
        Block* node = rootV4;
        scriptV4.push_back({{31, 32}, node, 0, "", StepTypeV4::TRAVERSE});
        for (int i = 0; i < data.length(); i++) {
            char c = data[i];
            scriptV4.push_back({{33}, node, 0, "", StepTypeV4::TRAVERSE});

            if (node->children.find(c) == node->children.end()) {
                Block* newNode = new Block(CIRCLE, 30.f, std::string(1, c));
                scriptV4.push_back({{34}, node, 0, "", StepTypeV4::TRAVERSE});
                scriptV4.push_back({{35}, newNode, -1, "", StepTypeV4::NEW_NODE, node});
                scriptV4.push_back({{35}, newNode, -1, "", StepTypeV4::INSERT, node});
                if(i + 1 == data.length()) scriptV4.back().isFinished = true;
                node = newNode;
                scriptV4.push_back({{37,38}, node, 0, "", StepTypeV4::TRAVERSE});
            } else {
                node = node->children[c];
                scriptV4.push_back({{37,38}, node, 1, "", StepTypeV4::TRAVERSE});
                if(i + 1 == data.length()) scriptV4.back().isFinished = true;
            }
        }
        scriptV4.push_back({{40}, node, 0, "", StepTypeV4::TRAVERSE});
        scriptV4.push_back({{}, nullptr, -1, "", StepTypeV4::FINISH});
        isCalculatingHistoryV4 = true;
        firstTime = true;
    }
}

void deleteVisualization4(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Delete an element in the Trie:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the string to delete:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##delete_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::SameLine();
    if((isStepByStep || checkFinishedV4()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        int n = rand() % 8;
        std::string data = "";
        for(int i = 0; i < n; i++){
            int value = rand() % 26 + 'a';
            data += std::to_string(value);
        }
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if((isStepByStep || checkFinishedV4()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        std::string data(inputBuffer);
        if(data == "") return;
        scriptV4.clear();
        currentStepIdxV4 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        historyV4.clear();
        for(auto& node: garbageV4) delete node;
        garbageV4.clear();
        isCalculatingHistoryV4 = true;
        firstTime = true;
        setColorTrie(rootV4);
        
        Block* node = rootV4;
        bool check_found = true;
        scriptV4.push_back({{52}, node, 0, "", StepTypeV4::TRAVERSE});

        for (int i = 0; i < data.length(); i++) {
            char c = data[i];
            if (node->children.find(c) == node->children.end()) {
                check_found = false;
                break;
            } else {
                node = node->children[c];
                scriptV4.push_back({{53, 43}, node, 0, "", StepTypeV4::TRAVERSE});
            }
        }

        if(check_found == false || node -> height <= 0) {
            scriptV4.push_back({{53}, nullptr, -1, "Not found!", StepTypeV4::FINISH});
            return;
        }
        node = rootV4;
        scriptV4.push_back({{55}, node, 0, "", StepTypeV4::TRAVERSE});
        for (int i = 0; i < data.length(); i++) {
            char c = data[i];
            
            if(node -> value > 1 || node == rootV4){
                scriptV4.push_back({{56}, nullptr, -1, "", StepTypeV4::HIGHLIGHT_1});
                if(node -> children[c] -> value == 1){
                    scriptV4.push_back({{58, 60,61, 14}, node -> children[c], 0, "", StepTypeV4::TRAVERSE});
                    scriptV4.push_back({{17}, node -> children[c], -1, "", StepTypeV4::DELETE, node});
                    if(i + 1 < data.length())
                        scriptV4.push_back({{18,14}, node -> children[c] -> children[data[i + 1]], -1, "", StepTypeV4::HIGHLIGHT_2});
                    else
                        scriptV4.push_back({{18,14}, nullptr, -1, "", StepTypeV4::HIGHLIGHT_1});
                }
                else
                    scriptV4.push_back({{58}, node -> children[c], -1, "", StepTypeV4::DELETE, node});
                if(i + 1 == data.length())
                    scriptV4.back().isFinished = true;
                if(node -> children[c] -> value > 1)
                    scriptV4.push_back({{65}, nullptr, -1, "", StepTypeV4::HIGHLIGHT_1});
            }
            else{
                scriptV4.push_back({{17}, node -> children[c], -1, "", StepTypeV4::DELETE});
                if(i + 1 < data.length())
                    scriptV4.push_back({{18,14}, node -> children[c] -> children[data[i + 1]], -1, "", StepTypeV4::HIGHLIGHT_2});
                else
                    scriptV4.push_back({{18,14}, nullptr, -1, "", StepTypeV4::HIGHLIGHT_1});
                if(i + 1 == data.length()){
                    scriptV4.push_back({{15}, nullptr, -1, "", StepTypeV4::HIGHLIGHT_1});
                    scriptV4.back().isFinished = true;
                }    
            }                
            node = node->children[c];
        }
        scriptV4.push_back({{67}, nullptr, -1, "", StepTypeV4::HIGHLIGHT_1});
        scriptV4.push_back({{}, nullptr, -1, "", StepTypeV4::FINISH});
    }
}

void searchVisualization4(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Search for an element in the Trie:");
    ImGui::Spacing();
    static char inputBuffer[256] = "";
    bool temp = false;
    ImGui::Text("Enter the string to search:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##search_input", "Example: 5", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::SameLine();
    if((isStepByStep || checkFinishedV4()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        int n = rand() % 8;
        std::string data = "";
        for(int i = 0; i < n; i++){
            int value = rand() % 26 + 'a';
            data += std::to_string(value);
        }
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if((isStepByStep || checkFinishedV4()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        std::string data(inputBuffer);
        if(data == "") return;
        scriptV4.clear();
        currentStepIdxV4 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        historyV4.clear();
        for(auto& node: garbageV4) delete node;
        garbageV4.clear();
        isCalculatingHistoryV4 = true;
        firstTime = true;
        setColorTrie(rootV4);
        
        Block* node = rootV4;
        bool check_found = true;
        scriptV4.push_back({{43,44}, node, 0, "", StepTypeV4::TRAVERSE});
        for(int i = 0; i < data.length(); i++){
            scriptV4.push_back({{45}, node, 0, "", StepTypeV4::TRAVERSE});
            char c = data[i];
            if(node -> children.find(c) == node -> children.end()){
                check_found = false;
                break;
            }
            else{
                node = node -> children[c];
                scriptV4.push_back({{47}, node, 0, "", StepTypeV4::TRAVERSE});
            }
        }
        if(check_found && node -> height > 0) scriptV4.push_back({{49}, node, 1, "Found!", StepTypeV4::SEARCH});
        else if(check_found) scriptV4.push_back({{49}, nullptr, -1, "Not found!", StepTypeV4::SEARCH});
        else scriptV4.push_back({{46}, nullptr, -1, "Not found!", StepTypeV4::SEARCH});
        scriptV4.push_back({{}, nullptr, -1, "", StepTypeV4::FINISH});
    }
}

bool DFScheckMoveV4(Block* node) {
    if(!node) return false;
    if(checkMove(node)) return true;
    return DFScheckMoveV4(node -> pLeft) || DFScheckMoveV4(node -> pRight);
}

bool checkNextStepV4(float dt, Block* rootV4, Block* newNode) {
    isWaitingV4 = true;
    std::cout << "bool newNode: " << (newNode != nullptr) << "\n";
    std::cout << "bool rootV4: " << (rootV4 != nullptr) << "\n";
    if (newNode && checkMove(newNode)) isWaitingV4 = false;
    
    if(DFScheckMoveV4(rootV4)) isWaitingV4 = false;
    
    if(isWaitingV4){ 
        delayTimerV4 += dealtaTime.asSeconds() * dtV4; 
        std::cout << "**********************************" << delayTimerV4 << "\n";
        if (delayTimerV4 >= dt) { 
            isWaitingV4 = false;
            delayTimerV4 = 0;
            return true; 
        }
    }
    
    return false;
}


void drawTrie(Block* node, sf::RenderWindow& window, Block* cloneRootV4) {
    if (node == nullptr) return;
    node -> move(dtV4, isCalculatingHistoryV4);
    if(!isCalculatingHistoryV4) node -> draw(window);

    if(node != cloneRootV4){
        Text text1(std::to_string(node -> value), style6);
        text1.setPosition(node -> center() + sf::Vector2f(30, -39));
        if(!isCalculatingHistoryV4) text1.draw(window);
        if(node -> height > 0){
            Text text2(std::to_string(node -> height), style6);
            text2.setFillColor(sf::Color(191, 62, 255));
            text2.setPosition(node -> center() + sf::Vector2f(-30, 39));
            if(!isCalculatingHistoryV4) text2.draw(window);
        }
    }

    for (auto& pair : node->children) {
        Block* child = pair.second;
        drawTrie(child, window, cloneRootV4);
        
        sf::Vector2f direction = child -> center() - node -> center();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if(length > eps) if(!isCalculatingHistoryV4) drawArrow(window, node -> center() + direction / length * node -> getRadius(), child -> center() - direction / length * child -> getRadius(), edgeColor);
    }

}
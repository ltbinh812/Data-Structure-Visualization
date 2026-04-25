#include <math.h>
#include <set>
#include <vector>
#include <queue>
#include <climits>
#include "vector"
#include "visualization5.h"
#include "performVisualization5.h"
#include "main.h"
#include "entity.h"
#include "draw.h"
#include "theme.h"
#include "graphPhysics.h"
#include "importFile.h"

bool isWaitingV5 = false;
float delayTimerV5 = 0;
float dtV5 = 1.0f;

std::vector<std::vector<std::pair<int,int>>> adjListV5; 
std::vector<int> distV5;
std::vector<std::pair<int,int>> prevV5;
std::vector<bool> visitedV5;

void initStatus5(){
    // visualization5.cpp
    isWaitingV5 = false;
    delayTimerV5 = 0;
    dtV5 = 1.0f;
    adjListV5.clear();
    distV5.clear();
    prevV5.clear();
    visitedV5.clear();
    graphPhysics.clearNodes();

    
    // performVisualization5.cpp
    scriptV5.clear();
    scriptV5.push_back({{}, -1, -1, "", StepTypeV5::FINISH});
    currentStepIdxV5 = 0;
    isCalculatingHistoryV5 = false;
    for(auto &clone:historyV5) delete clone; historyV5.clear();
    historyV5.push_back(new cloneVisualization5(distV5, prevV5, visitedV5));

    // draw.cpp
    o = INITIALIZE;
    showImLog = false;
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
    graphPhysics.getNodes().clear();
}

void initVisualization5(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Initializing the graph!");
    ImGui::Spacing();
    

    static char inputBuffer[15000] = "";
    bool temp = false;

    
    ImGui::SetNextItemWidth(400.0f);
    ImVec2 inputSize = ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4);    
    ImVec2 inputPos = ImGui::GetCursorScreenPos(); 

    ImGui::InputTextMultiline("##init_input", inputBuffer, IM_ARRAYSIZE(inputBuffer), inputSize, ImGuiInputTextFlags_AllowTabInput);
    if (inputBuffer[0] == '\0' && !ImGui::IsItemActive()) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 textPos = ImVec2(inputPos.x + 8.0f, inputPos.y + 8.0f); 
        float lineHeight = ImGui::GetTextLineHeight();
        drawList->AddText(textPos, IM_COL32(150, 150, 150, 150), "- Line 1: n m (n: vertices, m: edges)");
        textPos.y += lineHeight;         
        drawList->AddText(textPos, IM_COL32(150, 150, 150, 150), "- Next m lines: u v w (Edge from u to v, weight w)");
        textPos.y += lineHeight;         
        drawList->AddText(textPos, IM_COL32(230, 100, 100, 150), "*Note: Vertices are 0-indexed (0 to n-1). Undirected graph.");
    }
    
    ImGui::Spacing();

    if ((isStepByStep || checkFinishedV5()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        int n = rand() % 5 + 4; 
        int max_edges = n * (n - 1) / 2;
        int m = rand() % (max_edges / 2 + 1) + n - 1; 
        if (m > max_edges) m = max_edges;
        std::string data = std::to_string(n) + " " + std::to_string(m) + "\n";
        std::set<std::pair<int, int>> edges;
        while (edges.size() < m) {
            int u = rand() % n;
            int v = rand() % n;
            if (u != v) {
                if (u > v) std::swap(u, v); 
                edges.insert({u, v});
            }
        }
        for (auto& edge : edges) {
            int w = rand() % 20 + 1; 
            data += std::to_string(edge.first) + " " + std::to_string(edge.second) + " " + std::to_string(w) + "\n";
        }

        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV5()) && ImGui::Button("Load File", ImVec2(150.0f, 30))) {
        std::string fileContent = openAndReadFile();
        if (!fileContent.empty()) {
            strncpy(inputBuffer, fileContent.c_str(), sizeof(inputBuffer) - 1);
            inputBuffer[sizeof(inputBuffer) - 1] = '\0'; 
            temp = true; 
        }
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV5()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
        temp = false;
        std::string data(inputBuffer);
        if(data == "") return;
        
        std::stringstream ss(data);
        std::string line;
        
        if (!std::getline(ss, line)) return;
        if (!line.empty() && line.back() == '\r') line.pop_back(); 

        std::stringstream lineStream(line);
        std::string tokenN, tokenM, extraNM;
        if (!(lineStream >> tokenN >> tokenM) || (lineStream >> extraNM)) {
            setLog("Invalid input!"); return; 
        }

        int n = 0, m = 0;
        try {
            size_t pN, pM;
            n = std::stoi(tokenN, &pN);
            m = std::stoi(tokenM, &pM);
            if (pN != tokenN.length() || pM != tokenM.length() || n <= 0 || n > 999 || m < 0) {
                setLog("Invalid input!"); return;
            }
        } catch (...) { setLog("Invalid input!"); return; }

        std::map<std::pair<int, int>, int> edgeMap;
        int edgeCount = 0;
        bool isValid = true;

        while (std::getline(ss, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue; 
            
            std::stringstream es(line);
            std::string tokenU, tokenV, tokenW, extraEdge;
            // Ép buộc chỉ có đúng 3 phần tử trên 1 dòng
            if (!(es >> tokenU >> tokenV >> tokenW) || (es >> extraEdge)) { 
                isValid = false; break; 
            }
            
            int u = 0, v = 0, w = 0;
            try {
                size_t pU, pV, pW;
                u = std::stoi(tokenU, &pU);
                v = std::stoi(tokenV, &pV);
                w = std::stoi(tokenW, &pW);
                if (pU != tokenU.length() || pV != tokenV.length() || pW != tokenW.length() || 
                    u < 0 || u >= n || v < 0 || v >= n || w < 0 || w > 999) {
                    isValid = false; break;
                }
            } catch (...) { isValid = false; break; }

            edgeCount++; 
            if (u == v) continue; 

            int minNode = std::min(u, v);
            int maxNode = std::max(u, v);
            std::pair<int, int> currentEdge = {minNode, maxNode};

            if (edgeMap.find(currentEdge) == edgeMap.end()) {
                edgeMap[currentEdge] = w;
            } else {
                edgeMap[currentEdge] = std::min(edgeMap[currentEdge], w);
            }
        }

        if (!isValid || edgeCount < m) {
            setLog("Invalid input!"); return;
        }


        std::vector<std::vector<std::pair<int, int>>> tempAdj(n);
        for (auto const& [edge, weight] : edgeMap) {
            int u = edge.first;
            int v = edge.second;
            tempAdj[u].push_back({v, weight});
            tempAdj[v].push_back({u, weight});
        }


        scriptV5.clear();
        currentStepIdxV5 = 0;
        
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        
        for(auto &clone:historyV5) delete clone; historyV5.clear();
        isCalculatingHistoryV5 = true;
        firstTime = true;
        temp = false; 

        adjListV5 = tempAdj;
        distV5.assign(n, INT_MAX);
        prevV5.assign(n, {-1, -1});
        visitedV5.assign(n, false);
        graphPhysics.clearNodes();


        for(int i = 0; i < n; i++){
            Block* node = new Block(ShapeType::CIRCLE, 30.f, std::to_string(i));
            graphPhysics.addNode(node);
        }
        
        scriptV5.push_back({{12, 13, 14, 15}, -1, -1, "", StepTypeV5::HIGHLIGHT_1});
        scriptV5.push_back({{}, -1, -1, "", StepTypeV5::FINISH});

    }
    ImGui::Spacing();    
    ImGui::Text("Add an edge:");
    ImGui::SameLine();
    static char inputEdgeBuffer[15000] = "";
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##random_edge", "Example: 5", inputEdgeBuffer, IM_ARRAYSIZE(inputEdgeBuffer));
    ImGui::SameLine();

    if ((isStepByStep || checkFinishedV5()) && ImGui::Button("Random##Edge", ImVec2(100.0f, 30))){
        int n = distV5.size();
        if(n == 0) return;
        std::string data = std::to_string(rand() % n) + " " + std::to_string(rand() % n) + " " + std::to_string(rand() % 100);
        strcpy(inputEdgeBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV5()) && (temp || ImGui::Button("Add", ImVec2(100.0f, 30)))){
        temp = false;
        std::string data(inputEdgeBuffer);
        std::stringstream ss(data);
        std::string tokenU, tokenV, tokenW, extra;
        
        if (!(ss >> tokenU >> tokenV >> tokenW) || (ss >> extra)) {
            setLog("Invalid input!"); return;
        }

        int u = 0, v = 0, w = 0, n = distV5.size();
        try {
            size_t pU, pV, pW;
            u = std::stoi(tokenU, &pU);
            v = std::stoi(tokenV, &pV);
            w = std::stoi(tokenW, &pW);
            
            if (pU != tokenU.length() || pV != tokenV.length() || pW != tokenW.length() || 
                u < 0 || u >= n || v < 0 || v >= n || w < 0 || w > 999 || u == v) {
                setLog("Invalid input!"); return;
            }
        } catch (...) { setLog("Invalid input!"); return; }

        scriptV5.clear();
        scriptV5.push_back({{}, -1, -1, "", StepTypeV5::FINISH});
        currentStepIdxV5 = 0;        
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }        
        for(auto &clone:historyV5) delete clone; historyV5.clear();
        isCalculatingHistoryV5 = true;
        firstTime = true;
        int found_idx = -1;
        for(int i = 0; i < adjListV5[u].size(); i++) if(adjListV5[u][i].first == v) found_idx = i;
        if(found_idx != -1) adjListV5[u][found_idx].second = std::min(adjListV5[u][found_idx].second, w);
        else adjListV5[u].push_back({v, w});
        
        found_idx = -1;
        for(int i = 0; i < adjListV5[v].size(); i++) if(adjListV5[v][i].first == u) found_idx = i;
        if(found_idx != -1) adjListV5[v][found_idx].second = std::min(adjListV5[v][found_idx].second, w);
        else adjListV5[v].push_back({u, w});
    }
    ImGui::Spacing();
    ImGui::Text("Clear the Graph:");
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV5()) && ImGui::Button("Clear", ImVec2(100.0f, 30))){
        scriptV5.clear();
        scriptV5.push_back({{}, -1, -1, "", StepTypeV5::FINISH});
        currentStepIdxV5 = 0;        
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }        
        for(auto &clone:historyV5) delete clone; historyV5.clear();
        isCalculatingHistoryV5 = true;
        firstTime = true;
        adjListV5.clear();
        distV5.clear();
        prevV5.clear();
        visitedV5.clear();
        graphPhysics.clearNodes();
    }
}

void dijkstraVisualization5(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Calculate the shortest path using Dijkstra's algorithm:");
    ImGui::Spacing();
    static char inputBuffer[15000] = "";
    bool temp = false;

    ImGui::Text("Enter the source nodes list:");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##search_input", "Example: 0", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();
    if((checkFinishedV5() || isStepByStep) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        if(adjListV5.empty()) return;
        int n = std::min(3, rand() % (int)distV5.size());
        std::set<int> s;
        while(s.size() < n){
            s.insert(rand() % distV5.size());
        }

        std::string data = "";
        for(int i : s) data += std::to_string(i) + " ";
        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();


    if((isStepByStep || checkFinishedV5()) && (temp || ImGui::Button("Shortest Path", ImVec2(200.0f, 30)))) {
        temp = false;
        std::string data(inputBuffer);
        if(data == "") return;
        
        std::stringstream ss(data);
        std::string token;
        std::set<int> Set;
        bool isValid = true;
        
        while (ss >> token) {
            try {
                size_t parsedChars;
                int sourceNode = std::stoi(token, &parsedChars);
                if (parsedChars != token.length() || sourceNode < 0 || sourceNode >= distV5.size()) {
                    isValid = false; break;
                }
                Set.insert(sourceNode);
            } catch (...) {
                isValid = false; break;
            }
        }
        
        if (!isValid || Set.empty()) {
            setLog("Invalid input!"); return;
        }
        std::vector<int> sourceNodes(Set.begin(), Set.end());

        scriptV5.clear();
        currentStepIdxV5 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        for(auto &clone:historyV5) delete clone; historyV5.clear();
        isCalculatingHistoryV5 = true;
        firstTime = true;

        scriptV5.push_back({{17}, -1, -1, "", StepTypeV5::HIGHLIGHT_1});
        distV5.assign(distV5.size(), INT_MAX);
        prevV5.assign(prevV5.size(), {-1, -1});
        visitedV5.assign(visitedV5.size(), false);
        for(auto &node: graphPhysics.getNodes()) node -> setFillColor(sf::Color::White);

        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        
        for(int sourceNode : sourceNodes) {
            distV5[sourceNode] = 0;
            pq.push({0, sourceNode});
        }
        scriptV5.push_back({{21, 22, 23, 24}, -1, -1, "", StepTypeV5::HIGHLIGHT_1});


        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d > distV5[u]) continue;

            scriptV5.push_back({{26}, -1, -1, "", StepTypeV5::HIGHLIGHT_1});
            scriptV5.push_back({{27, 28, 29}, u, -1, "", StepTypeV5::TRAVERSE_VERTEX});

            for (int i = 0; i < adjListV5[u].size(); i++) {
                int v = adjListV5[u][i].first;
                int weight = adjListV5[u][i].second;
                if (distV5[u] + weight < distV5[v]) {
                    distV5[v] = distV5[u] + weight;
                    scriptV5.push_back({{33, 34, 35, 37, 38, 39}, u, v, "", StepTypeV5::CHOOSE_EDGE, i});
                    pq.push({distV5[v], v}); 
                }
                else{
                    scriptV5.push_back({{33, 34, 35}, u, v, "", StepTypeV5::NOT_CHOOSE_EDGE, i});
                }
            }
        }
        scriptV5.push_back({{42}, -1, -1, "", StepTypeV5::HIGHLIGHT_1});
        scriptV5.push_back({{}, -1, -1, "", StepTypeV5::FINISH});
        distV5.assign(distV5.size(), INT_MAX);
        prevV5.assign(prevV5.size(), {-1, -1});  
        visitedV5.assign(visitedV5.size(), false); 
        for(int sourceNode : sourceNodes) {
            distV5[sourceNode] = 0;
            visitedV5[sourceNode] = true;
        }
    }
}



bool checkNextStepV5(float dt) {
    isWaitingV5 = true;
    
    if(isWaitingV5){ 
        delayTimerV5 += dealtaTime.asSeconds() * dtV5; 
        if (delayTimerV5 >= dt) { 
            isWaitingV5 = false;
            delayTimerV5 = 0;
            return true; 
        }
    }
    
    return false;
}


void drawDijkstra(std::vector<Block*>& list, sf::RenderWindow& window, int u, int i, sf::Color e) {
    if(!isCalculatingHistoryV5){
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        
        std::set<std::pair<int, int>> visited;
        for(int v = 0; v < list.size(); v++){
            if(u != -1 && adjListV5[u][i].first == v){
                sf::Vector2f direction = nodes[v] -> center() - nodes[u] -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                Text weight(std::to_string(adjListV5[u][i].second), style7);
                weight.setPosition((nodes[v] -> center() + nodes[u] -> center()) / 2.f + sf::Vector2f(0, -20));
                if(length > eps){
                    drawArrow(window, nodes[u] -> center() + direction / length * nodes[u] -> getRadius(), nodes[v] -> center() - direction / length * nodes[v] -> getRadius(), e, false, 10.0f);                
                    weight.draw(window);
                }
                visited.insert({std::min(u, v), std::max(u, v)});
            }
            if(prevV5[v] != std::make_pair(-1, -1)){
                sf::Vector2f direction = nodes[v] -> center() - nodes[prevV5[v].first] -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                Text weight(std::to_string(prevV5[v].second), style7);
                weight.setPosition((nodes[v] -> center() + nodes[prevV5[v].first] -> center()) / 2.f + sf::Vector2f(0, -20));
                if(length > eps){
                    drawArrow(window, nodes[prevV5[v].first] -> center() + direction / length * nodes[prevV5[v].first] -> getRadius(), nodes[v] -> center() - direction / length * nodes[v] -> getRadius(), sf::Color::Green, false, 10.0f);                
                    weight.draw(window);
                }
                visited.insert({std::min(prevV5[v].first, v), std::max(prevV5[v].first, v)});
            }
        }
        for(int u = 0; u < adjListV5.size(); u++){
            for(int i = 0; i < adjListV5[u].size(); i++){
                int v = adjListV5[u][i].first;
                if(visited.find({std::min(u, v), std::max(u, v)}) == visited.end()){
                    sf::Vector2f direction = nodes[v] -> center() - nodes[u] -> center();
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    Text weight(std::to_string(adjListV5[u][i].second), style7);
                    weight.setPosition((nodes[v] -> center() + nodes[u] -> center()) / 2.f + sf::Vector2f(0, -20));
                    if(length > eps){
                        drawArrow(window, nodes[u] -> center() + direction / length * nodes[u] -> getRadius(), nodes[v] -> center() - direction / length * nodes[v] -> getRadius(), edgeColor, false, 5.0f);                
                        weight.draw(window);
                    }
                }
            }
        }

        for(int i = 0; i < list.size(); i++){
            Block* node = list[i];
            node -> draw(window);
            Text distText((distV5[i] == INT_MAX ? "inf" : (distV5[i] == 0 ? "source, ":"") + std::to_string(distV5[i])), style6);
            
            distText.setPosition(node -> center() + sf::Vector2f(0, -50));
            distText.draw(window);
        }

    }
}
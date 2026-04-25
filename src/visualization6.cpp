#include <math.h>
#include <set>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include "vector"
#include "visualization6.h"
#include "performVisualization6.h"
#include "main.h"
#include "entity.h"
#include "draw.h"
#include "theme.h"
#include "graphPhysics.h"
#include "importFile.h"

bool isWaitingV6 = false;
float delayTimerV6 = 0;
float dtV6 = 1.0f;

std::vector<std::pair<int,std::pair<int,int>>> adjListV6; 
std::vector<int> visitedEdgeV6;
std::vector<bool> visitedNodeV6;
std::vector<int> parentV6;

void initStatus6(){
    // visualization6.cpp
    isWaitingV6 = false;
    delayTimerV6 = 0;
    dtV6 = 1.0f;
    adjListV6.clear();
    visitedEdgeV6.clear();
    visitedNodeV6.clear();
    parentV6.clear();
    graphPhysics.clearNodes();

    
    // performVisualization6.cpp
    scriptV6.clear();
    scriptV6.push_back({{}, -1, -1, "", StepTypeV6::FINISH});
    currentStepIdxV6 = 0;
    isCalculatingHistoryV6 = false;
    for(auto &clone:historyV6) delete clone; historyV6.clear();
    historyV6.push_back(new cloneVisualization6(visitedEdgeV6, visitedNodeV6));

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


int findRoot(int i) {
    if (parentV6[i] == i) return i;
    return parentV6[i] = findRoot(parentV6[i]);
}

void unite(int i, int j) {
    int root_i = findRoot(i);
    int root_j = findRoot(j);
    if (root_i != root_j) parentV6[root_i] = root_j;
}

void initVisualization6(sf::RenderWindow& window) {
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
    
    if ((isStepByStep || checkFinishedV6()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        int n = rand() % 6 + 4; 
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
    if ((isStepByStep || checkFinishedV6()) && ImGui::Button("Load File", ImVec2(150.0f, 30))) {
        std::string fileContent = openAndReadFile();
        if (!fileContent.empty()) {
            strncpy(inputBuffer, fileContent.c_str(), sizeof(inputBuffer) - 1);
            inputBuffer[sizeof(inputBuffer) - 1] = '\0'; 
            temp = true; 
        }
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV6()) && (temp || ImGui::Button("Confirm", ImVec2(126.0f, 30)))) {
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
            if (pN != tokenN.length() || pM != tokenM.length() || n <= 0 || n > 20 || m < 0) {
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

        std::vector<std::pair<int,std::pair<int,int>>> tempAdj;
        for (auto const& [edge, weight] : edgeMap) {
            int u = edge.first;
            int v = edge.second;
            tempAdj.push_back({weight, {u, v}});
        }


        scriptV6.clear();
        currentStepIdxV6 = 0;
        
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        
        for(auto &clone:historyV6) delete clone; historyV6.clear();
        isCalculatingHistoryV6 = true;
        firstTime = true;
        temp = false; 

        adjListV6 = tempAdj;
        visitedEdgeV6.assign(adjListV6.size(), 0);
        parentV6.assign(n, -1);
        visitedNodeV6.assign(n, false);
        graphPhysics.clearNodes();



        for(int i = 0; i < n; i++){
            Block* node = new Block(ShapeType::CIRCLE, 30.f, std::to_string(i));
            graphPhysics.addNode(node);
        }
        
        scriptV6.push_back({{12, 13, 14, 16}, -1, -1, "", StepTypeV6::HIGHLIGHT_1});
        scriptV6.push_back({{}, -1, -1, "", StepTypeV6::FINISH});

    }
    ImGui::Spacing();    
    ImGui::Text("Add an edge:");
    ImGui::SameLine();
    static char inputEdgeBuffer[15000] = "";
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##random_edge", "Example: 6", inputEdgeBuffer, IM_ARRAYSIZE(inputEdgeBuffer));
    ImGui::SameLine();

    if ((isStepByStep || checkFinishedV6()) && ImGui::Button("Random##Edge", ImVec2(100.0f, 30))){
        int n = visitedNodeV6.size();
        if(n == 0) return;
        std::string data = std::to_string(rand() % n) + " " + std::to_string(rand() % n) + " " + std::to_string(rand() % 100);
        strcpy(inputEdgeBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV6()) && (temp || ImGui::Button("Add", ImVec2(100.0f, 30)))){
        temp = false;
        std::string data(inputEdgeBuffer);
        std::stringstream ss(data);
        std::string tokenU, tokenV, tokenW, extra;
        
        if (!(ss >> tokenU >> tokenV >> tokenW) || (ss >> extra)) {
            setLog("Invalid input!"); return;
        }

        int u = 0, v = 0, w = 0, n = visitedNodeV6.size();
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

        for(auto &clone:historyV6) delete clone; historyV6.clear();
        isCalculatingHistoryV6 = true;
        firstTime = true;
        int found_idx = -1;
        for(int i = 0; i < adjListV6.size(); i++) if(adjListV6[i].second.first == u && adjListV6[i].second.second == v) found_idx = i;
        for(int i = 0; i < adjListV6.size(); i++) if(adjListV6[i].second.first == v && adjListV6[i].second.second == u) found_idx = i;
        if(found_idx != -1) adjListV6[found_idx].first = std::min(adjListV6[found_idx].first, w);
        else adjListV6.push_back({w, {u, v}});
        visitedEdgeV6.assign(adjListV6.size(), 0);
        parentV6.assign(n, -1);
        visitedNodeV6.assign(n, false);
    }
    ImGui::Spacing();
    ImGui::Text("Clear the Graph:");
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV6()) && ImGui::Button("Clear", ImVec2(100.0f, 30))){
        scriptV6.clear();
        scriptV6.push_back({{}, -1, -1, "", StepTypeV6::FINISH});
        currentStepIdxV6 = 0;        
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }        
        for(auto &clone:historyV6) delete clone; historyV6.clear();
        isCalculatingHistoryV6 = true;
        firstTime = true;
        adjListV6.clear();
        visitedEdgeV6.clear();
        parentV6.clear();
        visitedNodeV6.clear();
        graphPhysics.clearNodes();
    }
}

void kruskalVisualization6(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Get the Minimum Spanning Tree:");
    if((isStepByStep || checkFinishedV6()) && ImGui::Button("Get", ImVec2(200.0f, 30))) {
        scriptV6.clear();
        currentStepIdxV6 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        for(auto &clone:historyV6) delete clone; historyV6.clear();
        isCalculatingHistoryV6 = true;
        firstTime = true;

        visitedEdgeV6.assign(adjListV6.size(), 0);
        parentV6.assign(visitedNodeV6.size(), -1);
        visitedNodeV6.assign(visitedNodeV6.size(), false);
        for(auto &node: graphPhysics.getNodes()) node -> setFillColor(sf::Color::White);

        
        for(int i = 0; i < parentV6.size(); i++) parentV6[i] = i;
        scriptV6.push_back({{42}, -1, -1, "", StepTypeV6::HIGHLIGHT_1});
        
        std::sort(adjListV6.begin(), adjListV6.end());
        for(int i = 0; i < adjListV6.size(); i++) {
            int u = adjListV6[i].second.first, v = adjListV6[i].second.second, w = adjListV6[i].first;
            scriptV6.push_back({{44}, i, -1, "", StepTypeV6::TRAVERSE_EDGE});
            scriptV6.push_back({{45, 46, 19, 48}, i, -1, "", StepTypeV6::TRAVERSE_EDGE});
            if(findRoot(u) != findRoot(v)) {
                scriptV6.push_back({{49, 24, 50}, i, -1, "", StepTypeV6::CHOOSE_EDGE});
                unite(u, v);
            }
            else{
                scriptV6.push_back({{52}, i, -1, "", StepTypeV6::NOT_CHOOSE_EDGE});
            }
        }


        scriptV6.push_back({{54}, -1, -1, "", StepTypeV6::HIGHLIGHT_1});
        scriptV6.push_back({{}, -1, -1, "", StepTypeV6::FINISH});
        visitedEdgeV6.assign(adjListV6.size(), 0);  
        parentV6.assign(parentV6.size(), -1);
        visitedNodeV6.assign(visitedNodeV6.size(), false); 
    }
}



bool checkNextStepV6(float dt) {
    isWaitingV6 = true;
    
    if(isWaitingV6){ 
        delayTimerV6 += dealtaTime.asSeconds() * dtV6; 
        if (delayTimerV6 >= dt) { 
            isWaitingV6 = false;
            delayTimerV6 = 0;
            return true; 
        }
    }
    
    return false;
}


void drawKruskal(std::vector<Block*>& list, sf::RenderWindow& window, std::vector<int> &visitedEdgeV6, std::vector<bool> &visitedNodeV6, int id, sf::Color e) {
    if(!isCalculatingHistoryV6){
        std::vector<Block*>& nodes = graphPhysics.getNodes();
        
        for(int i = 0; i < adjListV6.size(); i++){
            int u_adj = adjListV6[i].second.first, v_adj = adjListV6[i].second.second, w_adj = adjListV6[i].first;

            if(i == id){
                sf::Vector2f direction = nodes[v_adj] -> center() - nodes[u_adj] -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                Text weight(std::to_string(w_adj), style7);
                weight.setPosition((nodes[v_adj] -> center() + nodes[u_adj] -> center()) / 2.f + sf::Vector2f(0, -20));
                if(length > eps){
                    drawArrow(window, nodes[u_adj] -> center() + direction / length * nodes[u_adj] -> getRadius(), nodes[v_adj] -> center() - direction / length * nodes[v_adj] -> getRadius(), e, false, 10.0f);                
                    weight.draw(window);
                }
            }
            else{
                sf::Vector2f direction = nodes[v_adj] -> center() - nodes[u_adj] -> center();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                Text weight(std::to_string(w_adj), style7);
                weight.setPosition((nodes[v_adj] -> center() + nodes[u_adj] -> center()) / 2.f + sf::Vector2f(0, -20));
                if(length > eps){
                    sf::Color edgeColorV6;
                    if(visitedEdgeV6[i] == -1) edgeColorV6 = sf::Color(156, 156, 156);
                    else if(visitedEdgeV6[i] == 0) edgeColorV6 = edgeColor;
                    else edgeColorV6 = sf::Color::Green;
                    drawArrow(window, nodes[u_adj] -> center() + direction / length * nodes[u_adj] -> getRadius(), nodes[v_adj] -> center() - direction / length * nodes[v_adj] -> getRadius(), edgeColorV6, false, 5.0f);                
                    weight.draw(window);
                }
            }
        }
    

        for(int i = 0; i < list.size(); i++){
            Block* node = list[i];
            node -> draw(window);            
        }
    }
}

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
    historyV6.clear();
    historyV6.push_back(new cloneVisualization6(visitedEdgeV6, visitedNodeV6));

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

// Kruskal's algorithm
void initVisualization6(sf::RenderWindow& window) {
    ImGui::TextColored(title1Color, "Initializing the graph!");
    ImGui::Spacing();
    

    static char inputBuffer[1024] = "";
    bool temp = false;

    
    // Đặt kích thước khung nhập liệu
    ImGui::SetNextItemWidth(400.0f);
    ImVec2 inputSize = ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4);
    
    // LẤY TỌA ĐỘ MÀN HÌNH CHÍNH XÁC CỦA KHUNG INPUT TRƯỚC KHI VẼ
    ImVec2 inputPos = ImGui::GetCursorScreenPos(); 

    // Vẽ khung Input Multiline bình thường
    ImGui::InputTextMultiline("##init_input", inputBuffer, IM_ARRAYSIZE(inputBuffer), inputSize, ImGuiInputTextFlags_AllowTabInput);

    // ==========================================
    // TẠO HINT TEXT CHÌM (Chỉ vẽ khi buffer đang trống và không được focus)
    // ==========================================
    if (inputBuffer[0] == '\0' && !ImGui::IsItemActive()) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        // Canh lề (padding) cho chữ thụt vào trong khung một chút
        ImVec2 textPos = ImVec2(inputPos.x + 8.0f, inputPos.y + 8.0f); 
        float lineHeight = ImGui::GetTextLineHeight();

        // Vẽ từng dòng chữ chìm bằng màu sắc tùy chỉnh (RGBA: 0-255)
        // IM_COL32(R, G, B, Alpha) - Alpha 150 để tạo độ mờ (chìm)
        drawList->AddText(textPos, IM_COL32(150, 150, 150, 150), "- Line 1: n m (n: vertices, m: edges)");
        textPos.y += lineHeight; // Xuống dòng
        
        drawList->AddText(textPos, IM_COL32(150, 150, 150, 150), "- Next m lines: u v w (Edge from u to v, weight w)");
        textPos.y += lineHeight; // Xuống dòng
        
        // Dòng lưu ý in màu đỏ mờ để thu hút chú ý nhẹ
        drawList->AddText(textPos, IM_COL32(230, 100, 100, 150), "*Note: Vertices are 0-indexed (0 to n-1). Undirected graph.");
    }
    
    ImGui::Spacing();

    if ((isStepByStep || checkFinishedV6()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        // Sinh ngẫu nhiên số đỉnh từ 4 đến 8
        int n = rand() % 6 + 4; 
        
        // Sinh ngẫu nhiên số cạnh (đảm bảo không vượt quá số cạnh tối đa của đồ thị)
        int max_edges = n * (n - 1) / 2;
        int m = rand() % (max_edges / 2 + 1) + n - 1; // Ít nhất n-1 cạnh để đồ thị có thể liên thông
        if (m > max_edges) m = max_edges;

        std::string data = std::to_string(n) + " " + std::to_string(m) + "\n";
        
        // Dùng std::set để chống sinh trùng lặp cạnh (Multigraph)
        std::set<std::pair<int, int>> edges;
        while (edges.size() < m) {
            int u = rand() % n;
            int v = rand() % n;
            if (u != v) {
                // Đảm bảo u < v để set nhận diện được cạnh vô hướng
                if (u > v) std::swap(u, v); 
                edges.insert({u, v});
            }
        }
        
        // Ghi các cạnh vào chuỗi data
        for (auto& edge : edges) {
            int w = rand() % 20 + 1; // Trọng số từ 1 đến 20
            data += std::to_string(edge.first) + " " + std::to_string(edge.second) + " " + std::to_string(w) + "\n";
        }

        strcpy(inputBuffer, data.c_str());
        temp = true;
    }
    ImGui::SameLine();
    if ((isStepByStep || checkFinishedV6()) && (temp || ImGui::Button("Confirm", ImVec2(126.0f, 30)))) {
        temp = false;
        std::string data(inputBuffer);
        if(data == "") {
            temp = false;
            return;
        }
        std::cout << "Input data 1\n" << std::endl;
        std::stringstream ss(data);
        std::string line;
        
        // 1. KIỂM TRA FORMAT n VÀ m
        if (!std::getline(ss, line)) { temp = false; return; }
        std::stringstream lineStream(line);
        int n, m;
        if (!(lineStream >> n >> m) || n <= 0 || m < 0) {
            temp = false;
            return; 
        }

        // ==========================================
        // Dùng std::map để lọc cạnh trùng và cạnh khuyên. 
        // Key là cặp đỉnh {u, v} (quy ước u < v), Value là trọng số min.
        // ==========================================
        std::map<std::pair<int, int>, int> edgeMap;
        int edgeCount = 0;
        bool isValid = true;

        while (std::getline(ss, line)) {
            if (line.empty()) continue; 
            
            std::stringstream es(line);
            int u, v, w;
            
            if (!(es >> u >> v >> w)) { isValid = false; break; }
            if (u < 0 || u >= n || v < 0 || v >= n || w < 0) { isValid = false; break; }
            
            edgeCount++; 

            // LOẠI 1: Bỏ qua CẠNH KHUYÊN (Self-loop)
            if (u == v) continue;

            // LOẠI 2: Xử lý CẠNH TRÙNG (Chỉ lấy trọng số nhỏ nhất)
            // Chuẩn hóa để đỉnh nhỏ hơn luôn đứng trước (vì đồ thị vô hướng)
            int minNode = std::min(u, v);
            int maxNode = std::max(u, v);
            std::pair<int, int> currentEdge = {minNode, maxNode};

            // Nếu cạnh chưa tồn tại trong map -> Thêm mới
            if (edgeMap.find(currentEdge) == edgeMap.end()) {
                edgeMap[currentEdge] = w;
            } 
            // Nếu đã tồn tại -> Cập nhật lại bằng trọng số nhỏ hơn
            else {
                edgeMap[currentEdge] = std::min(edgeMap[currentEdge], w);
            }
        }
        std::cout << "Input data 1.6\n" << std::endl;
        std::cout << isValid << " " << edgeCount << " " << m << std::endl;
        if (!isValid || edgeCount < m) {
            temp = false;
            return;
        }
        std::cout << "Input data 2\n" << std::endl;


        // ==========================================
        // CHUYỂN TỪ MAP SANG DANH SÁCH KỀ CHÍNH THỨC (adjListV6)
        // ==========================================
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
        
        historyV6.clear();
        isCalculatingHistoryV6 = true;
        firstTime = true;
        temp = false; 

        adjListV6 = tempAdj;
        visitedEdgeV6.assign(adjListV6.size(), 0);
        parentV6.assign(n, -1);
        visitedNodeV6.assign(n, false);
        graphPhysics.clearNodes();

        std::cout << "Input data 3\n" << std::endl;


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
    // add random
    static char inputEdgeBuffer[256] = "";
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
        // u v w
        std::string data(inputEdgeBuffer);
        std::stringstream ss(data);
        int u, v, w, n = visitedNodeV6.size();
        if (!(ss >> u >> v >> w) || u < 0 || u >= n || v < 0 || v >= n || w < 0 || u == v) return;
        
        scriptV6.clear();
        scriptV6.push_back({{}, -1, -1, "", StepTypeV6::FINISH});
        currentStepIdxV6 = 0;        
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }        
        historyV6.clear();
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
        historyV6.clear();
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
        historyV6.clear();
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
        std::cout << "**********************************" << delayTimerV6 << "\n";
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

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
    historyV5.clear();
    historyV5.push_back(new cloneVisualization5(distV5, prevV5, visitedV5));

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

// dijkstra's algorithm
void initVisualization5(sf::RenderWindow& window) {
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

    if ((isStepByStep || checkFinishedV5()) && ImGui::Button("Random", ImVec2(100.0f, 30))) {
        // Sinh ngẫu nhiên số đỉnh từ 4 đến 8
        int n = rand() % 5 + 4; 
        
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
    if ((isStepByStep || checkFinishedV5()) && (temp || ImGui::Button("Confirm", ImVec2(125.0f, 30)))) {
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
        std::cout << "Input data 1.5\n" << std::endl;
        std::cout << isValid << " " << edgeCount << " " << m << std::endl;
        if (!isValid || edgeCount < m) {
            temp = false;
            return;
        }
        std::cout << "Input data 2\n" << std::endl;


        // ==========================================
        // CHUYỂN TỪ MAP SANG DANH SÁCH KỀ CHÍNH THỨC (adjListV5)
        // ==========================================
        std::vector<std::vector<std::pair<int, int>>> tempAdj(n);
        for (auto const& [edge, weight] : edgeMap) {
            int u = edge.first;
            int v = edge.second;
            // Đẩy vào cả 2 chiều vì đồ thị vô hướng
            tempAdj[u].push_back({v, weight});
            tempAdj[v].push_back({u, weight});
        }


        scriptV5.clear();
        currentStepIdxV5 = 0;
        
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        
        historyV5.clear();
        isCalculatingHistoryV5 = true;
        firstTime = true;
        temp = false; 

        adjListV5 = tempAdj;
        distV5.assign(n, INT_MAX);
        prevV5.assign(n, {-1, -1});
        visitedV5.assign(n, false);
        graphPhysics.clearNodes();

        std::cout << "Input data 3\n" << std::endl;


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
    // add random
    static char inputEdgeBuffer[256] = "";
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
        // u v w
        std::string data(inputEdgeBuffer);
        std::stringstream ss(data);
        int u, v, w, n = distV5.size();
        if (!(ss >> u >> v >> w) || u < 0 || u >= n || v < 0 || v >= n || w < 0 || u == v) return;
        
        scriptV5.clear();
        scriptV5.push_back({{}, -1, -1, "", StepTypeV5::FINISH});
        currentStepIdxV5 = 0;        
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }        
        historyV5.clear();
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
        historyV5.clear();
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
    static char inputBuffer[256] = "";
    bool temp = false;

    ImGui::Text("Enter the source nodes list:");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("##search_input", "Example: 0", inputBuffer, IM_ARRAYSIZE(inputBuffer));
    ImGui::Spacing();
    std::cout << "GRAPH\n";
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
        std::cout << "shortest path 1\n" << std::endl;
        std::string data(inputBuffer);
        if(data == "") return;
        std::string s;
        std::set<int> Set;
        std::stringstream ss(data);
        while (ss >> s) Set.insert(std::stoi(s));
        std::vector<int> sourceNodes(Set.begin(), Set.end());

        std::cout << "shortest path 2\n" << std::endl;
        scriptV5.clear();
        currentStepIdxV5 = 0;
        if(newNode){
            delete newNode;
            newNode = nullptr;
        }
        historyV5.clear();
        isCalculatingHistoryV5 = true;
        firstTime = true;

        scriptV5.push_back({{17}, -1, -1, "", StepTypeV5::HIGHLIGHT_1});
        distV5.assign(distV5.size(), INT_MAX);
        prevV5.assign(prevV5.size(), {-1, -1});
        visitedV5.assign(visitedV5.size(), false);
        for(auto &node: graphPhysics.getNodes()) node -> setFillColor(sf::Color::White);

        std::cout << "shortest path 3\n" << std::endl;

        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        
        for(int sourceNode : sourceNodes) {
            distV5[sourceNode] = 0;
            pq.push({0, sourceNode});
            // scriptV5.push_back({{22, 23}, sourceNode, -1, "", StepTypeV5::CHOOSE_VERTEX});
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

                // scriptV5.push_back({{33}, u, -1, "", StepTypeV5::TRAVERSE_VERTEX});
                // scriptV5.push_back({{33}, -1, -1, "", StepTypeV5::HIGHLIGHT_1});
                // scriptV5.push_back({{33, 34, 45}, u, v, "", StepTypeV5::TRAVERSE_EDGE, i});
                // scriptV5.push_back({{37}, -1, -1, "", StepTypeV5::HIGHLIGHT_1});
                
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
        std::cout << "shortest path 4\n" << std::endl;
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
        std::cout << "**********************************" << delayTimerV5 << "\n";
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
    
    // for (auto& pair : node->children) {
    //     Block* child = pair.second;
    //     drawTrie(child, window, cloneRootV5);
        
    //     sf::Vector2f direction = child -> center() - node -> center();
    //     float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    //     if(length > eps) if(!isCalculatingHistoryV5) drawArrow(window, node -> center() + direction / length * node -> getRadius(), child -> center() - direction / length * child -> getRadius(), edgeColor);
    // }

}
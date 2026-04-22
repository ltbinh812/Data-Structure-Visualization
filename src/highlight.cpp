#include <iostream>
#include <fstream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <unordered_set>
#include <cctype>
#include "highlight.h"
#include "theme.h"
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
const std::string filepath[] = {
    "/AlgorithmSourceCode/SinglyLinkedList.cpp",
    "/AlgorithmSourceCode/Heap.cpp",
    "/AlgorithmSourceCode/AVLtree.cpp",
    "/AlgorithmSourceCode/Trie.cpp",
    "/AlgorithmSourceCode/Dijkstra.cpp",
    "/AlgorithmSourceCode/Kruskal.cpp"
};
std::vector<std::string> sourceCode;
bool showCodePanel = true;  
float codePanelWidth = 230.0f; 


void printSyntaxLine(const std::string& line, bool isDarkMode, bool isLineActive, ImVec4 activeColor, ImVec4 normalColor) {
    // 1. BẢNG MÀU CHUẨN VS CODE ĐÃ ĐƯỢC CÂN CHỈNH LẠI
    ImVec4 colKeyword  = isDarkMode ? ImVec4(0.77f, 0.52f, 0.75f, 1.0f) : ImVec4(0.68f, 0.16f, 0.73f, 1.0f); // Hồng/Tím
    ImVec4 colType     = isDarkMode ? ImVec4(0.30f, 0.78f, 0.69f, 1.0f) : ImVec4(0.15f, 0.53f, 0.45f, 1.0f); // Xanh Ngọc (Teal)
    ImVec4 colFunc     = isDarkMode ? ImVec4(0.86f, 0.86f, 0.67f, 1.0f) : ImVec4(0.47f, 0.41f, 0.11f, 1.0f); // Vàng nhạt (Function)
    ImVec4 colString   = isDarkMode ? ImVec4(0.80f, 0.56f, 0.47f, 1.0f) : ImVec4(0.63f, 0.08f, 0.08f, 1.0f); // Cam/Đỏ
    ImVec4 colNumber   = isDarkMode ? ImVec4(0.71f, 0.80f, 0.65f, 1.0f) : ImVec4(0.05f, 0.53f, 0.41f, 1.0f); // Xanh lá mạ
    ImVec4 colComment  = isDarkMode ? ImVec4(0.41f, 0.60f, 0.33f, 1.0f) : ImVec4(0.00f, 0.50f, 0.00f, 1.0f); // Xanh lá đậm
    ImVec4 colPunct    = isDarkMode ? ImVec4(0.60f, 0.60f, 0.60f, 1.0f) : ImVec4(0.40f, 0.40f, 0.40f, 1.0f); // Xám (Dấu câu)
    ImVec4 colMacro    = isDarkMode ? ImVec4(0.77f, 0.52f, 0.75f, 1.0f) : ImVec4(0.68f, 0.16f, 0.73f, 1.0f); // Hồng (Cho #include)

    ImVec4 colDefault = isLineActive ? activeColor : normalColor;

    // Cập nhật thêm từ vựng
    static const std::unordered_set<std::string> keywords = {"if", "else", "for", "while", "return", "new", "delete", "nullptr", "class", "struct", "public", "private", "true", "false", "sizeof"};
    static const std::unordered_set<std::string> types = {"int", "float", "double", "bool", "void", "char", "size_t", "Block", "Node", "LinkedList", "std", "vector", "string"};

    std::string buffer = "";
    bool inString = false;
    bool inInclude = false; // Nhận diện dòng có #include
    
    // Check nhanh nếu là dòng include
    if (line.find("#include") != std::string::npos) inInclude = true;

    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];

        // Xử lý Comment (//)
        if (!inString && c == '/' && i + 1 < line.length() && line[i+1] == '/') {
            if (!buffer.empty()) { ImGui::TextColored(colDefault, "%s", buffer.c_str()); ImGui::SameLine(0, 0); buffer.clear(); }
            ImGui::TextColored(colComment, "%s", line.substr(i).c_str());
            ImGui::NewLine();
            return; 
        }

        // Xử lý Thư viện <iostream> hoặc Chuỗi "..."
        if (c == '"' || (inInclude && c == '<')) {
            if (!buffer.empty()) { 
                if (buffer == "#include") ImGui::TextColored(colMacro, "%s", buffer.c_str());
                else ImGui::TextColored(colDefault, "%s", buffer.c_str());
                ImGui::SameLine(0, 0); buffer.clear(); 
            }
            char endChar = (c == '<') ? '>' : '"';
            inString = true;
            buffer += c;
            
            // Đọc hết chuỗi/thư viện
            i++;
            while (i < line.length() && line[i] != endChar) { buffer += line[i]; i++; }
            if (i < line.length()) buffer += line[i]; // Lấy luôn dấu đóng
            
            ImGui::TextColored(colString, "%s", buffer.c_str()); ImGui::SameLine(0, 0); buffer.clear();
            inString = false;
            continue;
        }

        if (c == '#') { buffer += c; continue; } // Bắt đầu Macro

        if (isalnum(c) || c == '_') {
            buffer += c;
        } else {
            if (!buffer.empty()) {
                // THUẬT TOÁN NHẬN DIỆN: TỪ KHÓA / KIỂU / HÀM / BIẾN
                if (buffer == "#include" || buffer == "#define") {
                    ImGui::TextColored(colMacro, "%s", buffer.c_str());
                }
                else if (keywords.count(buffer)) ImGui::TextColored(colKeyword, "%s", buffer.c_str());
                else if (types.count(buffer)) ImGui::TextColored(colType, "%s", buffer.c_str());
                else if (isdigit(buffer[0])) ImGui::TextColored(colNumber, "%s", buffer.c_str());
                else {
                    // Check xem từ này có phải Tên Hàm không? (Nhìn về phía trước xem có dấu '(' không)
                    bool isFunc = false;
                    size_t peek = i;
                    while (peek < line.length() && isspace(line[peek])) peek++; // Bỏ qua dấu cách
                    if (peek < line.length() && line[peek] == '(') isFunc = true;

                    if (isFunc) ImGui::TextColored(colFunc, "%s", buffer.c_str()); // Vàng
                    else ImGui::TextColored(colDefault, "%s", buffer.c_str());     // Trắng/Đen (Biến bình thường)
                }
                
                ImGui::SameLine(0, 0);
                buffer.clear();
            }
            
            // Xử lý các dấu câu (Punctuation: ngoặc, chấm phẩy, toán tử...)
            std::string symbol(1, c);
            if (!isspace(c)) {
                ImGui::TextColored(colPunct, "%s", symbol.c_str()); // Cho dấu câu mờ đi một chút
            } else {
                ImGui::Text("%s", symbol.c_str()); // Giữ nguyên khoảng trắng
            }
            ImGui::SameLine(0, 0);
        }
    }
    
    // In nốt từ cuối cùng
    if (!buffer.empty()) {
        if (keywords.count(buffer)) ImGui::TextColored(colKeyword, "%s", buffer.c_str());
        else if (types.count(buffer)) ImGui::TextColored(colType, "%s", buffer.c_str());
        else if (isdigit(buffer[0])) ImGui::TextColored(colNumber, "%s", buffer.c_str());
        else ImGui::TextColored(colDefault, "%s", buffer.c_str());
    }
    
    ImGui::NewLine(); 
}


std::vector<std::string> loadSourceCode(int indexCSourceCode) {
    std::vector<std::string> code;    
    std::string rootPath = TOSTRING(ROOT_DIR);    
    rootPath.erase(std::remove(rootPath.begin(), rootPath.end(), '\"'), rootPath.end());
    std::string fullPath = rootPath + filepath[indexCSourceCode];
    std::ifstream file(fullPath);
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            code.push_back(line);
        }
        file.close();
    } else {
        code.push_back("// ERROR: Cannot open file at:");
        code.push_back("// " + fullPath); 
        // std::cout << "ERROR: Cannot open file at: " << fullPath << std::endl;
    }
    return code;
}

float screenWidth = 0.0f;
float screenHeight = 0.0f;

void drawCodeHighlightPanel(int indexSourceCode, int currentStepIdx, bool isStepByStep, bool isFinished, std::vector<int> activeLines) {
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;    
    screenWidth = displaySize.x;
    screenHeight = displaySize.y;

    static int lastLoadedIndex = -1;
    std::cout << "isDarkMode: " << isDarkMode << std::endl;
    if (indexSourceCode != lastLoadedIndex) {
        sourceCode = loadSourceCode(indexSourceCode - 1);
        lastLoadedIndex = indexSourceCode;
    }


// 2. KHỞI TẠO CHIỀU RỘNG LẦN ĐẦU TIÊN
    static bool isFirstTimeWidth = true;
    if (isFirstTimeWidth) {
        codePanelWidth = screenWidth / 3.0f;
        isFirstTimeWidth = false;
    }


    float buttonRadius = 16.0f;
    // Nếu đang hiện code: Nút bám lề trái của khung code. Nếu ẩn: Nút bám lề phải màn hình.
    float panelCurrentX = showCodePanel ? (screenWidth - codePanelWidth) : screenWidth;
    
    // Tọa độ tâm của nút: Lệch sang trái 12px so với biên, và cách lề trên 30px
    float btnCenterX = panelCurrentX - 20.0f; 
    float btnCenterY = 30.0f; 

    // Setup một cửa sổ tàng hình vừa khít với nút tròn
    ImGui::SetNextWindowPos(ImVec2(btnCenterX - buttonRadius, btnCenterY - buttonRadius));
    ImGui::SetNextWindowSize(ImVec2(buttonRadius * 2, buttonRadius * 2));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // Trong suốt tuyệt đối

    ImGui::Begin("ToggleCodeBtn", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);
    
    // 1. Tạo hit-box bắt sự kiện click
    if (ImGui::InvisibleButton("##toggle_btn", ImVec2(buttonRadius * 2, buttonRadius * 2))) {
        showCodePanel = !showCodePanel;
    }
    bool isBtnHovered = ImGui::IsItemHovered();

    // 2. Tự vẽ hình tròn đè lên trên hit-box với MÀU NỀN NỔI BẬT
    ImU32 circleColor;
    ImU32 borderColor;
    ImU32 textColor; 

    if (isDarkMode) {
        // GIAO DIỆN DARK: Nút màu Vàng Gruvbox siêu ngầu
        circleColor = isBtnHovered ? IM_COL32(250, 189, 47, 255)  
                                   : IM_COL32(215, 153, 33, 255); 
        borderColor = IM_COL32(255, 255, 255, 50);                
        textColor   = IM_COL32(40, 40, 40, 255);                  
    } else {
        // GIAO DIỆN LIGHT: Nút màu Xanh Lam VS Code thanh lịch
        circleColor = isBtnHovered ? IM_COL32(43, 144, 255, 255)  
                                   : IM_COL32(0, 102, 204, 255);  
        borderColor = IM_COL32(0, 0, 0, 50);                      
        textColor   = IM_COL32(255, 255, 255, 255);               
    }
    
    // Vẽ nền và viền
    ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(btnCenterX, btnCenterY), buttonRadius, circleColor);
    ImGui::GetWindowDrawList()->AddCircle(ImVec2(btnCenterX, btnCenterY), buttonRadius, borderColor, 0, 1.5f);
    
    // 3. Vẽ Icon Mũi tên (Dấu > khi đang hiện, dấu < khi đã ẩn)
    ImGui::PushFont(codeFont); 
    const char* iconText = showCodePanel ? ">" : "<";
    ImVec2 textSize = ImGui::CalcTextSize(iconText);
    
    // Tính tọa độ chữ và in ra với màu tương phản
    ImGui::GetWindowDrawList()->AddText(ImVec2(btnCenterX - textSize.x / 2.0f, btnCenterY - textSize.y / 2.0f), textColor, iconText);
    ImGui::PopFont();

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);

    if(!showCodePanel) return;




    ImGui::SetNextWindowPos(ImVec2(screenWidth - codePanelWidth, 0), ImGuiCond_Always);    
    ImGui::SetNextWindowSize(ImVec2(codePanelWidth, screenHeight), ImGuiCond_Once);

    ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, screenHeight), ImVec2(screenWidth * 0.5f, screenHeight));
    // THÊM CỜ THANH CUỘN NGANG VÀ DỌC
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | 
                                   ImGuiWindowFlags_NoTitleBar |
                                   ImGuiWindowFlags_HorizontalScrollbar | 
                                   ImGuiWindowFlags_AlwaysVerticalScrollbar; // Luôn hiện cuộn dọc
    
    bool forceLockScroll = !isStepByStep && !isFinished;
    if (forceLockScroll) {
        windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
    }

    // TẠO VIỀN CHO PANEL ĐỂ DỄ NHẬN BIẾT
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f); // Độ dày viền
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);   // Góc vuông để áp sát lề
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 20.0f);

    // Đổi màu viền và nền panel (Phong cách CATPPUCCIN)
    if (isDarkMode) {
        // Viền: Tím xám thẫm (Mocha Mantle)
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.09f, 0.09f, 0.15f, 1.0f)); 
        
        // Nền Dark: Tím xám khói. Rất sáng sủa, mượt mà và tuyệt đối không bị "ngộp"
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.18f, 1.0f)); 
    } else {
        // Viền: Xám ánh xanh nhạt (Latte Mantle)
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.90f, 0.91f, 0.94f, 1.0f)); 
        
        // Nền Light: Trắng ngà ánh tím sương. Cực kỳ nhẹ nhàng, trông như trang giấy xịn.
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.94f, 0.95f, 0.96f, 1.0f)); 
    }

    ImGui::PushFont(codeFont);
    ImGui::Begin("Code Highlight", nullptr, windowFlags);
    
    
    // Khóa an toàn: Không cho kéo quá to (vượt 80% màn hình) hoặc quá nhỏ (dưới 200px)
    codePanelWidth = ImGui::GetWindowWidth();
    if (codePanelWidth < 200.0f) codePanelWidth = 200.0f;
    if (codePanelWidth > screenWidth * 0.8f) codePanelWidth = screenWidth * 0.5f;


    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();

    // 1. Dải nền mờ dọc theo toàn bộ cạnh trái (giúp người dùng biết khu vực có thể nắm kéo)
    ImU32 edgeBgColor = isDarkMode ? IM_COL32(100, 100, 130, 80) : IM_COL32(200, 200, 220, 100);
    drawList->AddRectFilled(winPos, ImVec2(winPos.x + 6.0f, winPos.y + winSize.y), edgeBgColor);

    // 2. Cái "Tay cầm" (Viên thuốc nhỏ ở chính giữa)
    float gripHeight = 100.0f; // Chiều cao tay cầm
    float gripWidth = 7.0f;   // Chiều rộng tay cầm
    
    // Tính toán tọa độ để đặt viên thuốc ra đúng giữa trục Y
    ImVec2 gripP1(winPos.x + 2.0f, winPos.y + (winSize.y - gripHeight) / 2.0f);
    ImVec2 gripP2(winPos.x + 2.0f + gripWidth, winPos.y + (winSize.y + gripHeight) / 2.0f);
    
    // Đổi màu tay cầm cho nổi bật trên Theme
    ImU32 gripColor = isDarkMode ? IM_COL32(180, 180, 200, 255) : IM_COL32(120, 120, 140, 255);
    
    // Lệnh vẽ hình chữ nhật bo góc (Tham số cuối 2.0f là bán kính bo tròn)
    drawList->AddRectFilled(gripP1, gripP2, gripColor, 2.0f);


    // Xóa Push để không ảnh hưởng các UI khác
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3); 

    static int lastStepIdx = -1;
    bool stepJustChanged = (currentStepIdx != lastStepIdx);
    if (stepJustChanged) lastStepIdx = currentStepIdx;

    float scrollY = ImGui::GetScrollY();
    float windowH = ImGui::GetWindowHeight();
    float lineHeight = ImGui::GetTextLineHeightWithSpacing();

    int topVisibleLine = static_cast<int>(scrollY / lineHeight);
    int bottomVisibleLine = static_cast<int>((scrollY + windowH) / lineHeight);

    bool hasHighlightAbove = false;
    bool hasHighlightBelow = false;
    int targetFocusLine = activeLines.empty() ? -1 : activeLines[0];

    // XÁC ĐỊNH MÀU CHỮ DỰA TRÊN THEME
    ImVec4 normalCodeColor = isDarkMode ? ImVec4(0.85f, 0.90f, 0.95f, 1.0f) : ImVec4(0.1f, 0.1f, 0.15f, 1.0f);
    ImVec4 highlightCodeColor = isDarkMode ? ImVec4(1.0f, 0.9f, 0.2f, 1.0f) : ImVec4(0.8f, 0.1f, 0.1f, 1.0f); // Vàng cho Dark, Đỏ đô cho Light

    // DUYỆT VÀ IN TỪNG DÒNG CODE
// DUYỆT VÀ IN TỪNG DÒNG CODE
    for (int i = 0; i < sourceCode.size(); i++) {
        
        auto it = std::find(activeLines.begin(), activeLines.end(), i + 1);
        bool isHighlighted = (it != activeLines.end());
        
        int highlightOrder = -1;
        if (isHighlighted) {
            highlightOrder = std::distance(activeLines.begin(), it) + 1; 
        }

        if (isHighlighted) {
            if (i < topVisibleLine) hasHighlightAbove = true;
            if (i >= bottomVisibleLine - 1) hasHighlightBelow = true;
        }

        // 1. LƯU LẠI TỌA ĐỘ LỀ TRÁI GỐC
        float startPosX = ImGui::GetCursorPosX();

        if (isHighlighted) {
            ImVec4 highlightBgColor = isDarkMode ? ImVec4(0.60f, 0.20f, 0.20f, 0.5f) : ImVec4(0.98f, 0.92f, 0.65f, 0.8f);
            ImGui::PushStyleColor(ImGuiCol_Header, highlightBgColor);
            ImGui::Selectable((std::string("##line") + std::to_string(i)).c_str(), true, ImGuiSelectableFlags_SpanAllColumns);
            ImGui::PopStyleColor();
            ImGui::SameLine();
        }

        // 2. TRẢ CON TRỎ VỀ ĐÚNG LỀ TRÁI 
        ImGui::SetCursorPosX(startPosX);

        // Xử lý cuộn màn hình
        if (isHighlighted && targetFocusLine == i + 1) {
            if (forceLockScroll || stepJustChanged) {
                ImGui::SetScrollHereY(0.5f); 
            }
        }

        ImVec4 lineNumColor = isHighlighted ? highlightCodeColor : ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        
        // 3. In số dòng và dấu "|"
        ImGui::TextColored(lineNumColor, "%3d |", i + 1);
        ImGui::SameLine();

        // 4. Lấy vị trí lề hiện tại (ngay sau dấu |) và tính lề cho Code
        float afterPipeX = ImGui::GetCursorPosX();
        float codeOffset = afterPipeX + (activeLines.size() > 1 ? 35.0f : 0.0f); // Chừa 28px khoảng trống cho ô vuông

        // 5. VẼ Ô VUÔNG BỌC SỐ (NẾU dòng này được highlight và có nhiều dòng)
        if (isHighlighted && activeLines.size() > 1) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            
            // Lấy tọa độ tuyệt đối trên màn hình để vẽ đồ họa
            ImVec2 screenPos = ImGui::GetCursorScreenPos(); 
            
            float boxSize = 20.0f; // Kích thước ô vuông (18x18 pixel)
            
            // Đẩy ô vuông xuống 1 chút xíu để căn giữa với dòng chữ
            ImVec2 boxMin(screenPos.x, screenPos.y + 1.0f); 
            ImVec2 boxMax(screenPos.x + boxSize, screenPos.y + 1.0f + boxSize);
            
            // Màu sắc ô vuông: Cam rực (Dark Mode) hoặc Đỏ đậm (Light Mode)
            ImU32 boxColor = isDarkMode ? IM_COL32(140, 90, 210, 255)   // Xanh lá mạ
                                : IM_COL32(50, 160, 90, 255);  // Xanh lục đậm
            ImU32 textColor = IM_COL32(255, 255, 255, 255); // Chữ số luôn màu trắng cho nổi
            
            // Lệnh vẽ hình vuông bo góc (3.0f là độ bo tròn)
            drawList->AddRectFilled(boxMin, boxMax, boxColor, 3.0f);
            
            // Tính toán để in số nằm ngay chính giữa ô vuông
            std::string orderStr = std::to_string(highlightOrder);
            ImGui::PushFont(codeFont);
            ImVec2 textSize = ImGui::CalcTextSize(orderStr.c_str());
            ImVec2 textPos(
                boxMin.x + (boxSize - textSize.x) / 2.0f,
                boxMin.y + (boxSize - textSize.y) / 2.0f
            );
            
            drawList->AddText(textPos, textColor, orderStr.c_str());
            ImGui::PopFont();
        }

        // 6. Ép con trỏ nhảy tới đúng lề Code đã chừa khoảng trống
        ImGui::SetCursorPosX(codeOffset);

        // 7. In Text C++ 
        printSyntaxLine(sourceCode[i], isDarkMode, isHighlighted, highlightCodeColor, normalCodeColor);
    }

    // Logic vẽ mũi tên (Giữ nguyên)
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
 
    
    // Tọa độ tâm X để đặt mũi tên vào chính giữa cửa sổ
    float centerX = winPos.x + winSize.x / 2.0f;
    
    // 1. TĂNG CHIỀU CAO DẢI BĂNG (60px cho to và rõ ràng hơn)
    float bannerHeight = 40.0f; 
    
    // 2. TÍNH TỌA ĐỘ ĐÁY THỰC TẾ (Trừ đi 22px để né thanh cuộn ngang)
    float bottomEdge = winPos.y + winSize.y - 22.0f; 

    // 3. PHỐI LẠI MÀU (Light Mode sang tone Vàng)
    ImU32 bannerColor = isDarkMode ? IM_COL32(180, 50, 50, 120)    // Dark: Đỏ mờ
                                   : IM_COL32(255, 220, 100, 150); // Light: Vàng tươi mờ
    ImU32 arrowColor  = isDarkMode ? IM_COL32(255, 100, 100, 255)  // Dark: Đỏ rực
                                   : IM_COL32(220, 140, 0, 255);   // Light: Vàng cam đậm (Nổi bật trên nền vàng mờ)

    if (hasHighlightAbove) {
        // Vẽ dải Banner trên
        draw_list->AddRectFilled(
            ImVec2(winPos.x, winPos.y), 
            ImVec2(winPos.x + winSize.x, winPos.y + bannerHeight), 
            bannerColor
        );

        // Vẽ mũi tên TO HƠN chỉ LÊN (Rộng 40px, cao 25px)
        ImVec2 p1(centerX, winPos.y + 10.0f);          // Đỉnh trên
        ImVec2 p2(centerX - 20.0f, winPos.y + 30.0f);  // Đáy trái
        ImVec2 p3(centerX + 20.0f, winPos.y + 30.0f);  // Đáy phải
        draw_list->AddTriangleFilled(p1, p2, p3, arrowColor);
    }
    
    if (hasHighlightBelow) {
        // Vẽ dải Banner dưới (Đã nâng lên khỏi thanh cuộn)
        draw_list->AddRectFilled(
            ImVec2(winPos.x, bottomEdge - bannerHeight), 
            ImVec2(winPos.x + winSize.x, bottomEdge), 
            bannerColor
        );

        // Vẽ mũi tên TO HƠN chỉ XUỐNG (Rộng 40px, cao 25px)
        ImVec2 p1(centerX, bottomEdge - 10.0f);          // Đỉnh dưới
        ImVec2 p2(centerX - 20.0f, bottomEdge - 30.0f);  // Đáy trái
        ImVec2 p3(centerX + 20.0f, bottomEdge - 30.0f);  // Đáy phải
        draw_list->AddTriangleFilled(p1, p2, p3, arrowColor);
    }

    ImGui::End();
    ImGui::PopFont();
}
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

    ImVec4 colKeyword  = isDarkMode ? ImVec4(0.77f, 0.52f, 0.75f, 1.0f) : ImVec4(0.68f, 0.16f, 0.73f, 1.0f); 
    ImVec4 colType     = isDarkMode ? ImVec4(0.30f, 0.78f, 0.69f, 1.0f) : ImVec4(0.15f, 0.53f, 0.45f, 1.0f); 
    ImVec4 colFunc     = isDarkMode ? ImVec4(0.86f, 0.86f, 0.67f, 1.0f) : ImVec4(0.47f, 0.41f, 0.11f, 1.0f); 
    ImVec4 colString   = isDarkMode ? ImVec4(0.80f, 0.56f, 0.47f, 1.0f) : ImVec4(0.63f, 0.08f, 0.08f, 1.0f); 
    ImVec4 colNumber   = isDarkMode ? ImVec4(0.71f, 0.80f, 0.65f, 1.0f) : ImVec4(0.05f, 0.53f, 0.41f, 1.0f); 
    ImVec4 colComment  = isDarkMode ? ImVec4(0.41f, 0.60f, 0.33f, 1.0f) : ImVec4(0.00f, 0.50f, 0.00f, 1.0f); 
    ImVec4 colPunct    = isDarkMode ? ImVec4(0.60f, 0.60f, 0.60f, 1.0f) : ImVec4(0.40f, 0.40f, 0.40f, 1.0f); 
    ImVec4 colMacro    = isDarkMode ? ImVec4(0.77f, 0.52f, 0.75f, 1.0f) : ImVec4(0.68f, 0.16f, 0.73f, 1.0f); 

    ImVec4 colDefault = isLineActive ? activeColor : normalColor;

    static const std::unordered_set<std::string> keywords = {"if", "else", "for", "while", "return", "new", "delete", "nullptr", "class", "struct", "public", "private", "true", "false", "sizeof"};
    static const std::unordered_set<std::string> types = {"int", "float", "double", "bool", "void", "char", "size_t", "Block", "Node", "LinkedList", "std", "vector", "string"};

    std::string buffer = "";
    bool inString = false;
    bool inInclude = false; 
    
    if (line.find("#include") != std::string::npos) inInclude = true;

    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];

        if (!inString && c == '/' && i + 1 < line.length() && line[i+1] == '/') {
            if (!buffer.empty()) { ImGui::TextColored(colDefault, "%s", buffer.c_str()); ImGui::SameLine(0, 0); buffer.clear(); }
            ImGui::TextColored(colComment, "%s", line.substr(i).c_str());
            ImGui::NewLine();
            return; 
        }

        if (c == '"' || (inInclude && c == '<')) {
            if (!buffer.empty()) { 
                if (buffer == "#include") ImGui::TextColored(colMacro, "%s", buffer.c_str());
                else ImGui::TextColored(colDefault, "%s", buffer.c_str());
                ImGui::SameLine(0, 0); buffer.clear(); 
            }
            char endChar = (c == '<') ? '>' : '"';
            inString = true;
            buffer += c;
            
            i++;
            while (i < line.length() && line[i] != endChar) { buffer += line[i]; i++; }
            if (i < line.length()) buffer += line[i]; 
            
            ImGui::TextColored(colString, "%s", buffer.c_str()); ImGui::SameLine(0, 0); buffer.clear();
            inString = false;
            continue;
        }

        if (c == '#') { buffer += c; continue; } 

        if (isalnum(c) || c == '_') {
            buffer += c;
        } else {
            if (!buffer.empty()) {
                if (buffer == "#include" || buffer == "#define") {
                    ImGui::TextColored(colMacro, "%s", buffer.c_str());
                }
                else if (keywords.count(buffer)) ImGui::TextColored(colKeyword, "%s", buffer.c_str());
                else if (types.count(buffer)) ImGui::TextColored(colType, "%s", buffer.c_str());
                else if (isdigit(buffer[0])) ImGui::TextColored(colNumber, "%s", buffer.c_str());
                else {
                    bool isFunc = false;
                    size_t peek = i;
                    while (peek < line.length() && isspace(line[peek])) peek++; 
                    if (peek < line.length() && line[peek] == '(') isFunc = true;

                    if (isFunc) ImGui::TextColored(colFunc, "%s", buffer.c_str()); 
                    else ImGui::TextColored(colDefault, "%s", buffer.c_str());     
                }
                
                ImGui::SameLine(0, 0);
                buffer.clear();
            }
            
            std::string symbol(1, c);
            if (!isspace(c)) {
                ImGui::TextColored(colPunct, "%s", symbol.c_str()); 
            } else {
                ImGui::Text("%s", symbol.c_str()); 
            }
            ImGui::SameLine(0, 0);
        }
    }
    
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
    if (indexSourceCode != lastLoadedIndex) {
        sourceCode = loadSourceCode(indexSourceCode - 1);
        lastLoadedIndex = indexSourceCode;
    }


    static bool isFirstTimeWidth = true;
    if (isFirstTimeWidth) {
        codePanelWidth = screenWidth / 3.0f;
        isFirstTimeWidth = false;
    }


    float buttonRadius = 16.0f;
    float panelCurrentX = showCodePanel ? (screenWidth - codePanelWidth) : screenWidth;
    
    float btnCenterX = panelCurrentX - 20.0f; 
    float btnCenterY = 30.0f; 

    ImGui::SetNextWindowPos(ImVec2(btnCenterX - buttonRadius, btnCenterY - buttonRadius));
    ImGui::SetNextWindowSize(ImVec2(buttonRadius * 2, buttonRadius * 2));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); 

    ImGui::Begin("ToggleCodeBtn", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);
    
    if (ImGui::InvisibleButton("##toggle_btn", ImVec2(buttonRadius * 2, buttonRadius * 2))) {
        showCodePanel = !showCodePanel;
    }
    bool isBtnHovered = ImGui::IsItemHovered();

    ImU32 circleColor;
    ImU32 borderColor;
    ImU32 textColor; 

    if (isDarkMode) {
        circleColor = isBtnHovered ? IM_COL32(250, 189, 47, 255)  
                                   : IM_COL32(215, 153, 33, 255); 
        borderColor = IM_COL32(255, 255, 255, 50);                
        textColor   = IM_COL32(40, 40, 40, 255);                  
    } else {
        circleColor = isBtnHovered ? IM_COL32(43, 144, 255, 255)  
                                   : IM_COL32(0, 102, 204, 255);  
        borderColor = IM_COL32(0, 0, 0, 50);                      
        textColor   = IM_COL32(255, 255, 255, 255);               
    }
    
    ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(btnCenterX, btnCenterY), buttonRadius, circleColor);
    ImGui::GetWindowDrawList()->AddCircle(ImVec2(btnCenterX, btnCenterY), buttonRadius, borderColor, 0, 1.5f);
    
    ImGui::PushFont(codeFont); 
    const char* iconText = showCodePanel ? ">" : "<";
    ImVec2 textSize = ImGui::CalcTextSize(iconText);
    
    ImGui::GetWindowDrawList()->AddText(ImVec2(btnCenterX - textSize.x / 2.0f, btnCenterY - textSize.y / 2.0f), textColor, iconText);
    ImGui::PopFont();

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);

    if(!showCodePanel) return;




    ImGui::SetNextWindowPos(ImVec2(screenWidth - codePanelWidth, 0), ImGuiCond_Always);    
    ImGui::SetNextWindowSize(ImVec2(codePanelWidth, screenHeight), ImGuiCond_Once);

    ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, screenHeight), ImVec2(screenWidth * 0.5f, screenHeight));
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | 
                                   ImGuiWindowFlags_NoTitleBar |
                                   ImGuiWindowFlags_HorizontalScrollbar | 
                                   ImGuiWindowFlags_AlwaysVerticalScrollbar; 
    
    bool forceLockScroll = !isStepByStep && !isFinished;
    if (forceLockScroll) {
        windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f); 
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);   
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 20.0f);

    if (isDarkMode) {
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.09f, 0.09f, 0.15f, 1.0f)); 
        
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.18f, 1.0f)); 
    } else {
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.90f, 0.91f, 0.94f, 1.0f)); 
        
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.94f, 0.95f, 0.96f, 1.0f)); 
    }

    ImGui::PushFont(codeFont);
    ImGui::Begin("Code Highlight", nullptr, windowFlags);
    
    
    codePanelWidth = ImGui::GetWindowWidth();
    if (codePanelWidth < 200.0f) codePanelWidth = 200.0f;
    if (codePanelWidth > screenWidth * 0.8f) codePanelWidth = screenWidth * 0.5f;


    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 winSize = ImGui::GetWindowSize();

    ImU32 edgeBgColor = isDarkMode ? IM_COL32(100, 100, 130, 80) : IM_COL32(200, 200, 220, 100);
    drawList->AddRectFilled(winPos, ImVec2(winPos.x + 6.0f, winPos.y + winSize.y), edgeBgColor);

    float gripHeight = 100.0f; 
    float gripWidth = 7.0f;   
    
    ImVec2 gripP1(winPos.x + 2.0f, winPos.y + (winSize.y - gripHeight) / 2.0f);
    ImVec2 gripP2(winPos.x + 2.0f + gripWidth, winPos.y + (winSize.y + gripHeight) / 2.0f);
    
    ImU32 gripColor = isDarkMode ? IM_COL32(180, 180, 200, 255) : IM_COL32(120, 120, 140, 255);
    
    drawList->AddRectFilled(gripP1, gripP2, gripColor, 2.0f);


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

    ImVec4 normalCodeColor = isDarkMode ? ImVec4(0.85f, 0.90f, 0.95f, 1.0f) : ImVec4(0.1f, 0.1f, 0.15f, 1.0f);
    ImVec4 highlightCodeColor = isDarkMode ? ImVec4(1.0f, 0.9f, 0.2f, 1.0f) : ImVec4(0.8f, 0.1f, 0.1f, 1.0f); 


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

        float startPosX = ImGui::GetCursorPosX();

        if (isHighlighted) {
            ImVec4 highlightBgColor = isDarkMode ? ImVec4(0.60f, 0.20f, 0.20f, 0.5f) : ImVec4(0.98f, 0.92f, 0.65f, 0.8f);
            ImGui::PushStyleColor(ImGuiCol_Header, highlightBgColor);
            ImGui::Selectable((std::string("##line") + std::to_string(i)).c_str(), true, ImGuiSelectableFlags_SpanAllColumns);
            ImGui::PopStyleColor();
            ImGui::SameLine();
        }


        ImGui::SetCursorPosX(startPosX);

        if (isHighlighted && targetFocusLine == i + 1) {
            if (forceLockScroll || stepJustChanged) {
                ImGui::SetScrollHereY(0.5f); 
            }
        }

        ImVec4 lineNumColor = isHighlighted ? highlightCodeColor : ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        
        ImGui::TextColored(lineNumColor, "%3d |", i + 1);
        ImGui::SameLine();

        float afterPipeX = ImGui::GetCursorPosX();
        float codeOffset = afterPipeX + (activeLines.size() > 1 ? 35.0f : 0.0f); 

        if (isHighlighted && activeLines.size() > 1) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            
            ImVec2 screenPos = ImGui::GetCursorScreenPos(); 
            
            float boxSize = 20.0f; 
            
            ImVec2 boxMin(screenPos.x, screenPos.y + 1.0f); 
            ImVec2 boxMax(screenPos.x + boxSize, screenPos.y + 1.0f + boxSize);
            
            ImU32 boxColor = isDarkMode ? IM_COL32(140, 90, 210, 255)
                                : IM_COL32(50, 160, 90, 255);  
            ImU32 textColor = IM_COL32(255, 255, 255, 255); 
            
            drawList->AddRectFilled(boxMin, boxMax, boxColor, 3.0f);
            
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

        ImGui::SetCursorPosX(codeOffset);

        printSyntaxLine(sourceCode[i], isDarkMode, isHighlighted, highlightCodeColor, normalCodeColor);
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
 
    
    float centerX = winPos.x + winSize.x / 2.0f;
    
    float bannerHeight = 40.0f; 
    
    float bottomEdge = winPos.y + winSize.y - 22.0f; 

    ImU32 bannerColor = isDarkMode ? IM_COL32(180, 50, 50, 120)    
                                   : IM_COL32(255, 220, 100, 150); 
    ImU32 arrowColor  = isDarkMode ? IM_COL32(255, 100, 100, 255)  
                                   : IM_COL32(220, 140, 0, 255);   

    if (hasHighlightAbove) {
        draw_list->AddRectFilled(
            ImVec2(winPos.x, winPos.y), 
            ImVec2(winPos.x + winSize.x, winPos.y + bannerHeight), 
            bannerColor
        );

        ImVec2 p1(centerX, winPos.y + 10.0f);          
        ImVec2 p2(centerX - 20.0f, winPos.y + 30.0f);  
        ImVec2 p3(centerX + 20.0f, winPos.y + 30.0f);  
        draw_list->AddTriangleFilled(p1, p2, p3, arrowColor);
    }
    
    if (hasHighlightBelow) {
        draw_list->AddRectFilled(
            ImVec2(winPos.x, bottomEdge - bannerHeight), 
            ImVec2(winPos.x + winSize.x, bottomEdge), 
            bannerColor
        );

        ImVec2 p1(centerX, bottomEdge - 10.0f);          
        ImVec2 p2(centerX - 20.0f, bottomEdge - 30.0f);  
        ImVec2 p3(centerX + 20.0f, bottomEdge - 30.0f);  
        draw_list->AddTriangleFilled(p1, p2, p3, arrowColor);
    }

    ImGui::End();
    ImGui::PopFont();
}
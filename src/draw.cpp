#define SKIP_EXTERN_script
#include <iostream>
#include <math.h>
#include <vector>
#include "draw.h"
#include "main.h"
#include "entity.h"
#include "theme.h"
#include "highlight.h"
#include "graphPhysics.h"
#include "visualization1.h"
#include "performVisualization1.h"
#include "visualization2.h"
#include "performVisualization2.h"
#include "visualization3.h"
#include "performVisualization3.h"
#include "visualization4.h"
#include "performVisualization4.h"
#include "visualization5.h"
#include "performVisualization5.h"


Block* Log = nullptr;
float delayLog = 0;
OperationType o = INITIALIZE;
Block* newNode = nullptr;
bool isStepByStep = false;
int choosePrevNextButton = 0;
bool firstTime = true;

void setLog(std::string text, sf::Color color, sf::Color textColor) {
    Log = new Block(RECTANGLE, 500.f, 40.f, text);
    Log -> setFillColor(color);
    Log -> setFillColorText(textColor);
    Log -> currentPosition = {-300.f, WINDOW_HEIGHT - 500.f};
    Log -> targetPosition = {300.f, WINDOW_HEIGHT - 500.f};
    Log -> setPosition(Log->currentPosition);
    delayLog = 0;
}

bool checkMove(Block *Node){
    sf::Vector2f direction = Node -> targetPosition - Node -> currentPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if(distance > 0.0001f) return true;
    return false;
}

void drawLog(sf::RenderWindow& window) {
    if(Log){
        if(checkMove(Log)){
            Log->move(8.0f, false);
            delayLog = 0;
        }
        else{
            delayLog += dealtaTime.asSeconds();
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

void drawMainMenu(sf::RenderWindow& window, std::vector<Text>& texts, std::vector<Block>& buttons, std::vector<Image>& images, std::vector<Card>& cards){
   
    
    for(auto &button: buttons){
        button.draw(window);
    }

    for(auto &image: images){
        image.draw(window);
    }    


    texts[0].setPosition(WINDOW_WIDTH / 2.f, 100.f);
    for(auto &text: texts){    
        text.draw(window);
    }


    for(auto &card: cards){
        card.draw(window);
    }
    
}

void drawModeButton(sf::Vector2u windowSize, float panelHeight, bool checkFinished, float &dt) {
    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - panelHeight - 50.f));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_AlwaysAutoResize | 
                                    ImGuiWindowFlags_NoSavedSettings | 
                                    ImGuiWindowFlags_NoFocusOnAppearing | 
                                    ImGuiWindowFlags_NoNav | 
                                    ImGuiWindowFlags_NoBackground;

    ImGui::Begin("Mode Overlay", nullptr, window_flags);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 100.f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f); 
        if (isStepByStep) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.4f, 0.0f, 1.0f)); 
            if (ImGui::Button("MODE: STEP-BY-STEP") && choosePrevNextButton == 0) {
                isStepByStep = false;
            }
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f)); 
            if (ImGui::Button("MODE: RUN ONCE") && checkFinished) {
                isStepByStep = true;
            }
            ImGui::PopStyleColor();
        }
    ImGui::PopStyleVar();  
    ImGui::PopStyleColor(); 
    ImGui::PopStyleVar();
    ImGui::End();
}


void drawSpeedController(sf::Vector2u windowSize, float panelHeight, bool checkFinished, float& speed, float barWidth = 200.0f, float minM = 0.0f, float maxM = 10.0f) {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                             ImGuiWindowFlags_NoBackground | 
                             ImGuiWindowFlags_AlwaysAutoResize | 
                             ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(ImVec2(300, windowSize.y - panelHeight - 76), ImGuiCond_Always); 
    ImGui::Begin("##SpeedOverlay", nullptr, flags);
        ImGui::SetWindowFontScale(1.1f);
        ImGui::TextColored(ImVec4(0.27f, 0.73f, 0.42f, 1.0f), "Speed: x%.1f", speed);
        ImGui::SetWindowFontScale(1.f);

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        
        float padding = 8.0f;           // khoang cach giua nen va thanh keo
        float barHeight = 6.0f;         // do day cua thanh keo
        float knobRadius = 9.0f;        // ban kinh cua num tron
        float totalBoxHeight = barHeight + (padding * 2);
        float totalBoxWidth = barWidth + (padding * 2);

        // the panel background
        draw_list->AddRectFilled(p, 
                                ImVec2(p.x + totalBoxWidth, p.y + totalBoxHeight), 
                                IM_COL32(15, 15, 15, 255), 10.0f); //  bo goc
        ImVec2 barStart = ImVec2(p.x + padding, p.y + padding + (barHeight / 2.0f) - 3.0f);
        float progress = (speed - minM) / (maxM - minM);
        float currentPos = barWidth * progress;

        // handle mouse click
        ImGui::SetCursorScreenPos(p);
        ImGui::InvisibleButton("##speed_hitbox", ImVec2(totalBoxWidth, totalBoxHeight));        
        if (ImGui::IsItemActive()) {
            float mouseX = ImGui::GetMousePos().x;
            float newProgress = (mouseX - (p.x + padding)) / barWidth;
            if (newProgress < 0.0f) newProgress = 0.0f;
            if (newProgress > 1.0f) newProgress = 1.0f;
            speed = minM + newProgress * (maxM - minM);
        }

        // thanh process
        draw_list->AddRectFilled(barStart, ImVec2(barStart.x + barWidth, barStart.y + barHeight), 
                                IM_COL32(70, 70, 70, 255), 3.0f);
        draw_list->AddRectFilled(barStart, ImVec2(barStart.x + currentPos, barStart.y + barHeight), 
                                IM_COL32(70, 185, 108, 255), 3.0f);

        // num tron
        ImVec2 knobCenter = ImVec2(barStart.x + currentPos, barStart.y + (barHeight / 2.0f));
        draw_list->AddCircleFilled(knobCenter, knobRadius, IM_COL32(255, 255, 255, 255));
        draw_list->AddCircle(knobCenter, knobRadius, IM_COL32(70, 185, 108, 255), 0, 2.0f);

    ImGui::End();
}



void drawProgress(sf::Vector2u windowSize, float panelHeight, bool checkFinished, float &dt, int currentIdx, int totalSteps, float width) {
    ImGui::SetNextWindowPos(ImVec2(586.f, windowSize.y - panelHeight - 50.f), ImGuiCond_Always);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |   
                         ImGuiWindowFlags_NoBackground |   
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoSavedSettings | 
                         ImGuiWindowFlags_NoFocusOnAppearing |
                         ImGuiWindowFlags_NoMove;          
    ImGui::Begin("##StandaloneButtons", nullptr, flags);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 0.8f));      
        ImGui::BeginDisabled(choosePrevNextButton != 0);
        if (ImGui::Button(" <  PREV ") && choosePrevNextButton == 0) {
            isStepByStep = true;
            choosePrevNextButton = -1;
        }
        ImGui::SameLine(); 
        if (ImGui::Button(" NEXT  > ") && choosePrevNextButton == 0) {
            isStepByStep = true;
            choosePrevNextButton = 1;
        }
        ImGui::EndDisabled();
        
        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(2);
    ImGui::End();

    if (totalSteps <= 0) return;
    ImGuiWindowFlags overlay_flags = ImGuiWindowFlags_NoTitleBar | 
                                    ImGuiWindowFlags_NoResize | 
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoScrollbar | 
                                    ImGuiWindowFlags_NoSavedSettings | 
                                    ImGuiWindowFlags_NoInputs | 
                                    ImGuiWindowFlags_NoBackground | 
                                    ImGuiWindowFlags_NoDecoration; 
    ImGui::SetNextWindowPos(ImVec2(888.f, windowSize.y - panelHeight - 44.f), ImGuiCond_Always);
    ImGui::Begin("##InvisibleOverlay", nullptr, overlay_flags);

        float progress = (totalSteps == 1 ? 0.0f : std::min(1.0f, (float)(currentIdx) / (float)(totalSteps - 1)));
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos(); 
        
        float height = 20.0f;       // Do cao cua khung
        float padding = 4.0f;       // Khoang cach giua khung den va thanh tien do
        float rounding = 5.0f;      // Bo goc

        draw_list->AddRectFilled(pos, 
                                ImVec2(pos.x + width, pos.y + height), 
                                IM_COL32(0, 0, 0, 255), 
                                rounding);
        if (progress > 0.0f) {
            float maxBarWidth = width - (padding * 2);
            float currentBarWidth = maxBarWidth * progress;
            
            ImVec2 barStart1 = ImVec2(pos.x + padding, pos.y + padding);
            ImVec2 barEnd1 = ImVec2(pos.x + padding + std::max(0.0f, currentBarWidth - maxBarWidth / (totalSteps - 1)), pos.y + height - padding);
            ImVec2 barStart2 = ImVec2(pos.x + padding + std::max(0.0f, currentBarWidth - maxBarWidth / (totalSteps - 1)), pos.y + padding);
            ImVec2 barEnd2 = ImVec2(pos.x + padding + currentBarWidth, pos.y + height - padding);
            
            draw_list->AddRectFilled(barStart1, 
                                    barEnd1, 
                                    IM_COL32(70, 185, 108, 255), 
                                    rounding - 1.0f);
            draw_list->AddRectFilled(barStart2, 
                                    barEnd2, 
                                    IM_COL32(255 , 165, 0, 255), 
                                    rounding - 1.0f);
        }
        else if(totalSteps == 1) {
            float maxBarWidth = width - (padding * 2);
            ImVec2 barStart = ImVec2(pos.x + padding, pos.y + padding);
            ImVec2 barEnd = ImVec2(pos.x + padding + maxBarWidth, pos.y + height - padding);
            draw_list->AddRectFilled(barStart, 
                                    barEnd, 
                                    IM_COL32(70, 185, 108, 255), 
                                    rounding - 1.0f);
        }

        if (totalSteps > 1) {
            for (int i = 1; i < totalSteps - 1; i++) {
                float x_offset = (width / (float)(totalSteps - 1)) * i;
                ImVec2 tickStart = ImVec2(pos.x + x_offset, pos.y);
                ImVec2 tickEnd = ImVec2(pos.x + x_offset, pos.y + height);
                draw_list->AddLine(tickStart, tickEnd, IM_COL32(255, 255, 255, 255), 1.0f);
            }
        }
        draw_list->AddRect(pos, ImVec2(pos.x + width, pos.y + height), IM_COL32(200, 200, 200, 50), rounding);
        ImGui::Dummy(ImVec2(width, height + 5.0f)); 

    ImGui::End();
}


void drawGlobalModeToggle(sf::Vector2u windowSize, float panelHeight, bool checkFinished, float &dt, int currentStepIdx = 2, int totalSteps = 3) {
    drawModeButton(windowSize, panelHeight, checkFinished, dt);    
    drawSpeedController(windowSize, panelHeight, checkFinished, dt, 250.f);
    drawProgress(windowSize, panelHeight, checkFinished, dt, currentStepIdx, totalSteps, 350.0f);
}

void drawVisualization1(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color &sfmlBgColor) {
    sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_AlwaysAutoResize | 
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("##OverlayMenu", nullptr, window_flags);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.1f)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.2f));
        if (ImGui::ImageButton("home", textures[0], sf::Vector2f(75, 75))) {
            appState = AppState::MAIN_MENU;
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::SameLine(0, 20); 
        if (isDarkMode && ImGui::ImageButton("dark_mode", textures[1], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());        
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        else if(!isDarkMode && ImGui::ImageButton("light_mode", textures[2], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::PopStyleColor(3);
    ImGui::End();


    sf::Vector2u windowSize = window.getSize();
    float currentControlWidth = windowSize.x;
    if(showCodePanel) currentControlWidth -= codePanelWidth;
    float panelHeight = 275.f;
    ImGui::GetIO().FontGlobalScale = 2.0f;
    drawGlobalModeToggle(windowSize, panelHeight, checkFinishedV1(), dtV1, currentStepIdxV1, scriptV1.size());
    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - panelHeight));
    ImGui::SetNextWindowSize(ImVec2(currentControlWidth, panelHeight));
    drawLog(window);
    ImGui::Begin("ControlPanel", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBringToFrontOnFocus); 

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.4f, 0.7f, 1.0f));


        // Cot 1
        bool temp = (isStepByStep || checkFinishedV1());
        ImGui::BeginChild("Operations", ImVec2(175, 0), true);
            if (ImGui::Selectable("Initialize", o == INITIALIZE)) { if(temp) o = INITIALIZE; }
            if (ImGui::Selectable("Insert(i,v)", o == ADD))    { if(temp) o = ADD;   }
            if (ImGui::Selectable("Remove(v)", o == DELETE))    { if(temp) o = DELETE; }
            if (ImGui::Selectable("Update(u,v)", o == UPDATE))    { if(temp) o = UPDATE; }
            if (ImGui::Selectable("Search(v)", o == SEARCH))    { if(temp) o = SEARCH; }
        ImGui::EndChild();

        ImGui::SameLine();

        // Cot 2
        ImGui::BeginGroup();

            
            if (o == INITIALIZE) initVisualization1(window);
            else if(o == ADD) insertVisualization1(window);
            else if(o == DELETE) deleteVisualization1(window);
            else if(o == UPDATE) updateVisualization1(window);
            else if(o == SEARCH) searchVisualization1(window);

            performVisualization1(window);
            if(checkFinishedV1()){
                drawLinkedList(window);
            }
            ImGui::EndGroup();

        ImGui::PopStyleColor();
    ImGui::End();
}



void drawVisualization2(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color &sfmlBgColor){
    sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_AlwaysAutoResize | 
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("##OverlayMenu", nullptr, window_flags);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.1f)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.2f));
        if (ImGui::ImageButton("home", textures[0], sf::Vector2f(75, 75))) {
            appState = AppState::MAIN_MENU;
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::SameLine(0, 20); 
        if (isDarkMode && ImGui::ImageButton("dark_mode", textures[1], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());        
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        else if(!isDarkMode && ImGui::ImageButton("light_mode", textures[2], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::PopStyleColor(3);
    ImGui::End();




    sf::Vector2u windowSize = window.getSize();
    float currentControlWidth = windowSize.x;
    if(showCodePanel) currentControlWidth -= codePanelWidth;
    float panelHeight = 275.f;
    ImGui::GetIO().FontGlobalScale = 2.0f;
    drawGlobalModeToggle(windowSize, panelHeight, checkFinishedV2(), dtV2, currentStepIdxV2, scriptV2.size());
    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - panelHeight));
    ImGui::SetNextWindowSize(ImVec2(currentControlWidth, panelHeight));
    

    drawLog(window);


    ImGui::Begin("ControlPanel", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBringToFrontOnFocus); 

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.4f, 0.7f, 1.0f));


        // Cot 1
        bool temp = (isStepByStep || checkFinishedV2());
        ImGui::BeginChild("Operations", ImVec2(175, 0), true);
            if (ImGui::Selectable("Initialize", o == INITIALIZE)) { if(temp) o = INITIALIZE; }
            if (ImGui::Selectable("Insert(i,v)", o == ADD))    { if(temp) o = ADD;   }
            if (ImGui::Selectable("Get Top", o == GET_TOP))    { if(temp) o = GET_TOP; }
            if (ImGui::Selectable("Update(u,v)", o == UPDATE))    { if(temp) o = UPDATE; }
        ImGui::EndChild();

        ImGui::SameLine();

        // Cot 2
        ImGui::BeginGroup();

            
            if (o == INITIALIZE) initVisualization2(window);
            else if(o == ADD) insertVisualization2(window);
            else if(o == GET_TOP) getTopVisualization2(window);
            else if(o == UPDATE) updateVisualization2(window);


            performVisualization2(window);

            if(checkFinishedV2()){
                drawHeapList(window, heapList);
            }
            ImGui::EndGroup();

        ImGui::PopStyleColor();
    ImGui::End();


}


void drawVisualization3(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color &sfmlBgColor){
    sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_AlwaysAutoResize | 
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("##OverlayMenu", nullptr, window_flags);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.1f)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.2f));
        if (ImGui::ImageButton("home", textures[0], sf::Vector2f(75, 75))) {
            appState = AppState::MAIN_MENU;
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::SameLine(0, 20); 
        if (isDarkMode && ImGui::ImageButton("dark_mode", textures[1], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());        
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        else if(!isDarkMode && ImGui::ImageButton("light_mode", textures[2], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::PopStyleColor(3);
    ImGui::End();




    sf::Vector2u windowSize = window.getSize();
    float currentControlWidth = windowSize.x;
    if(showCodePanel) currentControlWidth -= codePanelWidth;
    float panelHeight = 275.f;
    ImGui::GetIO().FontGlobalScale = 2.0f;
    drawGlobalModeToggle(windowSize, panelHeight, checkFinishedV3(), dtV3, currentStepIdxV3, scriptV3.size());
    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - panelHeight));
    ImGui::SetNextWindowSize(ImVec2(currentControlWidth, panelHeight));


    drawLog(window);

    ImGui::Begin("ControlPanel", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBringToFrontOnFocus); 

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.4f, 0.7f, 1.0f));


        // Cot 1
        bool temp = (isStepByStep || checkFinishedV3());
        ImGui::BeginChild("Operations", ImVec2(175, 0), true);
            if (ImGui::Selectable("Initialize", o == INITIALIZE)) { if(temp) o = INITIALIZE; }
            if (ImGui::Selectable("Insert(v)", o == ADD))    { if(temp) o = ADD;   }
            if (ImGui::Selectable("Remove(v)", o == DELETE))    { if(temp) o = DELETE; }
            if (ImGui::Selectable("Search(v)", o == SEARCH))    { if(temp) o = SEARCH; }
        ImGui::EndChild();

        ImGui::SameLine();

        // Cot 2
        ImGui::BeginGroup();
            
            if (o == INITIALIZE) initVisualization3(window);
            else if(o == ADD) insertVisualization3(window);
            else if(o == DELETE) deleteVisualization3(window);
            else if(o == SEARCH) searchVisualization3(window);


            performVisualization3(window);
            
            if(checkFinishedV3()){
                drawAVLTree(rootV3, window);
            }
            ImGui::EndGroup();

        ImGui::PopStyleColor();
    ImGui::End();
}

void drawVisualization4(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color &sfmlBgColor){
    sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_AlwaysAutoResize | 
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("##OverlayMenu", nullptr, window_flags);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.1f)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.2f));
        if (ImGui::ImageButton("home", textures[0], sf::Vector2f(75, 75))) {
            appState = AppState::MAIN_MENU;
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::SameLine(0, 20); 
        if (isDarkMode && ImGui::ImageButton("dark_mode", textures[1], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());        
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        else if(!isDarkMode && ImGui::ImageButton("light_mode", textures[2], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::PopStyleColor(3);
    ImGui::End();


    
    sf::Vector2u windowSize = window.getSize();
    float currentControlWidth = windowSize.x;
    if(showCodePanel) currentControlWidth -= codePanelWidth;
    float panelHeight = 275.f;
    ImGui::GetIO().FontGlobalScale = 2.0f;
    drawGlobalModeToggle(windowSize, panelHeight, checkFinishedV4(), dtV4, currentStepIdxV4, scriptV4.size());
    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - panelHeight));
    ImGui::SetNextWindowSize(ImVec2(currentControlWidth, panelHeight));


    drawLog(window);

    ImGui::Begin("ControlPanel", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBringToFrontOnFocus); 

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.4f, 0.7f, 1.0f));


        // Cot 1
        bool temp = (isStepByStep || checkFinishedV4());
        std::cout << "check: " << temp << "\n";
        ImGui::BeginChild("Operations", ImVec2(175, 0), true);
            if (ImGui::Selectable("Initialize", o == INITIALIZE)) { if(temp) o = INITIALIZE; }
            if (ImGui::Selectable("Insert(v)", o == ADD))    { if(temp) o = ADD;   }
            if (ImGui::Selectable("Remove(v)", o == DELETE))    { if(temp) o = DELETE; }
            if (ImGui::Selectable("Search(v)", o == SEARCH))    { if(temp) o = SEARCH; }
            ImGui::EndChild();

        ImGui::SameLine();

        // Cot 2
        ImGui::BeginGroup();
            
            if (o == INITIALIZE) initVisualization4(window);
            else if(o == ADD) insertVisualization4(window);
            else if(o == DELETE) deleteVisualization4(window);
            else if(o == SEARCH) searchVisualization4(window);
            
            performVisualization4(window);
            
            if(checkFinishedV4()){
                drawTrie(rootV4, window, rootV4);
            }
            ImGui::EndGroup();

        ImGui::PopStyleColor();
    ImGui::End();
}


void drawVisualization5(sf::RenderWindow& window, std::vector<sf::Texture>& textures, sf::Color& sfmlBgColor){
    sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_AlwaysAutoResize | 
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("##OverlayMenu", nullptr, window_flags);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.1f)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.2f));
        if (ImGui::ImageButton("home", textures[0], sf::Vector2f(75, 75))) {
            appState = AppState::MAIN_MENU;
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::SameLine(0, 20);
        if (ImGui::ImageButton("dark_mode", textures[1], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        else if(!isDarkMode && ImGui::ImageButton("light_mode", textures[2], sf::Vector2f(67, 67))) {
            isDarkMode = !isDarkMode;
            sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
            ImGui::PopStyleColor(3);
            ImGui::End();
            return;
        }
        ImGui::PopStyleColor(3);
    ImGui::End();

    sf::Vector2u windowSize = window.getSize();
    float currentControlWidth = windowSize.x;
    if(showCodePanel) currentControlWidth -= codePanelWidth;
    float panelHeight = 275.f;
    ImGui::GetIO().FontGlobalScale = 2.0f;
    drawGlobalModeToggle(windowSize, panelHeight, checkFinishedV5(), dtV5, currentStepIdxV5, scriptV5.size());
    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - panelHeight));
    ImGui::SetNextWindowSize(ImVec2(currentControlWidth, panelHeight));


    drawLog(window);
    graphPhysics.drawBounds(window);
    ImGui::SetNextWindowPos(ImVec2(0, window.getSize().y - panelHeight));
    ImGui::SetNextWindowSize(ImVec2(currentControlWidth, panelHeight));
    ImGui::Begin("ControlPanel", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBringToFrontOnFocus);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f));
        ImGui::BeginChild("Operations", ImVec2(175, 0), true);
            if (ImGui::Selectable("Initialize", o == INITIALIZE)) { if(checkFinishedV5()) o = INITIALIZE; }
            if (ImGui::Selectable("Dijkstra(s)", o ==GRAPH ))    { if(checkFinishedV5()) o = GRAPH; }
            ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginGroup();
            if (o == INITIALIZE) initVisualization5(window);
            else if(o == GRAPH) dijkstraVisualization5(window);
            
            
            performVisualization5(window);
            
            if(checkFinishedV5()){
                drawDijkstra(graphPhysics.getNodes(), window);
            }
            ImGui::EndGroup();
        ImGui::PopStyleColor();
    ImGui::End();
}
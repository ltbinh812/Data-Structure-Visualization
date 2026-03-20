#include <iostream>
#include <vector>
#include "draw.h"
#include "main.h"
#include "entity.h"

#include "visualization1.h"
#include "performVisualization1.h"

Block* Log = nullptr;
float delayLog = 0;

OperationType o = INITIALIZE;

void setLog(std::string text, sf::Color color){
    Log = new Block(RECTANGLE, 500.f, 40.f, text);
    Log -> setFillColor(color);
    Log -> currentPosition = {-300.f, WINDOW_HEIGHT - 400.f};
    Log -> targetPosition = {300.f, WINDOW_HEIGHT - 400.f};
    Log -> setPosition(Log->currentPosition);
    delayLog = 0;
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


void drawVisualization1(sf::RenderWindow& window){
    sf::Vector2u windowSize = window.getSize();
    float panelHeight = 350.f;
    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - panelHeight));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, panelHeight));
    ImGui::GetIO().FontGlobalScale = 2.0f;
    

    drawLog(window);



    ImGui::Begin("ControlPanel", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBringToFrontOnFocus); 

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.4f, 0.7f, 1.0f));


        // Cot 1
        bool temp = (currentStepIdx + 1 == script.size());
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


            performInsertVisualization1(window);
            // else if(o == DELETE) performDeleteVisualization1(window);
            // else if(o == UPDATE) performUpdateVisualization1(window);
            // else if(o == SEARCH) performSearchVisualization1(window);
            
            if(currentStepIdx + 1 == script.size()){
                drawList(window);
                if(linkedList.size() > 0 && script[currentStepIdx].focusNodeIdx != -1)
                    drawPointer(window, linkedList[script[currentStepIdx].focusNodeIdx] -> center() + sf::Vector2f(0.f, -50.f), "pointer/" + std::to_string(script[currentStepIdx].focusNodeIdx));
            }
            ImGui::EndGroup();

        ImGui::PopStyleColor();
    ImGui::End();
}
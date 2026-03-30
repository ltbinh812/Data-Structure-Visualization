#define SKIP_EXTERN_script
#include <iostream>
#include <math.h>
#include <vector>
#include "draw.h"
#include "main.h"
#include "entity.h"

#include "visualization1.h"
#include "performVisualization1.h"
#include "visualization2.h"
#include "performVisualization2.h"
#include "visualization3.h"
#include "performVisualization3.h"
#include "visualization4.h"
#include "performVisualization4.h"

Block* Log = nullptr;
float delayLog = 0;
OperationType o = INITIALIZE;
Block* newNode = nullptr;

void setLog(std::string text, sf::Color color, sf::Color textColor) {
    Log = new Block(RECTANGLE, 500.f, 40.f, text);
    Log -> setFillColor(color);
    Log -> setFillColorText(textColor);
    Log -> currentPosition = {-300.f, WINDOW_HEIGHT - 400.f};
    Log -> targetPosition = {300.f, WINDOW_HEIGHT - 400.f};
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
            Log->move(8.0f);
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
        bool temp = checkFinishedV1();
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



void drawVisualization2(sf::RenderWindow& window){
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
        bool temp = checkFinishedV2();
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
                drawHeapList(window);
            }
            ImGui::EndGroup();

        ImGui::PopStyleColor();
    ImGui::End();


}


void drawVisualization3(sf::RenderWindow& window){
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
        bool temp = checkFinishedV3();
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

void drawVisualization4(sf::RenderWindow& window){
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
        bool temp = checkFinishedV4();
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
                drawTrie(rootV4, window);
            }
            ImGui::EndGroup();

        ImGui::PopStyleColor();
    ImGui::End();
}
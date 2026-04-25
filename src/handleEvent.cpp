#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "handleEvent.h"
#include "main.h"
#include "draw.h"
#include "theme.h"
#include "visualization1.h"
#include "visualization2.h"
#include "visualization3.h"
#include "visualization4.h"
#include "visualization5.h"
#include "visualization6.h"

void handleMainMenuEvents(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView, std::vector<Card>& cards){
    for(auto &card: cards){
        card.handleEvent(event, window, cameraView);
    }

    if(cards[0].clicked(event, window, cameraView)){
        window.setView(window.getDefaultView());
        initStatus1();
        appState = AppState::VISUALIZATION1;
    }
    if(cards[1].clicked(event, window, cameraView)){
        window.setView(window.getDefaultView());
        initStatus2();
        appState = AppState::VISUALIZATION2;
    }
    if(cards[2].clicked(event, window, cameraView)){
        window.setView(window.getDefaultView());
        initStatus3();
        appState = AppState::VISUALIZATION3;
    }
    if(cards[3].clicked(event, window, cameraView)){
        window.setView(window.getDefaultView());
        initStatus4();
        appState = AppState::VISUALIZATION4;
    }
    if(cards[4].clicked(event, window, cameraView)){
        window.setView(window.getDefaultView());
        initStatus5();
        appState = AppState::VISUALIZATION5;
    }
    if(cards[5].clicked(event, window, cameraView)){
        window.setView(window.getDefaultView());
        initStatus6();
        appState = AppState::VISUALIZATION6;
    }
}

void handleVisualization1Events(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView, std::vector<sf::Texture>& textures){
}

void handleVisualization2Events(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView, std::vector<sf::Texture>& textures){
}

void handleVisualization3Events(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView, std::vector<sf::Texture>& textures){
}

void handleVisualization4Events(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView, std::vector<sf::Texture>& textures){
}

void handleVisualization5Events(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView, std::vector<sf::Texture>& textures){

}

void handleVisualization6Events(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView, std::vector<sf::Texture>& textures){
 
}
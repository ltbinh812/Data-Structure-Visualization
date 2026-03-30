#include <iostream>
#include "handleEvent.h"
#include "main.h"
#include "draw.h"
#include "visualization1.h"
#include "visualization2.h"
#include "visualization3.h"
#include "visualization4.h"

void handleMainMenuEvents(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView, std::vector<Card>& cards){
    for(auto &card: cards){
        card.handleEvent(event, window, cameraView);
    }

    if(cards[0].clicked(event, window, cameraView)){
        initStatus1();
        appState = AppState::VISUALIZATION1;
    }
    if(cards[1].clicked(event, window, cameraView)){
        initStatus2();
        appState = AppState::VISUALIZATION2;
    }
    if(cards[2].clicked(event, window, cameraView)){
        initStatus3();
        appState = AppState::VISUALIZATION3;
    }
    if(cards[3].clicked(event, window, cameraView)){
        initStatus4();
        std::cout << "???????????????????\n";
        appState = AppState::VISUALIZATION4;
    }
    if(cards[4].clicked(event, window, cameraView)){
        o = INITIALIZE;
        appState = AppState::VISUALIZATION5;
    }
    if(cards[5].clicked(event, window, cameraView)){
        o = INITIALIZE;
        appState = AppState::VISUALIZATION6;
    }
}


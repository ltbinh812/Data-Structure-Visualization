#include <iostream>
#include "handleEvent.h"




void handleMainMenuEvents(sf::Event event, sf::RenderWindow& window, sf::View& cameraView, std::vector<Card>& cards){
    for(auto &card: cards){
        card.handleEvent(event, window, cameraView);
    }
}


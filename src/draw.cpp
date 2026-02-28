#include <iostream>
#include <vector>
#include "draw.h"
#include "main.h"
#include "entity.h"


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
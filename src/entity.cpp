#include <iostream>
#include <sstream>
#include "entity.h"
#include <SFML/Graphics.hpp>
#include "main.h"  



Text::Text(std::string string, Style &style)
    : text(style.font, string, style.characterSize)
{
    text.setFillColor(style.fillColor);
    text.setOrigin({text.getLocalBounds().position.x + text.getLocalBounds().size.x / 2.f, text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2.f});
}

void Text::setPosition(float x, float y){
    text.setPosition({x, y});
}

void Text::setPosition(sf::Vector2f pos){
    text.setPosition(pos);
}

void Text::setString(std::string string){
    text.setString(string);
    text.setOrigin({text.getLocalBounds().position.x + text.getLocalBounds().size.x / 2.f, text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2.f});
}

void Text::align(int idx){
    if (idx == 0) text.setOrigin({text.getLocalBounds().position.x, text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2.f});
    else if (idx == 1) text.setOrigin({text.getLocalBounds().position.x + text.getLocalBounds().size.x / 2.f, text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2.f});
    else if (idx == 2) text.setOrigin({text.getLocalBounds().position.x + text.getLocalBounds().size.x, text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2.f});
}

float Text::getWidth(){
    return text.getLocalBounds().size.x;
}

std::string Text::wrapText(const std::string& str, const sf::Font& font, unsigned int charSize, float maxWidth) {
    std::stringstream ss(str);
    std::string word;
    std::string result = "";
    std::string currentLine = "";
    std::string testLine = "";

    while (ss >> word) {
        testLine = "";
        if (currentLine.empty()) testLine = word;
        else testLine = currentLine + " " + word;
        

        sf::Text testObject(font, testLine, charSize);
        float currentWidth = testObject.getGlobalBounds().size.x;

        if (currentWidth < maxWidth)
            currentLine = testLine;
        else{
            if (!currentLine.empty())
                result += currentLine + "\n";
            currentLine = word;
        }
    }
    result += currentLine;
    return result;
}

sf::Color Text::getFillColor(){
    return text.getFillColor();
}

void Text::setFillColor(sf::Color color){
    text.setFillColor(color);
}

void Text::draw(sf::RenderWindow& window){
    window.draw(text);
}

Block::Block(ShapeType shape, float radius, sf::Color color){
    this->shape = shape;
    circle.setRadius(radius);
    circle.setFillColor(color);
    circle.setOrigin(sf::Vector2f(radius, radius));
}
Block::Block(ShapeType shape, float width, float height, sf::Color color){
    this->shape = shape;
    rectangle.setSize({width, height});
    rectangle.setFillColor(color);
    rectangle.setOrigin({width / 2.f, height / 2.f});
}

void Block::setoutline(int thickness, sf::Color color){
    if (shape == CIRCLE){
        circle.setOutlineThickness(thickness);
        circle.setOutlineColor(color);
    }
    else if (shape == RECTANGLE){
        rectangle.setOutlineThickness(thickness);
        rectangle.setOutlineColor(color);
    }
}


void Block::setPosition(float x, float y){
    if(shape == CIRCLE) circle.setPosition(sf::Vector2f(x, y));
    else if(shape == RECTANGLE) rectangle.setPosition(sf::Vector2f(x, y));
}

void Block::setPosition(sf::Vector2f pos){
    if(shape == CIRCLE) circle.setPosition(pos);
    else if(shape == RECTANGLE) rectangle.setPosition(pos);
}

sf::Vector2f Block::center(){
    if(shape == CIRCLE) return circle.getPosition();
    else if(shape == RECTANGLE) return rectangle.getPosition();
    return {0.f, 0.f};
}

void Block::initText(std::string string, Style &style){
    textLabel.emplace(string, style);
    
    if(shape == CIRCLE) textLabel->setString(textLabel->wrapText(string, style.font, style.characterSize, circle.getRadius() * 1.5f));
    else if(shape == RECTANGLE) textLabel->setString(textLabel->wrapText(string, style.font, style.characterSize, rectangle.getSize().x * 0.9f));
    textLabel->setPosition(center());
}   

void Block::align(int idx){
    if(shape == CIRCLE){
        if(idx == 0) circle.setOrigin({0.f, circle.getRadius()});
        else if(idx == 1) circle.setOrigin({circle.getRadius(), circle.getRadius()});
        else if(idx == 2) circle.setOrigin({circle.getRadius() * 2.f, circle.getRadius()});
    }
    else if(shape == RECTANGLE){
        if(idx == 0) rectangle.setOrigin({0.f, rectangle.getSize().y / 2.f});
        else if(idx == 1) rectangle.setOrigin({rectangle.getSize().x / 2.f, rectangle.getSize().y / 2.f});
        else if(idx == 2) rectangle.setOrigin({rectangle.getSize().x, rectangle.getSize().y / 2.f});
    }
}

sf::FloatRect Block::getGlobalBounds(){
    if(shape == CIRCLE) return circle.getGlobalBounds();
    else if(shape == RECTANGLE) return rectangle.getGlobalBounds();
    return sf::FloatRect();
}

sf::Color Block::getFillColor(){
    if(shape == CIRCLE) return circle.getFillColor();
    else if(shape == RECTANGLE) return rectangle.getFillColor();
    return sf::Color::Transparent;
}

void Block::setFillColor(sf::Color color){
    if(shape == CIRCLE) circle.setFillColor(color);
    else if(shape == RECTANGLE) rectangle.setFillColor(color);
}

void Block::draw(sf::RenderWindow& window){
    if(shape == CIRCLE) window.draw(circle);
    else if(shape == RECTANGLE) window.draw(rectangle);

    if(textLabel) textLabel->draw(window);
}



Image::Image(sf::Texture& texture, float width, float height){
    sprite.emplace(texture);
    sf::Vector2f scaleFactors(width / texture.getSize().x, height / texture.getSize().y);
    sprite->setScale(scaleFactors);
    sprite->setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
}

void Image::setPosition(sf::Vector2f pos){
    sprite->setPosition(pos);
}

void Image::setPosition(float x, float y){
    sprite->setPosition({x, y});
}

sf::Color Image::getFillColor(){
    if (sprite) return sprite->getColor();
    else return sf::Color::Transparent;
}

void Image::setFillColor(sf::Color color){
    if (sprite) sprite->setColor(color);
}

void Image::draw(sf::RenderWindow& window){
    if (sprite.has_value()) {
        window.draw(*sprite);
    }
}


Card::Card(sf::Texture& texture, std::string title, std::string tabName, sf::Vector2f pos)
    : background(RECTANGLE, 475, 400, sf::Color(255, 255, 255, 255)),
      image(texture, 425, 250),
      titleText(title, style2),
      titleBg(RECTANGLE, 425, 50, sf::Color(128, 128, 128, 255)),
      tabText(tabName, style3),
      tabBg(RECTANGLE, tabText.getWidth() + 30.f, 30, sf::Color(46, 188, 210, 255))
{
    background.setPosition(pos);
    image.setPosition(pos.x, pos.y - 50.f);
    
    titleText.align(0);
    titleText.setPosition(pos.x - 212.5f + 15.f, pos.y + 160.f);
    
    titleBg.align(0);
    titleBg.setPosition(pos.x - 212.5f, pos.y + 160.f);
    
    tabText.align(0);
    tabText.setPosition(pos.x - 212.5f + 15.f, pos.y + 110.f);
    
    tabBg.align(0);
    tabBg.setPosition(pos.x - 212.5f, pos.y + 110.f);
}

void Card::handleEvent(sf::Event& event, sf::RenderWindow& window, sf::View& cameraView){
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), cameraView);    
    sf::FloatRect cardBounds = background.getGlobalBounds();
    if (cardBounds.contains(mousePos)) {
        if(!background.isHovered){
            background.isHovered = true;
            
            background.setoutline(10, sf::Color(188, 188, 188, 255));
            background.setFillColor(background.getFillColor() - sf::Color(0, 0, 0, 50));
            titleBg.setFillColor(titleBg.getFillColor() - sf::Color(0, 0, 0, 50));
            tabBg.setFillColor(tabBg.getFillColor() - sf::Color(0, 0, 0, 50));

            titleText.setFillColor(titleText.getFillColor() - sf::Color(0, 0, 0, 50));
            tabText.setFillColor(tabText.getFillColor() - sf::Color(0, 0, 0, 50));

            image.setFillColor(image.getFillColor() - sf::Color(0, 0, 0, 50));
        }
    }
    else{
        if(background.isHovered){
            background.isHovered = false;
            
            background.setoutline(0, sf::Color::Transparent);
            background.setFillColor(background.getFillColor() + sf::Color(0, 0, 0, 50));
            titleBg.setFillColor(titleBg.getFillColor() + sf::Color(0, 0, 0, 50));
            tabBg.setFillColor(tabBg.getFillColor() + sf::Color(0, 0, 0, 50));

            titleText.setFillColor(titleText.getFillColor() + sf::Color(0, 0, 0, 50));
            tabText.setFillColor(tabText.getFillColor() + sf::Color(0, 0, 0, 50));

            image.setFillColor(image.getFillColor() + sf::Color(0, 0, 0, 50));
        }
    }
}
void Card::draw(sf::RenderWindow& window){
    background.draw(window);
    image.draw(window);
    tabBg.draw(window);
    tabText.draw(window);
    titleBg.draw(window);
    titleText.draw(window);
}



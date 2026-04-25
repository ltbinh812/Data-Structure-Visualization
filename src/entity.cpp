#include <iostream>
#include <sstream>
#include <math.h>
#include "entity.h"
#include <SFML/Graphics.hpp>
#include "main.h"  
#include "cmath"
#include "theme.h"

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

std::string Text::getString(){
    return text.getString();
}

void Text::align(int idx){
    if (idx == 0) text.setOrigin({text.getLocalBounds().position.x, text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2.f});
    else if (idx == 1) text.setOrigin({text.getLocalBounds().position.x + text.getLocalBounds().size.x / 2.f, text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2.f});
    else if (idx == 2) text.setOrigin({text.getLocalBounds().position.x + text.getLocalBounds().size.x, text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2.f});
}

float Text::getWidth(){
    return text.getLocalBounds().size.x;
}

sf::FloatRect Text::getGlobalBounds(){
    return text.getGlobalBounds();
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
        float currentWidth = testObject.getLocalBounds().size.x;

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

Block::Block(ShapeType shape, float width, float height, std::string label){
    this->shape = shape;
    rectangle.setSize({width, height});
    rectangle.setFillColor(sf::Color::White);
    setoutline(5, sf::Color::Black);
    rectangle.setOrigin({width / 2.f, height / 2.f});
    initText(label, style5);
}

Block::Block(ShapeType shape, float radius, std::string label){
    this->shape = shape;
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::White);
    setoutline(5, sf::Color::Black);
    circle.setOrigin(sf::Vector2f(radius, radius));
    initText(label, style4);
}

void Block::setText(std::string label){
    initText(label, style4);
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
    else if(shape == NODE){
        circle.setOutlineThickness(thickness);
        circle.setOutlineColor(color);
    }
}


void Block::setPosition(float x, float y){
    if(shape == CIRCLE) circle.setPosition(sf::Vector2f(x, y));
    else if(shape == RECTANGLE) rectangle.setPosition(sf::Vector2f(x, y));
    else if(shape == NODE) circle.setPosition(sf::Vector2f(x, y));
    if(textLabel) textLabel->setPosition(center());
}

void Block::setPosition(sf::Vector2f pos){
    if(shape == CIRCLE) circle.setPosition(pos);
    else if(shape == RECTANGLE) rectangle.setPosition(pos);
    else if(shape == NODE) circle.setPosition(pos);
    if(textLabel) textLabel->setPosition(center());
}

void Block::setSize(sf::Vector2f rect){
    rectangle.setSize(rect);
    rectangle.setOrigin({rect.x / 2.f, rect.y / 2.f});
}

void Block::setColor(sf::Color color){
    if(shape == CIRCLE) circle.setOutlineColor(color);
    else if(shape == RECTANGLE) rectangle.setOutlineColor(color);
    else if(shape == NODE) circle.setOutlineColor(color);
}

std::pair<float, float> Block::getPosition(){
    if(shape == CIRCLE) return {circle.getPosition().x, circle.getPosition().y};
    else if(shape == RECTANGLE) return {rectangle.getPosition().x, rectangle.getPosition().y};
    else if(shape == NODE) return {circle.getPosition().x, circle.getPosition().y};
    return {0.f, 0.f};
}

sf::Vector2f Block::center(){
    if(shape == CIRCLE) return circle.getPosition();
    else if(shape == RECTANGLE) return rectangle.getPosition();
    else if(shape == NODE) return circle.getPosition();
    return {0.f, 0.f};
}


float Block::getRadius(){
    if(shape == CIRCLE) return circle.getRadius();
    else if(shape == RECTANGLE) return rectangle.getSize().x / 2.f;
    else if(shape == NODE) return circle.getRadius();
    return 0.f;
}

void Block::initText(std::string string, Style &style){
    textLabel = std::nullopt;
    textLabel.emplace(string, style);
    
    if(shape == CIRCLE) textLabel->setString(textLabel->wrapText(string, style.font, style.characterSize, circle.getRadius() * 1.5f));
    else if(shape == RECTANGLE) textLabel->setString(textLabel->wrapText(string, style.font, style.characterSize, rectangle.getSize().x * 0.9f));
    else if(shape == NODE) textLabel->setString(textLabel->wrapText(string, style.font, style.characterSize, circle.getRadius() * 1.5f));
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
    else if(shape == NODE){
        if(idx == 0) circle.setOrigin({0.f, circle.getRadius()});
        else if(idx == 1) circle.setOrigin({circle.getRadius(), circle.getRadius()});
        else if(idx == 2) circle.setOrigin({circle.getRadius() * 2.f, circle.getRadius()});
    }
}

sf::FloatRect Block::getGlobalBounds(){
    if(shape == CIRCLE) return circle.getGlobalBounds();
    else if(shape == RECTANGLE) return rectangle.getGlobalBounds();
    else if(shape == NODE) return circle.getGlobalBounds();
    return sf::FloatRect();
}

sf::Color Block::getFillColor(){
    if(shape == CIRCLE) return circle.getFillColor();
    else if(shape == RECTANGLE) return rectangle.getFillColor();
    else if(shape == NODE) return circle.getFillColor();
    return sf::Color::Transparent;
}

void Block::setFillColor(sf::Color color){
    if(shape == CIRCLE) circle.setFillColor(color);
    else if(shape == RECTANGLE) rectangle.setFillColor(color);
    else if(shape == NODE) circle.setFillColor(color);
}

void Block::setFillColorText(sf::Color color){
    if(textLabel) textLabel->setFillColor(color);
}

void Block::move(float dt, bool isStepByStep){
    if(isStepByStep){
        currentPosition = targetPosition;
        setPosition(currentPosition);
    }
    else{
        sf::Vector2f direction = targetPosition - currentPosition;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > eps) {
            sf::Vector2f normalizedDirection = direction / distance;
            sf::Vector2f movement = normalizedDirection * moveSpeed * dealtaTime.asSeconds() * dt;

            if (std::sqrt(movement.x * movement.x + movement.y * movement.y) > distance) {
                currentPosition = targetPosition;
            } else {
                currentPosition += movement;
            }
            setPosition(currentPosition);
        }
    }
    if(textLabel) textLabel->setPosition(center());
}

bool Block::isMoving(){
    sf::Vector2f diff = targetPosition - currentPosition;
    float distSq = diff.x * diff.x + diff.y * diff.y;
    return distSq > 0.1f;
}

void Block::draw(sf::RenderWindow& window){
    setColor(outlineColor);

    if(shape == CIRCLE) window.draw(circle);
    else if(shape == RECTANGLE) window.draw(rectangle);
    else if(shape == NODE) window.draw(circle);

    if(textLabel) textLabel->draw(window);
}

std::string Block::getLabel(){
    if(textLabel) return textLabel->getString();
    else return "";
}

Image::Image(sf::Texture& texture, float width, float height){
    sprite.emplace(texture);
    sf::Vector2f scaleFactors(width / texture.getSize().x, height / texture.getSize().y);
    sprite->setScale(scaleFactors);
    sprite->setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
}

Image::Image(sf::Texture& texture, float width, float height, float x, float y){
    sprite.emplace(texture);
    sf::Vector2f scaleFactors(width / texture.getSize().x, height / texture.getSize().y);
    sprite->setScale(scaleFactors);
    sprite->setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
    sprite->setPosition({x, y});
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

void Card::updateTheme(bool isDark) {
    sf::Color baseBg, baseOutline, baseTitleBg, baseTitleText;
    
    if (isDark) {
        baseBg = sf::Color(20, 30, 50, 255);
        baseOutline = sf::Color(0, 191, 255, 255);
        baseTitleBg = sf::Color(30, 45, 75, 255);
        baseTitleText = sf::Color(240, 248, 255, 255);
    } else {
        baseBg = sf::Color(255, 255, 255, 255);
        baseOutline = sf::Color(173, 216, 230, 255);
        baseTitleBg = sf::Color(230, 242, 255, 255);
        baseTitleText = sf::Color(10, 40, 80, 255);
    }

    sf::Color baseTabBg = sf::Color(46, 188, 210, 255);
    sf::Color baseTabText = style3.fillColor; 
    sf::Color baseImage = sf::Color::White;

    if (background.isHovered) {
        background.setoutline(10, sf::Color(188, 188, 188, 255));
        background.setFillColor(baseBg - sf::Color(0, 0, 0, 50));
        titleBg.setFillColor(baseTitleBg - sf::Color(0, 0, 0, 50));
        titleText.setFillColor(baseTitleText - sf::Color(0, 0, 0, 50));
        
        tabBg.setFillColor(baseTabBg - sf::Color(0, 0, 0, 50));
        tabText.setFillColor(baseTabText - sf::Color(0, 0, 0, 50));
        image.setFillColor(baseImage - sf::Color(0, 0, 0, 50));
    } else {
        background.setoutline(2, baseOutline);
        background.setFillColor(baseBg);
        titleBg.setFillColor(baseTitleBg);
        titleText.setFillColor(baseTitleText);
        
        tabBg.setFillColor(baseTabBg);
        tabText.setFillColor(baseTabText);
        image.setFillColor(baseImage);
    }
}

void Card::handleEvent(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView){
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), cameraView);    
    sf::FloatRect cardBounds = background.getGlobalBounds();
    
    if (cardBounds.contains(mousePos)) {
        background.isHovered = true;
    } else {
        background.isHovered = false;
    }
}

bool Card::clicked(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView) {
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseBtn->position, cameraView);

            if (background.getGlobalBounds().contains(mouseWorldPos)) {
                return true;
            }
        }
    }
    
    return false; 
}

void Card::draw(sf::RenderWindow& window){
    background.draw(window);
    image.draw(window);
    tabBg.draw(window);
    tabText.draw(window);
    titleBg.draw(window);
    titleText.draw(window);
}



void Notification::trigger(std::string text){
    if (message == nullptr) {
        message = new sf::Text(style5.font, text, style5.characterSize);
    } else {
        message->setString(text);
    }

    auto textRect = message->getLocalBounds();
    float paddingX = 60.f; 
    float paddingY = 30.f;
    box.setSize({ textRect.size.x + paddingX, textRect.size.y + paddingY });

    box.setFillColor(sf::Color(255, 230, 200, 255)); 
    box.setOutlineColor(sf::Color(200, 0, 0));      
    box.setOutlineThickness(3.f);

    box.setOrigin(box.getLocalBounds().position + box.getLocalBounds().size / 2.f);
    message->setOrigin(message->getLocalBounds().position + message->getLocalBounds().size / 2.f);

    float margin = 20.f;
    visiblePos = { (box.getSize().x / 2.f) + margin, 
                   WINDOW_HEIGHT - (box.getSize().y / 2.f) - margin -  350.f };
    
    hiddenPos = { - (box.getSize().x / 2.f) - margin - 10.f, visiblePos.y };

    if (state == NotifyState::HIDDEN) {
        currentPos = hiddenPos;
        box.setPosition(currentPos);
    }
    
    state = NotifyState::SLIDING_IN;
    timer = 0.0f; 
}

void Notification::draw(sf::RenderWindow& window) {
    if (state != NotifyState::HIDDEN) {
        window.draw(box);
        if (message) window.draw(*message); 
    }
}



void drawArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, sf::Color arrowColor, bool drawArrowHead, float thickness) {
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length <= 1e-4) return; 

    sf::RectangleShape line(sf::Vector2f{length, thickness});
    line.setOrigin(sf::Vector2f{0.f, thickness / 2.f}); 
    line.setPosition(start);
    line.setFillColor(arrowColor);
    line.setRotation(sf::radians(std::atan2(direction.y, direction.x)));
    window.draw(line);

    
    if (drawArrowHead) {
        const float arrowSize = 10.f + (thickness * 1.5f); 
        sf::Vector2f unitDirection = direction / length;
        sf::Vector2f perpendicular{-unitDirection.y, unitDirection.x}; 
        sf::Vertex arrowHead[3];
        arrowHead[0].position = end;
        arrowHead[0].color = arrowColor;
        arrowHead[1].position = end - unitDirection * arrowSize + perpendicular * (arrowSize / 2.f);
        arrowHead[1].color = arrowColor;
        arrowHead[2].position = end - unitDirection * arrowSize - perpendicular * (arrowSize / 2.f);
        arrowHead[2].color = arrowColor;
        window.draw(arrowHead, 3, sf::PrimitiveType::Triangles);
    }
}

void drawPointer(sf::RenderWindow& window, sf::Vector2f Pos, std::string label) {
    Text text(label, style4);
    text.setFillColor(sf::Color::Red);
    text.setPosition(Pos);
    text.draw(window);
}
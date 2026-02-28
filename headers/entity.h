#pragma once
#include <iostream>
#include <optional>
#include <SFML/Graphics.hpp>


enum ShapeType {
    CIRCLE,
    RECTANGLE
};

struct Style{
    sf::Font font;
    unsigned int characterSize;
    sf::Color fillColor;
    int outlineThickness;
    sf::Color outlineColor;
    Style() = default;
    Style(const sf::Font& font, unsigned int characterSize, sf::Color fillColor, int outlineThickness = 0, sf::Color outlineColor = sf::Color::Transparent)
        : font(font), characterSize(characterSize), fillColor(fillColor), outlineThickness(outlineThickness), outlineColor(outlineColor) {}
};

class Text{
private:
    sf::Text text;

public:
    Text() = default;
    Text(std::string string, Style &style);
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f pos);
    void setString(std::string string);
    void align(int idx);
    float getWidth();
    sf::Color getFillColor();
    void setFillColor(sf::Color color);
    std::string wrapText(const std::string& str, const sf::Font& font, unsigned int charSize, float maxWidth);
    void draw(sf::RenderWindow& window);
    ~Text() = default;
};

class Block{
private:
    ShapeType shape;
    sf::CircleShape circle;
    sf::RectangleShape rectangle;
    std::optional<Text> textLabel = std::nullopt;

public:
    bool isHovered = false;
    Block() = default;
    Block(ShapeType shape, float radius, sf::Color color);
    Block(ShapeType shape, float width, float height, sf::Color color);
    void setoutline(int thickness, sf::Color color);
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f center();
    void initText(std::string string, Style &style);
    void align(int idx);
    sf::FloatRect getGlobalBounds();
    sf::Color getFillColor();
    void setFillColor(sf::Color color);
    void draw(sf::RenderWindow& window);
    ~Block() = default;
};

class Image{
private:
    std::optional<sf::Sprite> sprite;
public:
    Image() = default;
    Image(sf::Texture& texture, float width, float height);
    void setPosition(sf::Vector2f pos);
    void setPosition(float x, float y);
    sf::Color getFillColor();
    void setFillColor(sf::Color color);
    void draw(sf::RenderWindow& window);
    ~Image() = default;
};


class Card{
private:
    Block background;
    Image image;
    Text titleText;
    Block titleBg;
    Text tabText;
    Block tabBg;
public:
    Card() = default;
    Card(sf::Texture& texture, std::string title, std::string tabName, sf::Vector2f pos);
    void handleEvent(sf::Event& event, sf::RenderWindow& window, sf::View& cameraView);
    void draw(sf::RenderWindow& window);
};
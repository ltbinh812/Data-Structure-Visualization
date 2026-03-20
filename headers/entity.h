#pragma once
#include <iostream>
#include <optional>
#include <SFML/Graphics.hpp>


enum ShapeType {
    CIRCLE,
    RECTANGLE,
    NODE
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
    void setFillColor(sf::Color color);
    std::string getString();
    void align(int idx);
    float getWidth();
    sf::FloatRect getGlobalBounds();
    sf::Color getFillColor();
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
    sf::Vector2f currentPosition;
    sf::Vector2f targetPosition;
    float moveSpeed = 200.f;
    bool isHovered = false;
    Block() = default;
    Block(ShapeType shape, float radius, sf::Color color);
    Block(ShapeType shape, float width, float height, sf::Color color);
    Block(ShapeType shape, float width, float height, std::string label);
    Block(ShapeType shape, float radius, std::string label);
    void setoutline(int thickness, sf::Color color);
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f pos);
    void setSize(sf::Vector2f rect);
    sf::Vector2f center();
    float getRadius();
    void initText(std::string string, Style &style);
    void align(int idx);
    sf::FloatRect getGlobalBounds();
    sf::Color getFillColor();
    void setFillColor(sf::Color color);
    void setFillColorText(sf::Color color);
    void move(float dt);
    void draw(sf::RenderWindow& window);
    std::string getLabel();
    bool isMoving();
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
    void handleEvent(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView);
    bool clicked(const sf::Event& event, sf::RenderWindow& window, sf::View& cameraView);
    void draw(sf::RenderWindow& window);
};


struct Notification{
    enum class NotifyState { HIDDEN, SLIDING_IN, VISIBLE, SLIDING_OUT };
    sf::RectangleShape box;
    NotifyState state = NotifyState::HIDDEN;
    
    sf::Vector2f hiddenPos;
    sf::Vector2f visiblePos;
    sf::Vector2f currentPos;

    float timer = 0.0f;
    float displayTime = 2.0f; // Đợi 2 giây
    float speed = 200.0f;       // Tốc độ trượt
    Notification() = default;
    sf::Text* message = nullptr;
    void trigger(std::string text);
    void draw(sf::RenderWindow& window);
    ~Notification() {
        if (message) delete message;
    }

};



void drawArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end);
void drawPointer(sf::RenderWindow& window, sf::Vector2f Pos, std::string label);
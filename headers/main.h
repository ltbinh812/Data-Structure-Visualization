#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "entity.h"
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int MAX_SCROLL_Y = 320;
const float scrollSpeed = 40.f;
const float eps = 0.0001f;

extern sf::Time dealtaTime;
extern sf::Font font1;
extern sf::Font font2;
extern sf::Font font3;

extern ImFont* codeFont;

extern Style style1;
extern Style style2;
extern Style style3;
extern Style style4;
extern Style style5;
extern Style style6;

enum AppState{
    MAIN_MENU,
    VISUALIZATION1,
    VISUALIZATION2,
    VISUALIZATION3,
    VISUALIZATION4,
    VISUALIZATION5,
    VISUALIZATION6
};


extern AppState appState;
extern float minX;
extern float maxX;
extern float minY;
extern float maxY;

void resetRectangleMinMax();
void getRectangleMinMax(std::pair<float, float> pos);


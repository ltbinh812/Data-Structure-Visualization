#pragma once
#include <SFML/Graphics.hpp>
#include "entity.h"
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int MAX_SCROLL_Y = 320;
const float scrollSpeed = 40.f;


extern sf::Font font1;
extern sf::Font font2;


extern Style style1;
extern Style style2;
extern Style style3;

enum AppState{
    MAIN_MENU
};
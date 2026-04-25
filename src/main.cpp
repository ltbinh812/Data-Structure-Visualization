#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "main.h"
#include "entity.h"
#include "draw.h"
#include "handleEvent.h"
#include "theme.h"
#include "graphPhysics.h"
using namespace std;

sf::Time dealtaTime;
sf::Font font1;
sf::Font font2;
sf::Font font3;
ImFont* codeFont = nullptr;
Style style1; //logo
Style style2; //button text 1
Style style3; //button text 2
Style style4; //node text
Style style5; //log
Style style6; //Idx text
Style style7; //edge text

AppState appState = AppState::MAIN_MENU;

float minX = 0.0f;   
float maxX = WINDOW_WIDTH;
float minY = 0.0f;
float maxY = WINDOW_HEIGHT;


void resetRectangleMinMax(){
    minX = 0.0f;   
    maxX = WINDOW_WIDTH;
    minY = 0.0f;
    maxY = WINDOW_HEIGHT;
}

void getRectangleMinMax(std::pair<float, float> pos){
    minX = std::min(minX, pos.first);
    maxX = std::max(maxX, pos.first);
    minY = std::min(minY, pos.second);
    maxY = std::max(maxY, pos.second);
}

int main() {
    srand(time(0));
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Visualizing Stack");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        cerr << "Failed to initialize ImGui-SFML" << endl;
        return -1;
    }
    sf::View cameraView = window.getDefaultView();
    

    sf::Texture homeTexture;
    sf::Texture darkModeTexture;
    sf::Texture lightModeTexture;
    if (!homeTexture.loadFromFile("assets/house.png")) {
        cerr << "Failed to load homeTexture" << endl;
        return -1;
    }
    if (!darkModeTexture.loadFromFile("assets/moon.png")) {
        cerr << "Failed to load darkModeTexture" << endl;
        return -1;
    }
    if (!lightModeTexture.loadFromFile("assets/sun.png")) {
        cerr << "Failed to load lightModeTexture" << endl;
        return -1;
    }

    if (!font1.openFromFile("assets/PTC55F.ttf")) {
        cerr << "Failed to load font1" << endl;
        return -1;
    }
    if (!font2.openFromFile("assets/PTC75F.ttf")) {
        cerr << "Failed to load font2" << endl;
        return -1;
    }
    if (!font3.openFromFile("assets/PTC55F.ttf")) {
        cerr << "Failed to load font3" << endl;
        return -1;
    }
    ImGuiIO& io = ImGui::GetIO();
    codeFont = io.Fonts->AddFontFromFileTTF("assets/JetBrainsMono-Regular.ttf", 15.0f);
    if (!ImGui::SFML::UpdateFontTexture()) {
        std::cout << "Failed to update font texture\n";
    }
    io.ConfigWindowsResizeFromEdges = true;
    
    style1 = Style(font2, 120, sf::Color::Black); //logo
    style2 = Style(font2, 35, sf::Color::Black); //button text 1
    style3 = Style(font1, 20, sf::Color::White); //button text 2
    style4 = Style(font2, 25, sf::Color::Black); //node text
    style5 = Style(font3, 25, sf::Color(225, 28, 28, 220)); // log
    style6 = Style(font2, 30, sf::Color::Red); //Idx text
    style7 = Style(font2, 20, sf::Color(255, 0, 255)); //edge text
    sf::Texture LinkedListTexture;
    if (!LinkedListTexture.loadFromFile("assets/LinkedListImage.jpg")) {
        cerr << "Failed to load LinkedListTexture" << endl;
        return -1;
    }
    sf::Texture Test1;
    if (!Test1.loadFromFile("assets/LinkedListImage.jpg")) {
        cerr << "Failed to load Test1" << endl;
        return -1;
    }
    sf::Texture Test2;
    if (!Test2.loadFromFile("assets/MinHeapAndMaxHeap1.png")) {
        cerr << "Failed to load Test2" << endl;
        return -1;
    }
    sf::Texture Test3;
    if (!Test3.loadFromFile("assets/AVL-Tree-in-Javascript.png")) {
        cerr << "Failed to load Test3" << endl;
        return -1;
    }
    sf::Texture Test4;
    if (!Test4.loadFromFile("assets/trie.png")) {
        cerr << "Failed to load Test4" << endl;
        return -1;
    }
    sf::Texture Test5;
    if (!Test5.loadFromFile("assets/dijkstra.jpg")) {
        cerr << "Failed to load Test5" << endl;
        return -1;
    }
    sf::Texture Test6;
    if (!Test6.loadFromFile("assets/kruskal.jpg")) {
        cerr << "Failed to load Test6" << endl;
        return -1;
    }


    Card card1(Test1, "Singly linked list", "Linked list", sf::Vector2f(WINDOW_WIDTH / 4.f - 125.f, WINDOW_HEIGHT / 2.f));
    Card card2(Test2, "Heap", "Tree", sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
    Card card3(Test3, "AVL Tree", "Tree", sf::Vector2f(WINDOW_WIDTH * 3.f / 4.f + 125.f, WINDOW_HEIGHT / 2.f));
    Card card4(Test4, "Trie", "Tree", sf::Vector2f(WINDOW_WIDTH / 4.f - 125.f, WINDOW_HEIGHT * 3.f / 4.f + 250.f));
    Card card5(Test5, "Dijkstra", "Graph", sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT * 3.f / 4.f + 250.f));
    Card card6(Test6, "Kruskal", "Graph", sf::Vector2f(WINDOW_WIDTH * 3.f / 4.f + 125.f, WINDOW_HEIGHT * 3.f / 4.f + 250.f));


    vector<Text> MainMenu_Texts{
        Text("Data Structure Visualizer", style1),
        Text("Developed by Le Tien Binh", style2)
        // card1Text,
        // card1Tab,
        // card2Text,
        // card2Tab
    };

    vector<Block> MainMenu_Buttons{
        // card1, 
        // card2,
        // card3,
        // card4,
        // card5,
        // card6,
        // card1TextBackground,
        // card1TabBackground,
        // card2TextBackground,
        // card2TabBackground
    };
    vector<Image> MainMenu_Images{
        // card1Image,
        // card2Image
        // card3Image,
        // card4Image,
        // card5Image,
        // card6Image
    };

    vector<Card> MainMenu_Cards{
        card1,
        card2,
        card3,
        card4,
        card5,
        card6
    };

    vector<sf::Texture> MainMenu_Textures{
        darkModeTexture,
        lightModeTexture
    };

    vector<sf::Texture> Global_Textures{
        homeTexture,
        darkModeTexture,
        lightModeTexture
        // Image(homeTexture, 75.f, 75.f, 67.f, 50.f),
        // Image(darkModeTexture, 67.f, 67.f, 167.f, 50.f),
        // Image(lightModeTexture, 67.f, 67.f, 167.f, 50.f)
    };

    auto applyCameraConstraints = [&]() {
        float paddingLeft = 200.0f;    
        float paddingRight = 600.0f;  
        float paddingTop = 300.0f;    
        float paddingBottom = 500.0f; 
        float curSafeMinX = minX - paddingLeft;
        float curSafeMaxX = maxX + paddingRight;
        float curSafeMinY = minY - paddingTop;
        float curSafeMaxY = maxY + paddingBottom;

        sf::Vector2f center = cameraView.getCenter();
        sf::Vector2f size = cameraView.getSize();
        
        float halfW = size.x / 2.0f;
        float halfH = size.y / 2.0f;
        float mapWidth = curSafeMaxX - curSafeMinX;
        float mapHeight = curSafeMaxY - curSafeMinY;

        if (size.x >= mapWidth) {
            center.x = curSafeMinX + mapWidth / 2.0f;
        } else {
            if (center.x - halfW < curSafeMinX) center.x = curSafeMinX + halfW;
            if (center.x + halfW > curSafeMaxX) center.x = curSafeMaxX - halfW;
        }

        if (size.y >= mapHeight) {
            center.y = curSafeMinY + mapHeight / 2.0f;
        } else {
            if (center.y - halfH < curSafeMinY) center.y = curSafeMinY + halfH;
            if (center.y + halfH > curSafeMaxY) center.y = curSafeMaxY - halfH;
        }

        cameraView.setCenter(center);
    };




    sf::Color sfmlBgColor = applyTheme(isDarkMode, ImGui::GetStyle());
    float scrollY = 0.f;
    sf::Clock deltaClock;
    bool isDragging = false;
    sf::Vector2f oldMousePos;
    AppState previousState = appState;
    while (window.isOpen()) {
        dealtaTime = deltaClock.restart();
        if(previousState != appState){
            cameraView = window.getDefaultView();
            previousState = appState;
        }

        while (const auto event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }
        

            if(appState == AppState::MAIN_MENU){
                ImGui::SFML::ProcessEvent(window, *event);
                if (event->is<sf::Event::MouseWheelScrolled>()) {
                    if(previousState != appState){
                        scrollY = 0.f;
                        cameraView = window.getDefaultView();
                    }
                    float step = (event->getIf<sf::Event::MouseWheelScrolled>()->delta > 0 ? -1 : 1) * scrollSpeed; 
                    scrollY += step;
                    if(scrollY < 0.f) scrollY = 0.f;
                    else if(scrollY > MAX_SCROLL_Y) scrollY = MAX_SCROLL_Y;
                    else cameraView.move({0.f, step});
                    break;
                }
                handleMainMenuEvents(*event, window, cameraView, MainMenu_Cards);
            }
            

            else if(appState == AppState::VISUALIZATION1 || 
                    appState == AppState::VISUALIZATION2 || 
                    appState == AppState::VISUALIZATION3 || 
                    appState == AppState::VISUALIZATION4 || 
                    appState == AppState::VISUALIZATION5 || 
                    appState == AppState::VISUALIZATION6){
                    window.setView(window.getDefaultView());
                    ImGui::SFML::ProcessEvent(window, *event);
                    
                    if (ImGui::GetIO().WantCaptureMouse) continue; 
                    if (const auto* scrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
                        if (scrolled->wheel == sf::Mouse::Wheel::Vertical) { 
                            float zoomFactor = 1.0f;
                            if (scrolled->delta > 0) zoomFactor = 0.9f; 
                            else if (scrolled->delta < 0) zoomFactor = 1.1f; 
                            sf::Vector2f currentSize = cameraView.getSize();
                            sf::Vector2f newSize = currentSize * zoomFactor;

                            float minViewWidth = 300.0f; 
                            float minViewHeight = 300.0f;
                            
                            float paddingLeft = 200.0f; 
                            float paddingRight = 600.0f;
                            float paddingTop = 300.0f;
                            float paddingBottom = 500.0f; 
                            float maxViewWidth = (maxX + paddingRight) - (minX - paddingLeft); 
                            float maxViewHeight = (maxY + paddingBottom) - (minY - paddingTop);
                            if (newSize.x < minViewWidth || newSize.y < minViewHeight) {
                                zoomFactor = std::max(minViewWidth / currentSize.x, minViewHeight / currentSize.y);
                            } 
                            else if (newSize.x > maxViewWidth || newSize.y > maxViewHeight) {
                                zoomFactor = std::min(maxViewWidth / currentSize.x, maxViewHeight / currentSize.y);
                            }

                            cameraView.zoom(zoomFactor);
                            
                            applyCameraConstraints();
                        }
                    }

                    if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                        if (pressed->button == sf::Mouse::Button::Right) { 
                            isDragging = true;
                            oldMousePos = window.mapPixelToCoords(pressed->position, cameraView);
                        }
                    }

                    if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>()) {
                        if (released->button == sf::Mouse::Button::Right) {
                            isDragging = false;
                        }
                    }


                    if (const auto* moved = event->getIf<sf::Event::MouseMoved>()) {
                        if (isDragging) {
                            sf::Vector2f newMousePos = window.mapPixelToCoords(moved->position, cameraView);
                            sf::Vector2f delta = oldMousePos - newMousePos;
                            
                            cameraView.move(delta);
                            
                            applyCameraConstraints();

                            oldMousePos = window.mapPixelToCoords(moved->position, cameraView);
                        }
                    }   
                    if(appState == AppState::VISUALIZATION5 || appState == AppState::VISUALIZATION6){
                        graphPhysics.handleEvent(event, window, cameraView);          
                    }
            }
        }


        
        if(appState == AppState::VISUALIZATION1){
            // window.clear(sf::Color(255, 255, 255)); 
            window.clear(sfmlBgColor);
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);

            drawVisualization1(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }
  
        if(appState == AppState::VISUALIZATION2){
            window.clear(sfmlBgColor); 
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);

            drawVisualization2(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }

        if(appState == AppState::VISUALIZATION3){
            window.clear(sfmlBgColor); 
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);

            drawVisualization3(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }

        if(appState == AppState::VISUALIZATION4){
            window.clear(sfmlBgColor); 
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);

            drawVisualization4(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }

        if(appState == AppState::VISUALIZATION5){
            window.clear(sfmlBgColor); 
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);
            graphPhysics.updatePhysics(dealtaTime.asSeconds());

            drawVisualization5(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }

        if(appState == AppState::VISUALIZATION6){
            window.clear(sfmlBgColor); 
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);
            graphPhysics.updatePhysics(dealtaTime.asSeconds());

            drawVisualization6(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }

        if(appState == AppState::MAIN_MENU){
            sf::Color menuBgColor = isDarkMode ? sf::Color(15, 18, 25) : sf::Color(245, 247, 250);
            window.clear(menuBgColor);
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);

            drawMainMenu(window, MainMenu_Texts, MainMenu_Buttons, MainMenu_Images, MainMenu_Cards, MainMenu_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }

        window.display();    
    }


    ImGui::SFML::Shutdown();


    return 0;
}
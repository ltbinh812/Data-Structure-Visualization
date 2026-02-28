#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "main.h"
#include "entity.h"
#include "draw.h"
#include "handleEvent.h"


using namespace std;


sf::Font font1;
sf::Font font2;
Style style1; //logo
Style style2; //button text 1
Style style3; //button text 2

int main() {

    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Visualizing Stack");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        cerr << "Failed to initialize ImGui-SFML" << endl;
        return -1;
    }
    sf::View cameraView = window.getDefaultView();
    
    if (!font1.openFromFile("assets/PTC55f.ttf")) {
        cerr << "Failed to load font1" << endl;
        return -1;
    }
    if (!font2.openFromFile("assets/PTC75F.ttf")) {
        cerr << "Failed to load font2" << endl;
        return -1;
    }
    style1 = Style(font2, 120, sf::Color::Black); //logo
    style2 = Style(font2, 35, sf::Color::Black); //button text 1
    style3 = Style(font1, 20, sf::Color::White); //button text 2
    
    sf::Texture LinkedListTexture;
    if (!LinkedListTexture.loadFromFile("assets/LinkedListImage.jpg")) {
        cerr << "Failed to load LinkedListTexture" << endl;
        return -1;
    }
    sf::Texture Test1;
    if (!Test1.loadFromFile("assets/SPOILER_Screenshot_762.png")) {
        cerr << "Failed to load Test1" << endl;
        return -1;
    }
    sf::Texture Test2;
    if (!Test2.loadFromFile("assets/SPOILER_Screenshot_763.png")) {
        cerr << "Failed to load Test2" << endl;
        return -1;
    }
    sf::Texture Test3;
    if (!Test3.loadFromFile("assets/SPOILER_Screenshot_764.png")) {
        cerr << "Failed to load Test3" << endl;
        return -1;
    }
    sf::Texture Test4;
    if (!Test4.loadFromFile("assets/SPOILER_Screenshot_765.png")) {
        cerr << "Failed to load Test4" << endl;
        return -1;
    }
    sf::Texture Test5;
    if (!Test5.loadFromFile("assets/SPOILER_Screenshot_766.png")) {
        cerr << "Failed to load Test5" << endl;
        return -1;
    }
    sf::Texture Test6;
    if (!Test6.loadFromFile("assets/SPOILER_Screenshot_767.png")) {
        cerr << "Failed to load Test6" << endl;
        return -1;
    }


    Card card1(Test1, "Double linked list", "Linked list", sf::Vector2f(WINDOW_WIDTH / 4.f - 125.f, WINDOW_HEIGHT / 2.f));
    Card card2(Test2, "Circular linked list", "Linked list", sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
    Card card3(Test3, "Stack", "Stack", sf::Vector2f(WINDOW_WIDTH * 3.f / 4.f + 125.f, WINDOW_HEIGHT / 2.f));
    Card card4(Test4, "Queue", "Queue", sf::Vector2f(WINDOW_WIDTH / 4.f - 125.f, WINDOW_HEIGHT * 3.f / 4.f + 250.f));
    Card card5(Test5, "Binary tree", "Tree", sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT * 3.f / 4.f + 250.f));
    Card card6(Test6, "Graph", "Graph", sf::Vector2f(WINDOW_WIDTH * 3.f / 4.f + 125.f, WINDOW_HEIGHT * 3.f / 4.f + 250.f));

    Text test("*Hinh anh chi mang tinh chat minh hoa=))", style2);
    test.setPosition(WINDOW_WIDTH / 2.f, 200.f);

    vector<Text> MainMenu_Texts{
        Text("Data Structure Visualization", style1),
        test
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

    AppState appState = AppState::MAIN_MENU;
    float scrollY = 0.f;
    sf::Clock deltaClock;
    sf::Time dealtaTime;
    while (window.isOpen()) {
        dealtaTime = deltaClock.restart();

        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            if (event->is<sf::Event::MouseWheelScrolled>()) {
                float step = (event->getIf<sf::Event::MouseWheelScrolled>()->delta > 0 ? -1 : 1) * scrollSpeed; 
                scrollY += step;
                if(scrollY < 0.f) scrollY = 0.f;
                else if(scrollY > MAX_SCROLL_Y) scrollY = MAX_SCROLL_Y;
                else cameraView.move({0.f, step});
                // cout << "Scroll Y: " << scrollY << endl;
                break;
            }

            if(appState == AppState::MAIN_MENU){
                handleMainMenuEvents(*event, window, cameraView, MainMenu_Cards);
            }
        }

        // SFML

        if(appState == AppState::MAIN_MENU){
            // cameraView.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + scrollY);
            window.clear(sf::Color(192,192,192)); // Màu nền tối cho chuyên nghiệp
            window.setView(cameraView);

            drawMainMenu(window, MainMenu_Texts, MainMenu_Buttons, MainMenu_Images, MainMenu_Cards);
        }
        
  
        



        // ImGui 
        window.setView(window.getDefaultView());
        ImGui::SFML::Update(window, dealtaTime);


        ImGui::Begin("Code Trace 1");
        ImGui::Text("1. Khởi tạo cửa sổ và ImGui");
        ImGui::Text("2. Vòng lặp sự kiện và xử lý cuộn chuột");
        ImGui::Text("3. Vẽ các phần tử Stack và giao diện ImGui");
        ImGui::End();
        ImGui::Begin("Code Trace 2");
        ImGui::Text("1. Khởi tạo cửa sổ và ImGui");
        ImGui::Text("2. Vòng lặp sự kiện và xử lý cuộn chuột");
        ImGui::Text("3. Vẽ các phần tử Stack và giao diện ImGui");
        ImGui::End();


        ImGui::SFML::Render(window);
        window.display();    
    }


    ImGui::SFML::Shutdown();


    return 0;
}
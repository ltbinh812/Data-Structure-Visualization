#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "main.h"
#include "entity.h"
#include "draw.h"
#include "handleEvent.h"
#include "theme.h"

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
    if (!Test4.loadFromFile("assets/LinkedListImage.jpg")) {
        cerr << "Failed to load Test4" << endl;
        return -1;
    }
    sf::Texture Test5;
    if (!Test5.loadFromFile("assets/LinkedListImage.jpg")) {
        cerr << "Failed to load Test5" << endl;
        return -1;
    }
    sf::Texture Test6;
    if (!Test6.loadFromFile("assets/LinkedListImage.jpg")) {
        cerr << "Failed to load Test6" << endl;
        return -1;
    }


    Card card1(Test1, "Singly linked list", "Linked list", sf::Vector2f(WINDOW_WIDTH / 4.f - 125.f, WINDOW_HEIGHT / 2.f));
    Card card2(Test2, "Heap", "Tree", sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
    Card card3(Test3, "AVL Tree", "Tree", sf::Vector2f(WINDOW_WIDTH * 3.f / 4.f + 125.f, WINDOW_HEIGHT / 2.f));
    Card card4(Test4, "Queue", "Queue", sf::Vector2f(WINDOW_WIDTH / 4.f - 125.f, WINDOW_HEIGHT * 3.f / 4.f + 250.f));
    Card card5(Test5, "Binary tree", "Tree", sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT * 3.f / 4.f + 250.f));
    Card card6(Test6, "Graph", "Graph", sf::Vector2f(WINDOW_WIDTH * 3.f / 4.f + 125.f, WINDOW_HEIGHT * 3.f / 4.f + 250.f));

    Text test("*Hinh anh chi mang tinh chat minh hoa=)*", style2);
    test.setPosition(WINDOW_WIDTH / 2.f, 200.f);


    vector<Text> MainMenu_Texts{
        Text("Data Structure Visualizer", style1),
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

    vector<sf::Texture> Global_Textures{
        homeTexture,
        darkModeTexture,
        lightModeTexture
        // Image(homeTexture, 75.f, 75.f, 67.f, 50.f),
        // Image(darkModeTexture, 67.f, 67.f, 167.f, 50.f),
        // Image(lightModeTexture, 67.f, 67.f, 167.f, 50.f)
    };

    auto applyCameraConstraints = [&]() {
        // TÍNH TOÁN LẠI SAFE BORDER LIÊN TỤC MỖI KHI HÀM ĐƯỢC GỌI
        float paddingLeft = 200.0f;   // Lề trái 
        float paddingRight = 600.0f;  // Lề phải (Rộng hơn để chừa chỗ cho bảng Code Highlight)
        float paddingTop = 300.0f;    // Lề trên
        float paddingBottom = 500.0f; // Lề dưới (Chừa chỗ cho thanh Control Panel)
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

        // Xử lý kẹp trục X
        if (size.x >= mapWidth) {
            // Nếu khung nhìn to hơn cả bản đồ -> Căn giữa bản đồ luôn
            center.x = curSafeMinX + mapWidth / 2.0f;
        } else {
            // Nếu bình thường -> Đụng vách nào ép lùi về vách đó
            if (center.x - halfW < curSafeMinX) center.x = curSafeMinX + halfW;
            if (center.x + halfW > curSafeMaxX) center.x = curSafeMaxX - halfW;
        }

        // Xử lý kẹp trục Y
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
                if (event->is<sf::Event::MouseWheelScrolled>()) {
                    if(previousState != appState) scrollY = 0.f;
                    float step = (event->getIf<sf::Event::MouseWheelScrolled>()->delta > 0 ? -1 : 1) * scrollSpeed; 
                    scrollY += step;
                    if(scrollY < 0.f) scrollY = 0.f;
                    else if(scrollY > MAX_SCROLL_Y) scrollY = MAX_SCROLL_Y;
                    else cameraView.move({0.f, step});
                    // cout << "Scroll Y: " << scrollY << endl;
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
                    std::cout << "main.cpp\n";
                    // 1. TÍNH NĂNG ZOOM (Dùng hàm getIf<T> để trích xuất dữ liệu cuộn chuột)
                    if (const auto* scrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
                        // Lưu ý: SFML 3.0 đổi tên Enum thành sf::Mouse::Wheel::Vertical
                        if (scrolled->wheel == sf::Mouse::Wheel::Vertical) { 
                            float zoomFactor = 1.0f;
                            if (scrolled->delta > 0) zoomFactor = 0.9f;      // Lăn lên -> Phóng to
                            else if (scrolled->delta < 0) zoomFactor = 1.1f; // Lăn xuống -> Thu nhỏ
                            sf::Vector2f currentSize = cameraView.getSize();
                            sf::Vector2f newSize = currentSize * zoomFactor;

                            // -- ĐỊNH NGHĨA KÍCH THƯỚC TRẦN VÀ SÀN --
                           // -- ĐỊNH NGHĨA KÍCH THƯỚC TRẦN VÀ SÀN --
                            float minViewWidth = 300.0f; // Zoom In kịch kim là khung 300x300
                            float minViewHeight = 300.0f;
                            
                                // CẬP NHẬT TRỰC TIẾP TỪ BIẾN TOÀN CỤC
                            float paddingLeft = 200.0f;   // Lề trái 
                            float paddingRight = 600.0f;  // Lề phải (Rộng hơn để chừa chỗ cho bảng Code Highlight)
                            float paddingTop = 300.0f;    // Lề trên
                            float paddingBottom = 500.0f; // Lề dưới (Chừa chỗ cho thanh Control Panel)                            float maxViewWidth = (maxX + currentPadding) - (minX - currentPadding); 
                            float maxViewWidth = (maxX + paddingRight) - (minX - paddingLeft); 
                            float maxViewHeight = (maxY + paddingBottom) - (minY - paddingTop);
                            // -- TÍNH TOÁN ÉP ZOOM FACTOR --
                            if (newSize.x < minViewWidth || newSize.y < minViewHeight) {
                                // Ép dừng zoom in
                                zoomFactor = std::max(minViewWidth / currentSize.x, minViewHeight / currentSize.y);
                            } 
                            else if (newSize.x > maxViewWidth || newSize.y > maxViewHeight) {
                                // Ép dừng zoom out
                                zoomFactor = std::min(maxViewWidth / currentSize.x, maxViewHeight / currentSize.y);
                            }

                            cameraView.zoom(zoomFactor);
                            
                            // Lỡ có dính viền khi zoom ra thì ép nó đẩy vào trong lại
                            applyCameraConstraints();
                        }
                    }

                    // 2. TÍNH NĂNG PAN (Kéo chuột)
                    
                    // Bắt sự kiện: Bấm chuột xuống
                    if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                        if (pressed->button == sf::Mouse::Button::Right) { // SFML 3.0 Enum mới
                            isDragging = true;
                            oldMousePos = window.mapPixelToCoords(pressed->position, cameraView);
                        }
                    }

                    // Bắt sự kiện: Nhả chuột ra
                    if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>()) {
                        if (released->button == sf::Mouse::Button::Right) {
                            isDragging = false;
                        }
                    }

                    // Bắt sự kiện: Di chuyển chuột
                    if (const auto* moved = event->getIf<sf::Event::MouseMoved>()) {
                        if (isDragging) {
                            // Tọa độ chuột giờ nằm ở biến moved->position
                            sf::Vector2f newMousePos = window.mapPixelToCoords(moved->position, cameraView);
                            sf::Vector2f delta = oldMousePos - newMousePos;
                            
                            cameraView.move(delta);
                            oldMousePos = window.mapPixelToCoords(moved->position, cameraView);
                        }
                    }
                    if (const auto* moved = event->getIf<sf::Event::MouseMoved>()) {
                    if (isDragging) {
                        sf::Vector2f newMousePos = window.mapPixelToCoords(moved->position, cameraView);
                        sf::Vector2f delta = oldMousePos - newMousePos;
                        
                        cameraView.move(delta);
                        
                        // Ép camera dội ngược lại nếu người dùng cố tình kéo lố ra ngoài biển
                        applyCameraConstraints();

                        // QUAN TRỌNG: Cập nhật lại tọa độ chuột theo Camera ĐÃ ĐƯỢC ÉP VỀ
                        // (Nếu thiếu dòng này, kéo trúng viền là camera sẽ bị giật cục/kẹt cứng)
                        oldMousePos = window.mapPixelToCoords(moved->position, cameraView);
                    }
                }
                    
            }

        }

        // SFML

        
        if(appState == AppState::VISUALIZATION1){
            // window.clear(sf::Color(255, 255, 255)); // Màu nền trắng cho phần visualization
            window.clear(sfmlBgColor);
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);

            drawVisualization1(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }
  
        if(appState == AppState::VISUALIZATION2){
            window.clear(sfmlBgColor); // Màu nền trắng cho phần visualization
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);

            drawVisualization2(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }

        if(appState == AppState::VISUALIZATION3){
            window.clear(sfmlBgColor); // Màu nền trắng cho phần visualization
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);

            drawVisualization3(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }

        if(appState == AppState::VISUALIZATION4){
            window.clear(sfmlBgColor); // Màu nền trắng cho phần visualization
            ImGui::SFML::Update(window, dealtaTime);
            window.setView(cameraView);

            drawVisualization4(window, Global_Textures, sfmlBgColor);
            ImGui::SFML::Render(window);
        }



        if(appState == AppState::MAIN_MENU){
            // cameraView.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + scrollY);
            window.clear(sf::Color(192,192,192)); // Màu nền tối cho chuyên nghiệp
            window.setView(cameraView);

            drawMainMenu(window, MainMenu_Texts, MainMenu_Buttons, MainMenu_Images, MainMenu_Cards);
        }

        window.display();    
    }


    ImGui::SFML::Shutdown();


    return 0;
}
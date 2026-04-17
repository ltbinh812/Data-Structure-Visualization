#include "theme.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "main.h"

bool isDarkMode = false;
ImVec4 title1Color = ImVec4(0, 1, 0, 1);
ImVec4 title2Color = ImVec4(1, 1, 0, 1);
sf::Color edgeColor = sf::Color(255, 127, 36);
sf::Color outlineColor = sf::Color::Black;

sf::Color applyTheme(bool isDark, ImGuiStyle& style) {
    // Luôn đặt các thông số chung ở đây để tránh bị "dính" style cũ
    style.WindowRounding = 8.0f;
    style.FrameRounding  = 5.0f;
    style.WindowBorderSize = 1.0f;

    isDarkMode = isDark; 


    if (isDark) {
        ImGui::StyleColorsDark();

        // 1. Màu nền & Chữ
        style.Colors[ImGuiCol_WindowBg]         = ImVec4(0.10f, 0.12f, 0.16f, 1.0f);
        style.Colors[ImGuiCol_Text]             = ImVec4(0.85f, 0.90f, 0.95f, 1.0f);
        style.Colors[ImGuiCol_Border]           = ImVec4(0.18f, 0.22f, 0.30f, 1.0f);

        // 2. Button (Phải đủ 3 trạng thái)
        style.Colors[ImGuiCol_Button]           = ImVec4(0.16f, 0.24f, 0.35f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered]    = ImVec4(0.22f, 0.35f, 0.55f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive]     = ImVec4(0.28f, 0.45f, 0.70f, 1.0f); // Thêm Active

        // 3. Header & Tabs
        style.Colors[ImGuiCol_Header]           = ImVec4(0.18f, 0.50f, 0.92f, 0.4f);
        style.Colors[ImGuiCol_HeaderHovered]    = ImVec4(0.18f, 0.50f, 0.92f, 0.7f);
        style.Colors[ImGuiCol_HeaderActive]     = ImVec4(0.18f, 0.50f, 0.92f, 1.0f);

        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.2f);

        title1Color = ImVec4(0, 1, 0, 1);
        title2Color = ImVec4(1, 1, 0, 1);

        edgeColor = sf::Color(255, 127, 36);
        outlineColor = sf::Color(255, 127, 36);

        return sf::Color(18, 20, 25);
    } 
    else {
        ImGui::StyleColorsLight();

        // 1. Màu nền & Chữ
        style.Colors[ImGuiCol_WindowBg]         = ImVec4(0.94f, 0.97f, 1.00f, 1.0f); 
        style.Colors[ImGuiCol_Text]             = ImVec4(0.07f, 0.15f, 0.30f, 1.0f);
        style.Colors[ImGuiCol_Border]           = ImVec4(0.75f, 0.85f, 0.95f, 1.0f);

        // 2. Button (Phải ghi đè lại hoàn toàn để không dính màu Dark)
        style.Colors[ImGuiCol_Button]           = ImVec4(0.80f, 0.90f, 1.00f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered]    = ImVec4(0.70f, 0.85f, 0.95f, 1.0f); // Phải có cái này!
        style.Colors[ImGuiCol_ButtonActive]     = ImVec4(0.60f, 0.80f, 0.90f, 1.0f); // Phải có cái này!

        // 3. Header & Tabs
        style.Colors[ImGuiCol_Header]           = ImVec4(0.00f, 0.45f, 0.90f, 0.3f);
        style.Colors[ImGuiCol_HeaderHovered]    = ImVec4(0.00f, 0.45f, 0.90f, 0.6f);
        style.Colors[ImGuiCol_HeaderActive]     = ImVec4(0.00f, 0.45f, 0.90f, 1.0f);

        style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.4f);

        title1Color = ImVec4(0.13f, 0.55f, 0.13f, 1.0f); // Xanh lá đậm Light
        title2Color = ImVec4(0.00f, 0.40f, 0.85f, 1.0f); // Xanh dương Light

        edgeColor = sf::Color::Black;
        outlineColor = sf::Color::Black;

        return sf::Color(215, 230, 240); 
    }
}
// sf::Color applyTheme(bool isDark, ImGuiStyle& style) {
//     if (isDark) {
//         // --- CHẾ ĐỘ DARK (Giữ nguyên phong cách cũ) ---
//         ImGui::StyleColorsDark();
//         style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
//         style.Colors[ImGuiCol_Text]     = ImVec4(0.85f, 0.85f, 0.85f, 1.0f);
//         style.WindowBorderSize          = 0.0f;
//         return sf::Color(30, 30, 30);
//     } else {
//         // --- CHẾ ĐỘ LIGHT (Tông Xám Bạc - Chắc chắn không phải trắng) ---
//         ImGui::StyleColorsLight();

//         // 1. Nền ImGui: Màu xám bạc rõ rệt (Tương đương RGB 215, 215, 220)
//         style.Colors[ImGuiCol_WindowBg] = ImVec4(0.84f, 0.84f, 0.86f, 1.0f); 
        
//         // 2. Chữ ImGui: Màu đen than (Để cực kỳ sắc nét trên nền xám)
//         style.Colors[ImGuiCol_Text]     = ImVec4(0.08f, 0.08f, 0.10f, 1.0f);
        
//         // 3. Các thành phần khác để đồng bộ
//         style.Colors[ImGuiCol_Border]   = ImVec4(0.70f, 0.70f, 0.75f, 1.0f); // Viền xám đậm hơn nền
//         style.WindowBorderSize          = 1.0f;
//         style.Colors[ImGuiCol_Button]   = ImVec4(0.75f, 0.75f, 0.80f, 1.0f); // Nút màu xám thép

//         // Tăng độ bo góc cho hiện đại
//         style.WindowRounding = 8.0f;
//         style.FrameRounding  = 4.0f;

//         // 4. Trả về màu nền SFML (Màu xám xi măng nhạt)
//         // Việc để nền SFML tối hơn bảng điều khiển giúp bảng "nổi" lên mà không cần dùng màu trắng
//         return sf::Color(180, 180, 185); 
//     }
// }
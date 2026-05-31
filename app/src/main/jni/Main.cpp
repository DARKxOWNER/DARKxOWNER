#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <jni.h>
#include <android/log.h>
#include <elf.h>
#include <dlfcn.h>
#include <sys/system_properties.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "Includes/Logger.h"
#include "Includes/Macros.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"

#include "ImGui/Call_ImGui.h"
#include "IL2Cpp/Call_IL2Cpp.h"
#include "Hacks/include.h"
#include "KittyMemory/MemoryPatch.h"

#include "ctorHook/ConstructorHook.hpp"
#include "FTools/Iconcpp.h"
#include "FTools/Font.h"
#include "FTools/Icon.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <jni.h>
#include <pthread.h>
#include <stdio.h>
#include <android/log.h>
#include <iostream>
#include <fstream>
#include <array>
#include <unordered_map>

#include "Fields.h"
#include "Hooks.h"

std::chrono::steady_clock::time_point appStartTime = std::chrono::steady_clock::now();

std::string FormatTimeDuration(std::chrono::steady_clock::duration duration) {
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration % std::chrono::hours(1)).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration % std::chrono::minutes(1)).count();

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
    return std::string(buffer);
}

void RenderFloatingInfo(ImDrawList* draw, float screenWidth, float screenHeight) {
    if (ClearDisplay) return;

    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&currentTime);
    
    char dateStr[64], timeStr[64];
    strftime(dateStr, sizeof(dateStr), "%B %d, %Y", localTime);
    strftime(timeStr, sizeof(timeStr), "%I:%M %p", localTime);
    
    for (char* p = dateStr; *p; p++) *p = toupper(*p);
    for (char* p = timeStr; *p; p++) *p = toupper(*p);

    auto currentDuration = std::chrono::steady_clock::now() - appStartTime;
    std::string playTime = FormatTimeDuration(currentDuration);
    for (char& c : playTime) c = toupper(c);

    float padding = 30.0f;
    float lineSpacing = 28.0f;
    float startY = screenHeight - padding - (lineSpacing * 5);

    auto drawTextWithSpacing = [&](const char* text, ImVec2 pos, ImColor color) {
        float xOffset = 0.0f;
        float charSpacing = 1.5f;
        for (int i = 0; text[i] != '\0'; i++) {
            char c[2] = { text[i], '\0' };
            draw->AddText(ImVec2(pos.x + xOffset, pos.y), color, c);
            xOffset += ImGui::CalcTextSize(c).x + charSpacing;
        }
    };

    char line2[128], line3[64], line4[64];
    snprintf(line2, sizeof(line2), "DATE : %s", dateStr);
    snprintf(line3, sizeof(line3), "TIME : %s", timeStr);
    snprintf(line4, sizeof(line4), "PLAYING TIME : %s", playTime.c_str());

    ImColor gold(255, 215, 0, 255);
    ImColor white(255, 255, 255, 255);

    drawTextWithSpacing("CALL OF DUTY GLOBAL | 1.0.54", ImVec2(padding, startY), gold);
    drawTextWithSpacing(line2, ImVec2(padding, startY + lineSpacing), white);
    drawTextWithSpacing(line3, ImVec2(padding, startY + lineSpacing * 2), white);
    drawTextWithSpacing(line4, ImVec2(padding, startY + lineSpacing * 3), white);
    drawTextWithSpacing("DEVELOPER : @NewbotPrexh", ImVec2(padding, startY + lineSpacing * 4), white);
}

static bool isMenuVisible = true;

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    static bool g_Initialized = false;
    
    if (!g_Initialized)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle *style = &ImGui::GetStyle();
        
        style->WindowPadding = ImVec2(10, 10);
        style->FrameBorderSize = 6.0f;
        style->WindowBorderSize = 6.0f;
        style->FramePadding = ImVec2(2, 2);
        style->ItemSpacing = ImVec2(3, 2);
        style->IndentSpacing = 12;
        style->ScrollbarSize = 6.5;
        style->WindowRounding = 2.0f;
        style->FrameRounding = 1.5f;
        style->ScrollbarRounding = 2;
        style->WindowBorderSize = 4;
        style->FrameBorderSize = 4;
        style->WindowTitleAlign = ImVec2(0.5, 0.5);
        style->ButtonTextAlign = ImVec2(0.5, 0.5);
        style->WindowPadding = ImVec2(6, 8);
        style->FramePadding = ImVec2(2, 2);

        // Text and Basic Colors
        style->Colors[ImGuiCol_Text]                 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style->Colors[ImGuiCol_TextDisabled]         = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        style->Colors[ImGuiCol_WindowBg]             = ImVec4(0.08f, 0.08f, 0.08f, 0.95f);
        style->Colors[ImGuiCol_TitleBg]              = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        style->Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        style->Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
        style->Colors[ImGuiCol_Border]               = ImVec4(0.3f, 0.3f, 0.3f, 0.5f);
        style->Colors[ImGuiCol_FrameBg]              = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
        style->Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style->Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
        style->Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        style->Colors[ImGuiCol_Separator]            = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        style->Colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        style->Colors[ImGuiCol_SeparatorActive]      = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        style->Colors[ImGuiCol_Button]               = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style->Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.3f, 0.3f, 0.6f, 1.0f);
        style->Colors[ImGuiCol_ButtonActive]         = ImVec4(0.4f, 0.4f, 0.8f, 1.0f);
        style->Colors[ImGuiCol_Tab]                  = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        style->Colors[ImGuiCol_TabHovered]           = ImVec4(0.3f, 0.3f, 0.6f, 1.0f);
        style->Colors[ImGuiCol_TabActive]            = ImVec4(0.4f, 0.4f, 0.8f, 1.0f);
        style->Colors[ImGuiCol_Header]               = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        style->Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.3f, 0.3f, 0.6f, 1.0f);
        style->Colors[ImGuiCol_HeaderActive]         = ImVec4(0.4f, 0.4f, 0.8f, 1.0f);
        style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        style->Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.4f, 0.4f, 0.8f, 1.0f);
        style->Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.5f, 0.5f, 1.0f, 1.0f);
        style->Colors[ImGuiCol_CheckMark]            = ImVec4(0.8f, 0.4f, 0.4f, 1.0f);
        style->Colors[ImGuiCol_SliderGrab]           = ImVec4(0.6f, 0.3f, 0.3f, 1.0f);
        style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.8f, 0.4f, 0.4f, 1.0f);
        style->Colors[ImGuiCol_PopupBg]              = ImVec4(0.08f, 0.08f, 0.08f, 0.95f);
        style->Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

        style->ScrollbarSize *= 1.0f;

        ImGui_ImplAndroid_Init();
        ImGui_ImplOpenGL3_Init("#version 300 es");

        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = NULL;
        
        static const ImWchar icons_ranges[] = {
            0x0020, 0x00FF, 0x3000, 0x30FF, 0x31F0, 0x31FF,
            0xFF00, 0xFFEF, 0x4e00, 0x9FAF, 0xf000, 0xf3ff,
            0xe000, 0xf8ff, 0
        };

        ImFontConfig CustomFont;
        CustomFont.FontDataOwnedByAtlas = true;
        ImFontConfig icons_config;
        CustomFont.FontDataOwnedByAtlas = false;

        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5;
        icons_config.OversampleV = 2.5;

        io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 22.f, &CustomFont);
        io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data1, font_awesome_size1, 28.0f, &icons_config, icons_ranges);
        io.Fonts->AddFontFromMemoryCompressedTTF(Custom_data, Custom_size, 26.0f, &CustomFont, io.Fonts->GetGlyphRangesJapanese());

        ImGui::GetStyle().ScaleAllSizes(3.0f);
        
        // Initialize namespace-based configs
        Config::ESP::Enabled = false;
        Config::ESP::Alert = false;
        Config::ESP::Count = false;
        Config::ESP::Name = false;
        Config::ESP::isPlayerLine = false;
        Config::ESP::Box = false;
        Config::ESP::Health = false;
        Config::ESP::Distance = false;
        Config::ESP::Crosshair = false;
        Config::ESP::Aimline = false;
        Config::ESP::lineSize = 1.5f;
        Config::ESP::target = LineTarget::Bottom;
        
        Config::ESP::Colors::PlayerLine = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        Config::ESP::Colors::PlayerBox = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        Config::ESP::Colors::PlayerName = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        Config::ESP::Colors::PlayerHealth = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        Config::ESP::Colors::PlayerDistance = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        Config::ESP::Colors::PlayerSkeleton = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        Config::ESP::Colors::BotLine = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
        Config::ESP::Colors::BotBox = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
        Config::ESP::Colors::BotName = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
        Config::ESP::Colors::BotHealth = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
        Config::ESP::Colors::BotDistance = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
        
        Config::ESP::Colors::Others = ImVec4(0.88f, 0.0f, 0.0f, 0.7f);
        
        Config::Aim::Aimbot360 = false;
        Config::Aim::AimAssist = false;
        Config::Aim::showAimAssistSlider = false;
        Config::Aim::AimAssistSize = 50.0f;
        Config::Aim::AimSilent = false;
        Config::Aim::Target = EAimTarget::Chest;
        Config::Aim::Trigger = EAimTrigger::None;
        Config::Aim::By = EAim::Distance;
        Config::Aim::size = 100.0f;
        
        Config::Memory::Recoil = false;
        Config::Memory::Spread = false;
        Config::Memory::Reload = false;
        Config::Memory::Shake = false;
        Config::Memory::Scope = false;
        Config::Memory::Switch = false;
        Config::Memory::Flash = false;
        Config::Memory::Hit = false;
        Config::Memory::Auto = false;
        Config::Memory::Fire = false;
        Config::Memory::Cost = false;
        Config::Memory::Overheat = false;
        Config::Memory::Range = false;
        Config::Memory::Kinetic = false;
        Config::Memory::AutoSnipe = false;
        
        Config::Movement::Speed = false;
        Config::Movement::SpeedMultiplier = 2.0f;
        Config::Movement::Jump = false;
        Config::Movement::JumpHeight = 1.5f;
        Config::Movement::Slide = false;
        Config::Movement::SlideSpeed = 1.5f;
        Config::Movement::Snowboard = false;
        Config::Movement::SnowboardSpeed = 50.0f;
        
        Config::Features::Parachute = false;
        Config::Features::Diving = false;
        Config::Features::Radar = false;
        Config::Features::Spectate = false;
        Config::Features::Revive = false;
        Config::Features::Medicine = false;
        Config::Features::JetJump = false;
        Config::Features::JetSlide = false;
        Config::Features::LongJump = false;
        Config::Features::HighFPS = false;
        
        ClearDisplay = false;
        
        g_Initialized = true;
    }

    ImGuiIO *io = &ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(get_width(), get_height());
    ImGui::NewFrame();

    if (!isMenuVisible) {
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImVec2 buttonSize = ImVec2(200, 50);
        ImVec2 buttonPos = ImVec2((displaySize.x - buttonSize.x) / 2, 10);

        ImGui::SetNextWindowPos(buttonPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(buttonSize);

        ImGui::Begin("InvisibleButtonWindow", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoScrollbar);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        if (ImGui::InvisibleButton("##ShowMenu", buttonSize)) {
            isMenuVisible = true;
        }

        ImGui::PopStyleVar(2);
        ImGui::End();
    }

    // Draw ESP
    ESP::DrawESP(ImGui::GetBackgroundDrawList(), get_width(), get_height(), get_density());
    ESP::AimBotFunc();

    if (isMenuVisible) {
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
        ImGui::SetNextWindowPos(center, ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(885, 635), ImGuiCond_Once);

        char title[256];
        sprintf(title, " CODM GLOBAL LOADER | FPS: %.0d ###FPSTitle", static_cast<int>(io->Framerate));

        if (ImGui::Begin(title, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
            static int tab = 1;
            const float navWidth = 190.0f;
            const float childHeight = ImGui::GetContentRegionAvail().y;

            // RGB Animation for header
            static float t = 0.0f;
            t += ImGui::GetIO().DeltaTime * 2.0f;
            ImVec4 rgbColor = ImVec4(
                (sin(t) * 0.5f + 0.5f),
                (sin(t + 2.094f) * 0.5f + 0.5f),
                (sin(t + 4.188f) * 0.5f + 0.5f),
                1.0f
            );

            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Developer: @Newbotprexh").x) * 0.5f);
            ImGui::TextColored(rgbColor, "Developer: @Newbotprexh");
            ImGui::Separator();

            ImGui::BeginChild("Tabs", ImVec2(navWidth, 0), true);
            if (ImGui::Button("  " ICON_FA_EYE " ", ImVec2(-1, 50))) tab = 1;
            if (ImGui::Button("  " ICON_FA_DATABASE " ", ImVec2(-1, 50))) tab = 2;
            if (ImGui::Button("  " ICON_FA_GAMEPAD " ", ImVec2(-1, 50))) tab = 3;
            if (ImGui::Button("  " ICON_FA_CROSSHAIRS " ", ImVec2(-1, 50))) tab = 4;
            if (ImGui::Button("  " ICON_FA_COG " ", ImVec2(-1, 50))) tab = 5;
            ImGui::EndChild();

            ImGui::SameLine();
            const float contentWidth = ImGui::GetContentRegionAvail().x;
            const float leftPanelWidth = contentWidth * 0.40f;
            const float rightPanelWidth = contentWidth - leftPanelWidth - ImGui::GetStyle().ItemSpacing.x;

            // TAB 1: ESP
            if (tab == 1) {
                static float rgb_timer = 0.0f;
                rgb_timer += ImGui::GetIO().DeltaTime * 0.5f;
                ImVec4 rgb_color = ImVec4(
                    (sin(rgb_timer) * 0.5f + 0.5f),
                    (sin(rgb_timer + 2.0f) * 0.5f + 0.5f),
                    (sin(rgb_timer + 4.0f) * 0.5f + 0.5f),
                    1.0f
                );
                
                ImGui::BeginChild("ESP Nav##1", ImVec2(leftPanelWidth, childHeight), true);
                {
                    ImGui::TextColored(rgb_color, "Extra Sensory Perception");
                    ImGui::Spacing();
                    
                    if (Config::ESP::Enabled) {
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.8f, 0.1f, 1.0f));
                        if (ImGui::Button("ESP (ON)", ImVec2(-1, 40))) Config::ESP::Enabled = false;
                        ImGui::PopStyleColor();
                    } else {
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
                        if (ImGui::Button("ESP (OFF)", ImVec2(-1, 40))) Config::ESP::Enabled = true;
                        ImGui::PopStyleColor();
                    }
                    
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::TextColored(rgb_color, "Advanced Visuals");
                    ImGui::Spacing();
                    ImGui::Checkbox("Player Line", &Config::ESP::isPlayerLine);
                    ImGui::Checkbox("Player Box", &Config::ESP::Box);
                    ImGui::Checkbox("Player Health", &Config::ESP::Health);
                    ImGui::Checkbox("Player Name", &Config::ESP::Name);
                    ImGui::Checkbox("Player Distance", &Config::ESP::Distance);
                    ImGui::Checkbox("Player Counter", &Config::ESP::Count);
                    ImGui::Checkbox("Player 360 Alert", &Config::ESP::Alert);
                    ImGui::Separator();
                }
                ImGui::EndChild();

                ImGui::SameLine();
                ImGui::BeginChild("ESP Content##1", ImVec2(rightPanelWidth, childHeight), true);
                {
                    ImGui::TextColored(rgb_color, "Extra Visuals");
                    ImGui::Spacing();
                    
                    if (ImGui::CollapsingHeader("Player ESP Colors")) {
                        ImGui::ColorEdit4("Player Line", (float*)&Config::ESP::Colors::PlayerLine, ImGuiColorEditFlags_NoInputs);
                        ImGui::ColorEdit4("Player Box", (float*)&Config::ESP::Colors::PlayerBox, ImGuiColorEditFlags_NoInputs);
                        ImGui::ColorEdit4("Player Name", (float*)&Config::ESP::Colors::PlayerName, ImGuiColorEditFlags_NoInputs);
                        ImGui::ColorEdit4("Player Health", (float*)&Config::ESP::Colors::PlayerHealth, ImGuiColorEditFlags_NoInputs);
                        ImGui::ColorEdit4("Player Distance", (float*)&Config::ESP::Colors::PlayerDistance, ImGuiColorEditFlags_NoInputs);
                        ImGui::Separator();
                    }

                    if (ImGui::CollapsingHeader("Bot ESP Colors")) {
                        ImGui::ColorEdit4("Bot Line", (float*)&Config::ESP::Colors::BotLine, ImGuiColorEditFlags_NoInputs);
                        ImGui::ColorEdit4("Bot Box", (float*)&Config::ESP::Colors::BotBox, ImGuiColorEditFlags_NoInputs);
                        ImGui::ColorEdit4("Bot Name", (float*)&Config::ESP::Colors::BotName, ImGuiColorEditFlags_NoInputs);
                        ImGui::ColorEdit4("Bot Health", (float*)&Config::ESP::Colors::BotHealth, ImGuiColorEditFlags_NoInputs);
                        ImGui::ColorEdit4("Bot Distance", (float*)&Config::ESP::Colors::BotDistance, ImGuiColorEditFlags_NoInputs);
                        ImGui::Separator();
                    }

                    if (ImGui::Button("Reset All Colors to Default", ImVec2(-1, 30))) {
                        Config::ESP::Colors::PlayerLine = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                        Config::ESP::Colors::PlayerBox = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                        Config::ESP::Colors::PlayerName = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                        Config::ESP::Colors::PlayerHealth = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                        Config::ESP::Colors::PlayerDistance = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                        Config::ESP::Colors::BotLine = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
                        Config::ESP::Colors::BotBox = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
                        Config::ESP::Colors::BotName = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
                        Config::ESP::Colors::BotHealth = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
                        Config::ESP::Colors::BotDistance = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
                    }
                    
                    
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Text("FOV Size: %.0f", Config::Aim::size);
                    ImGui::SliderFloat("##FOV", &Config::Aim::size, 0, 500, "%.0f");
                    
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Text("Custom Settings:");
                    ImGui::Checkbox("Render FOV", &Config::ESP::Crosshair);
                    ImGui::Checkbox("Render AimLine", &Config::ESP::Aimline);
                }
                ImGui::EndChild();
            }
            
            // TAB 2: Memory/Weapons
            else if (tab == 2) {
                static float rgb_timer = 0.0f;
                rgb_timer += ImGui::GetIO().DeltaTime * 0.5f;
                ImVec4 rgb_color = ImVec4(
                    (sin(rgb_timer) * 0.5f + 0.5f),
                    (sin(rgb_timer + 2.0f) * 0.5f + 0.5f),
                    (sin(rgb_timer + 4.0f) * 0.5f + 0.5f),
                    1.0f
                );

                ImGui::BeginChild("Memory Nav##2", ImVec2(leftPanelWidth, childHeight), true);
                {
                    ImGui::TextColored(rgb_color, "Custom Aimlock");
                    ImGui::Spacing();
                    
                    if (ImGui::Checkbox("Aimlock", &Config::Aim::AimAssist)) {
                        Config::Aim::showAimAssistSlider = Config::Aim::AimAssist;
                    }
                    
                    if (Config::Aim::showAimAssistSlider) {
                        ImGui::SliderFloat("Locking", &Config::Aim::AimAssistSize, 0, 100, "%.0f");
                        ImGui::Spacing();
                        ImGui::Separator();
                    }
                    
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::TextColored(rgb_color, "Weapon Memory V1");
                    ImGui::Spacing();
                    ImGui::Checkbox("Weapon Kinetic", &Config::Memory::Kinetic);
                    ImGui::Checkbox("No Recoil", &Config::Memory::Recoil);
                    ImGui::Checkbox("No Spread", &Config::Memory::Spread);
                    ImGui::Checkbox("No Reload", &Config::Memory::Reload);
                    ImGui::Checkbox("No Scope", &Config::Memory::Scope);
                    ImGui::Checkbox("No Switch", &Config::Memory::Switch);
                    ImGui::Checkbox("No Shake", &Config::Memory::Shake);
                    ImGui::Checkbox("No Flashbang", &Config::Memory::Flash);
                    ImGui::Separator();
                }
                ImGui::EndChild();

                ImGui::SameLine();
                ImGui::BeginChild("Memory Content##2", ImVec2(rightPanelWidth, childHeight), true);
                {
                    ImGui::TextColored(rgb_color, "Weapon Memory V2");
                    ImGui::Spacing();
                    ImGui::Checkbox("No Overheat", &Config::Memory::Overheat);
                    ImGui::Checkbox("Long Range", &Config::Memory::Range);
                    ImGui::Checkbox("Instant Shot", &Config::Memory::AutoSnipe);
                    ImGui::Checkbox("Hitbox++", &Config::Memory::Hit);
                    ImGui::Checkbox("Quick Firing", &Config::Memory::Auto);
                    ImGui::Checkbox("Brutal Firerate", &Config::Memory::Fire);
                    ImGui::Separator();
                }
                ImGui::EndChild();
            }
            
            // TAB 3: Movement/Abilities
            else if (tab == 3) {
                static float rgb_timer = 0.0f;
                rgb_timer += ImGui::GetIO().DeltaTime * 0.5f;
                ImVec4 rgb_color = ImVec4(
                    (sin(rgb_timer) * 0.5f + 0.5f),
                    (sin(rgb_timer + 2.0f) * 0.5f + 0.5f),
                    (sin(rgb_timer + 4.0f) * 0.5f + 0.5f),
                    1.0f
                );

                ImGui::BeginChild("Ability Nav##3", ImVec2(leftPanelWidth, childHeight), true);
                {
                    ImGui::TextColored(rgb_color, "Movement Abilities");
                    ImGui::Spacing();
                    
                    ImGui::Checkbox("Snowboard Boost", &Config::Movement::Snowboard);
                    ImGui::Checkbox("Long Slide", &Config::Movement::Slide);
                    ImGui::Checkbox("Speed Hack", &Config::Movement::Speed);
                    ImGui::Checkbox("High Jump", &Config::Movement::Jump);

                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::TextColored(rgb_color, "Visual Abilities");
                    ImGui::Spacing();
                    ImGui::Checkbox("Spectate No Delay", &Config::Features::Spectate);
                    ImGui::Checkbox("Wallhack Outline", &Config::Visual::Outline);
                    ImGui::Checkbox("Wallhack Glow", &Config::Visual::WallHack);
                }
                ImGui::EndChild();

                ImGui::SameLine();
                ImGui::BeginChild("Ability Content##3", ImVec2(rightPanelWidth, childHeight), true);
                {
                    ImGui::TextColored(rgb_color, "Ability Settings");
                    ImGui::Spacing();

                    if (Config::Movement::Snowboard) {
                        ImGui::SliderFloat("Snowboard Boost", &Config::Movement::SnowboardSpeed, 0.0f, 100.0f, "%.1f");
                        ImGui::Spacing();
                    }
                    if (Config::Movement::Slide) {
                        ImGui::SliderFloat("Slide Range", &Config::Movement::SlideSpeed, 0.0f, 30.0f, "%.1f");
                        ImGui::Spacing();
                    }
                    if (Config::Movement::Speed) {
                        ImGui::SliderFloat("Speed Multiplier", &Config::Movement::SpeedMultiplier, 0.5f, 10.0f, "%.1f");
                        ImGui::Spacing();
                    }
                    if (Config::Movement::Jump) {
                        ImGui::SliderFloat("Jump Height", &Config::Movement::JumpHeight, 0.5f, 5.0f, "%.1f");
                        ImGui::Spacing();
                    }

                    ImGui::Separator();
                    ImGui::TextColored(rgb_color, "Additional Features");
                    ImGui::Spacing();
                    ImGui::Checkbox("Jet Boost + Jump", &Config::Features::JetJump);
                    ImGui::Checkbox("Jet Boost + Slide", &Config::Features::JetSlide);
                    ImGui::Checkbox("Long Execute", &isExecute);
                    ImGui::Checkbox("Radar MP", &Config::Features::Radar);
                    ImGui::Checkbox("No Parachute", &Config::Features::Parachute);
                    ImGui::Checkbox("Sky Diving Speed", &Config::Features::Diving);
                }
                ImGui::EndChild();
            }
            
            // TAB 4: Aimbot
            else if (tab == 4) {
                ImGui::BeginChild("Aim Nav##4", ImVec2(leftPanelWidth, childHeight), true);
                ImGui::Checkbox("Aimbot 360", &Config::Aim::Aimbot360);
                ImGui::Checkbox("Bullet Track", &Config::Aim::AimSilent);
                ImGui::Checkbox("Fast Revival Time", &Config::Features::Revive);
                ImGui::Checkbox("Fast Medicine Time", &Config::Features::Medicine);
                ImGui::EndChild();

                ImGui::SameLine();
                ImGui::BeginChild("Aim Content##4", ImVec2(rightPanelWidth, childHeight), true);
                ImGui::Text("Location");
                const char* targets[] = { "Head", "Chest", "Body" };
                for (int i = 0; i < IM_ARRAYSIZE(targets); i++) {
                    if (ImGui::RadioButton(targets[i], static_cast<int>(Config::Aim::Target) == i))
                        Config::Aim::Target = static_cast<EAimTarget>(i);
                    if (i < 2) ImGui::SameLine();
                }
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Text("Trigger");
                const char* triggers[] = { "None", "Shooting", "Scoping" };
                for (int i = 0; i < IM_ARRAYSIZE(triggers); i++) {
                    if (ImGui::RadioButton(triggers[i], static_cast<int>(Config::Aim::Trigger) == i))
                        Config::Aim::Trigger = static_cast<EAimTrigger>(i);
                    if (i < 2) ImGui::SameLine();
                }
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Text("Target Priority");
                const char* tarets[] = { "Distance", "FOV" };
                for (int i = 0; i < IM_ARRAYSIZE(tarets); i++) {
                    if (ImGui::RadioButton(tarets[i], static_cast<int>(Config::Aim::By) == i))
                        Config::Aim::By = static_cast<EAim>(i);
                    if (i < 1) ImGui::SameLine();
                }
                ImGui::EndChild();
            }
            
            // TAB 5: Settings
            else if (tab == 5) {
                static int skinTab = 0;
                ImVec2 tabSize = ImVec2(70, 28);
                float footerHeight = 40.0f;
                float listHeight = childHeight - footerHeight - 35.0f;

                ImGui::BeginChild("SkinsTabs", ImVec2(-1, 30), false);
                if (ImGui::Selectable("Weapon", skinTab == 0, 0, tabSize)) skinTab = 0;
                ImGui::SameLine();
                if (ImGui::Selectable("Skins", skinTab == 1, 0, tabSize)) skinTab = 1;
                ImGui::SameLine();
                if (ImGui::Selectable("Plane", skinTab == 2, 0, tabSize)) skinTab = 2;
                ImGui::SameLine();
                if (ImGui::Selectable("Watch", skinTab == 3, 0, tabSize)) skinTab = 3;
                ImGui::SameLine();
                if (ImGui::Selectable("Box", skinTab == 4, 0, tabSize)) skinTab = 4;
                ImGui::EndChild();

                ImGui::BeginChild("SkinsList", ImVec2(leftPanelWidth, listHeight), true);
                if (skinTab == 0) {
                    if (!itemData.empty()) {
                        std::string searchLower = ToLower(searchQuery);
                        for (const auto& getitem : itemData) {
                            std::string itemNameLower = ToLower(getitem.itemName);
                            if (!searchLower.empty() && itemNameLower.find(searchLower) == std::string::npos) continue;
                            ImGui::Checkbox(getitem.itemName.c_str(), &sBool[getitem.itemName]);
                            if (sBool[getitem.itemName]) {
                                int baseWeaponID = getitem.WeaponConf[0];
                                for (auto item : itemInventoryInstance) {
                                    if (!item) continue;
                                    item2Fields = (Item2InventoryFields*) ((uintptr_t) item + 0x20);
                                    if (item2Fields->ItemID == getitem.Item2Inventory[0] || item2Fields->ItemID == getitem.Item2Inventory[3]) {
                                        item2Fields->WeaponAssetGroupID = getitem.Item2Inventory[1];
                                        item2Fields->WeaponIconID = getitem.Item2Inventory[2];
                                    }
                                }
                                for (auto conf : weaponConfInstance) {
                                    if (!conf) continue;
                                    weaponconfFields = (WeaponConfFields*) ((uintptr_t) conf + 0x20);
                                    if (getitem.WeaponConf[0] == weaponconfFields->ID || getitem.Item2Inventory[3] == weaponconfFields->ID) {
                                        weaponconfFields->ColorID = getitem.WeaponConf[1];
                                        weaponconfFields->DefWeaponSkinID = getitem.WeaponConf[2];
                                        weaponconfFields->DefaultKillBrocast = getitem.WeaponConf[3];
                                        weaponconfFields->WinnerCircleWeaponMountType = getitem.WeaponConf[4];
                                    }
                                }
                                if (getitem.WeaponExtra[4] > 0) activeKillEffects[baseWeaponID] = getitem.WeaponExtra[4];
                                for (auto extra : weaponExtraInstance) {
                                    if (!extra) continue;
                                    weaponextraFields = (WeaponConfExtraFields*) ((uintptr_t) extra + 0x10);
                                    if (weaponextraFields->ID == getitem.WeaponExtra[2] && weaponextraFields->ID == getitem.WeaponConf[0]) {
                                        if (getitem.itemName.find("[M]") != std::string::npos) {
                                            weaponextraFields->DefaultMythicArmor = getitem.WeaponExtra[1];
                                            weaponextraFields->DefaultMythicSig = getitem.WeaponExtra[2];
                                        }
                                        weaponextraFields->DefaultDeadReplayEffectId = getitem.WeaponExtra[3];
                                        weaponextraFields->DefaultKillEffectId = getitem.WeaponExtra[4];
                                    }
                                }
                                for (auto asset : weaponAssetGroupInstance) {
                                    if (!asset) continue;
                                    weaponAssetFields = (WeaponAssetGroupFields*) ((uintptr_t) asset + 0x40);
                                    if (weaponAssetFields->Id == getitem.WeaponAsset[0]) {
                                        if (getitem.WeaponAsset[1] / 10 == getitem.WeaponAsset[2] / 10 && getitem.WeaponAsset[1] > 0) {
                                            weaponAssetFields->FireEffectGroupID = getitem.WeaponAsset[1];
                                        }
                                    }
                                    for (auto itemResource : itemResourceConfigInstance) {
                                        if (!itemResource) continue;
                                        itemFields = (ItemResourceFields*) ((uintptr_t) itemResource + 0x10);
                                        if (itemFields->ID == getitem.Item2Inventory[0]) {
                                            itemFields->FxAssetID = getitem.ItemResInt[0];
                                            itemFields->InventoryModelID = getitem.ItemResInt[1];
                                            itemFields->ModelAssetIDRaw = getitem.ItemResInt[2];
                                            itemFields->UIMiniSpriteName = getitem.ItemRes[0];
                                            itemFields->UISmallSpriteName = getitem.ItemRes[1];
                                            itemFields->UISpriteName = getitem.ItemRes[2];
                                            itemFields->UISquareSpriteName = getitem.ItemRes[3];
                                        }
                                    }
                                }
                                sBool[getitem.itemName] = false;
                            }
                        }
                    }
                } else if (skinTab == 1) {
                    if (!charData.empty()) {
                        std::string searchLower = ToLower(searchQuery);
                        for (const auto& getchar : charData) {
                            std::string itemNameLower = ToLower(getchar.charName);
                            if (!searchLower.empty() && itemNameLower.find(searchLower) == std::string::npos) continue;
                            ImGui::Checkbox(getchar.charName.c_str(), &sBool[getchar.charName]);
                            if (sBool[getchar.charName]) {
                                if (CharacterModelConfigInstance.size() > 0 && itemResourceConfigInstance.size() > 0 && RoleConfConfigInstance.size() > 0 && RolePackConfConfigInstance.size() > 0 && BRDeadboxSkinConfigInstance.size() > 0) {
                                    for (auto charModel : CharacterModelConfigInstance) {
                                        if (!charModel) continue;
                                        characterfields = (CharacterModelFields *)((uintptr_t)charModel + 0x10);
                                        auto Trait = characterfields->Traitor1P;
                                        if (Trait == 710001101) {
                                            characterfields->BRBagModel = getchar.charModel[0];
                                            characterfields->BRHeadModel = getchar.charModel[1];
                                            characterfields->BRLobby = getchar.charModel[2];
                                            characterfields->BRModel = getchar.charModel[3];
                                            characterfields->BindEffect1P = getchar.charModel[4];
                                            characterfields->ChangeClipEffect1P = getchar.charModel[5];
                                            characterfields->DefaultModelID = getchar.charModel[6];
                                            characterfields->Guarder1P = getchar.charModel[7];
                                            characterfields->Guarder3P = getchar.charModel[8];
                                            characterfields->GuarderBagModel = getchar.charModel[9];
                                            characterfields->GuarderHeadModel = getchar.charModel[10];
                                            characterfields->GuarderLobby = getchar.charModel[11];
                                        }
                                    }
                                    for (auto itemRes : itemResourceConfigInstance) {
                                        if (!itemRes) continue;
                                        itemFields = (ItemResourceFields*)((uintptr_t)itemRes + 0x10);
                                        if (itemFields->ID == 100301208) {
                                            itemFields->FxAssetID = getchar.charRes[0];
                                            itemFields->InventoryModelID = getchar.charRes[1];
                                            itemFields->ModelAssetIDRaw = getchar.charRes[2];
                                            itemFields->UIMiniSpriteName = getchar.charRes2[0];
                                            itemFields->UISmallSpriteName = getchar.charRes2[1];
                                            itemFields->UISpriteName = getchar.charRes2[2];
                                            itemFields->UISquareSpriteName = getchar.charRes2[3];
                                        }
                                    }
                                    for (auto roles : RoleConfConfigInstance) {
                                        if (!roles) continue;
                                        RoleConfFields *roleF = (RoleConfFields*)((uintptr_t)roles + 0x14);
                                        if (roleF->ID == 100301208) {
                                            roleF->roleLeftArmID = getchar.charRole[0];
                                            roleF->roleFinalSuitID = getchar.charRole[1];
                                            roleF->roleBasicHologramID = getchar.charRole[2];
                                            roleF->ColorID = getchar.charRole[3];
                                            roleF->ColorSubID = getchar.charRole[4];
                                            roleF->ShowRare = getchar.charRole[5];
                                            roleF->RoleLvGroupID = getchar.charRole[6];
                                            roleF->RolePackID = getchar.charRole[7];
                                        }
                                    }
                                    for (auto pack : RolePackConfConfigInstance) {
                                        if (!pack) continue;
                                        packfields = (RolePackFields*)((uintptr_t)pack + 0x10);
                                        if (packfields->RolePackID == getchar.charPack[0] - 1) {
                                            if (packfields->LoadingFrame != 0 && packfields->LobbySceneType != 0) {
                                                packfields->LoadingFrame = getchar.charPack[1];
                                            }
                                        }
                                    }
                                }
                                sBool[getchar.charName] = false;
                            }
                        }
                    }
                } else if (skinTab == 2) {
                    if (dropplane.size() > 0) {
                        for (const auto& planex : dropplane) {
                            ImGui::Checkbox(planex.planename.c_str(), &sBool[planex.planename]);
                            if (sBool[planex.planename]) {
                                if (BRDropPlaneSkinConfigInstance.size() > 0) {
                                    for (auto planeID : BRDropPlaneSkinConfigInstance) {
                                        if (!planeID) continue;
                                        dropplaneFields = (BRDropPlaneSkinFields*)((uintptr_t)planeID + 0x10);
                                        if (dropplaneFields->ID == 0) {
                                            dropplaneFields->ColorID = planex.plane[0];
                                            dropplaneFields->ModelAsset1P = planex.plane[1];
                                            dropplaneFields->ModelAsset3P = planex.plane[2];
                                            dropplaneFields->ModelAssetCutScene = planex.plane[3];
                                            dropplaneFields->ModelAssetUI = planex.plane[4];
                                            dropplaneFields->Priority = planex.plane[5];
                                        }
                                    }
                                }
                                sBool[planex.planename] = false;
                            }
                        }
                    } else {
                        ImGui::TextWrapped("Goto [Safehouse] > [Arsenal] > [Operators]");
                    }
                } else if (skinTab == 3) {
                    if (watch.size() > 0) {
                        for (const auto& forwatch : watch) {
                            ImGui::Checkbox(forwatch.watchname.c_str(), &sBool[forwatch.watchname]);
                            if (sBool[forwatch.watchname]) {
                                if (CharacterModelConfigInstance.size() > 0) {
                                    for (auto charModel : CharacterModelConfigInstance) {
                                        if (!charModel) continue;
                                        characterfields = (CharacterModelFields *)((uintptr_t)charModel + 0x10);
                                        auto Trait = characterfields->Traitor1P;
                                        if (Trait == 710001101) {
                                            characterfields->BindEffect1P = forwatch.watchvalue;
                                        }
                                    }
                                }
                                sBool[forwatch.watchname] = false;
                            }
                        }
                    }
                } else if (skinTab == 4) {
                    if (deadboxF.size() > 0) {
                        for (const auto& deadx : deadboxF) {
                            ImGui::Checkbox(deadx.deadname.c_str(), &sBool[deadx.deadname]);
                            if (sBool[deadx.deadname]) {
                                if (CharacterModelConfigInstance.size() > 0) {
                                    for (auto deadID : BRDeadboxSkinConfigInstance) {
                                        if (!deadID) continue;
                                        deadboxFields = (BRDeadboxSkinFields*)((uintptr_t)deadID + 0x10);
                                        if (deadboxFields->ID == 180300004) {
                                            deadboxFields->ColorID = deadx.dead[0];
                                            deadboxFields->DeadBoxEffectAsset = deadx.dead[1];
                                            deadboxFields->Flag = deadx.dead[2];
                                            deadboxFields->FlagAsset = deadx.dead[3];
                                            deadboxFields->ModelAsset3P = deadx.dead[4];
                                            deadboxFields->ModelAssetUI = deadx.dead[5];
                                        }
                                    }
                                }
                                sBool[deadx.deadname] = false;
                            }
                        }
                    }
                }
                ImGui::EndChild();

                ImGui::SameLine();
                ImGui::BeginChild("SkinsFooter", ImVec2(rightPanelWidth, childHeight), true);
                ImGui::Text("Search");
                ImGui::InputText("##Search", searchQuery, IM_ARRAYSIZE(searchQuery));
                ImGui::EndChild();
            }
        }
        ImGui::End();
    }

    auto Input_get_touchCount = (int (*)())(Class_Input__get_touchCount);
    if (Input_get_touchCount() > 0) {
        auto Input_GetTouch = (Touch(*)(uintptr_t, int))(Class_Input__GetTouch);
        auto Input_get_mousePosition = (Vector3(*)(uintptr_t))(Class_Input__get_mousePosition);
        switch (Input_GetTouch(0, 0).m_Phase) {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io->MouseDown[0] = true;
                io->MousePos = ImVec2(Input_get_mousePosition(0).x, get_height() - Input_get_mousePosition(0).y);
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io->MouseDown[0] = false;
                break;
            case TouchPhase::Moved:
                io->MousePos = ImVec2(Input_get_mousePosition(0).x, get_height() - Input_get_mousePosition(0).y);
                break;
            default: break;
        }
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

void Init_Thread()
{
    while (!m_unity) {
        m_unity = Tools::GetBaseAddress("libunity.so");
        sleep(3); // let Unity + EGL fully init
    }
    
    UpdateAllOffsets();
    LOGI("libunity.so: %p", m_unity);
    
    HOOK_LIB("libunity.so", "0xA9E4D04", ESP::CreateBulletLine, ESP::old_CreateBulletLine);
    HOOK_LIB("libunity.so", "0xA9F0AC0", ESP::CreateBulletProjectile, ESP::old_CreateBulletProjectile);
    
    // Constructor hooks
    hookConstructor((void*)getRealOffset(Item2InventoryAddress), itemInventoryInstance);
    hookConstructor((void*)getRealOffset(WeaponConfExtraAddress), weaponExtraInstance);
    hookConstructor((void*)getRealOffset(WeaponFireEffectAddress), weaponFireEffectInstance);
    hookConstructor((void*)getRealOffset(WeaponConfAddress), weaponConfInstance);
    hookConstructor((void*)getRealOffset(WeaponAssetGroupAddress), weaponAssetGroupInstance);
    hookConstructor((void*)getRealOffset(MythicArmorConfigAddress), mythicArmorInstance);
    hookConstructor((void*)getRealOffset(MythicSightConfigAddress), mythicSightInstance);
    hookConstructor((void*)getRealOffset(WeaponSkinAddress), weaponSkinConfigInstance);
    hookConstructor((void*)getRealOffset(KillEffectItemConfConfigAddress), killEffectItemInstance);
    hookConstructor((void*)getRealOffset(ItemResourceAddress), itemResourceConfigInstance);
    hookConstructor((void*)getRealOffset(CharacterModelAddress), CharacterModelConfigInstance);
    hookConstructor((void*)getRealOffset(RoleConfAddress), RoleConfConfigInstance);
    hookConstructor((void*)getRealOffset(RoleSkinAddress), RoleSkinConfigInstance);
    hookConstructor((void*)getRealOffset(RolePackConfAddress), RolePackConfConfigInstance);
    hookConstructor((void*)getRealOffset(BRDeadboxSkinAddress), BRDeadboxSkinConfigInstance);
    hookConstructor((void*)getRealOffset(BRDropPlaneSkinAddress), BRDropPlaneSkinConfigInstance);
    
    pthread_t t;
    pthread_create(&t, NULL, GameUpdate, NULL);

    Tools::Hook((void*)KittyMemory::getAbsoluteAddress("libunity.so", GetCurrentWeaponAddress), (void*)_GetCurrentWeaponKillEffect, (void**)&orig_GetCurrentWeaponKillEffect);

    auto swapBuffers = (uintptr_t) DobbySymbolResolver("libEGL.so", "eglSwapBuffers");
    KittyMemory::ProtectAddr((void *)swapBuffers, sizeof(swapBuffers), PROT_READ | PROT_WRITE | PROT_EXEC);
    DobbyHook((void *) swapBuffers, (void *) hook_eglSwapBuffers, (void **) &old_eglSwapBuffers);
}

const char* packageName = "com.activision.callofduty.shooter";
const char* gameVersion = "1.0.54";

extern "C"
JNIEXPORT jstring JNICALL
Java_com_magic_loader_Main_getSupportedPackage(JNIEnv* env, jclass clazz) {
    return env->NewStringUTF(packageName);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_magic_loader_Main_getSupportedVersion(JNIEnv* env, jclass clazz) {
    return env->NewStringUTF(gameVersion);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_magic_loader_Main_checkVersionMatch(JNIEnv* env, jclass clazz, jstring installedVersion) {
    const char* installed = env->GetStringUTFChars(installedVersion, nullptr);
    bool match = (installed != nullptr && strcmp(installed, gameVersion) == 0);
    if (installed) env->ReleaseStringUTFChars(installedVersion, installed);
    return match ? JNI_TRUE : JNI_FALSE;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_loader_Main_start(JNIEnv* env, jclass clazz) {
    LOGI("Start() called - preparing to launch game");
    std::thread(Init_Thread).detach();
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI loaded");
    return JNI_VERSION_1_6;
}
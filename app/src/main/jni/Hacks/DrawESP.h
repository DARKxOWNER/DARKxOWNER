#pragma once

#include "FTools/Iconcpp.h"
#include "FTools/Font.h"
#include "FTools/Icon.h"

namespace ESP {

bool isInsideScreen(const Vector3& screenPos);
bool isInsideFOV(const Vector3& screenPos, float fovSize);
uintptr_t GetClosestTarget();
uintptr_t GetInsideFOVTarget();

static bool g_isEspReady = false;
static int g_totalBots = 0;
static int g_totalEnemies = 0;

ImU32 ToColor(const ImVec4& col) {
    return IM_COL32((int)(col.x * 255), (int)(col.y * 255), (int)(col.z * 255), (int)(col.w * 255));
}

Vector2 PushToScreenBorder(const Vector2& pos, const Vector2& screen, int borders, int offset) {
    int x = static_cast<int>(pos.x);
    int y = static_cast<int>(pos.y);
    if (borders & 1) y = 0 - offset;
    if (borders & 2) x = static_cast<int>(screen.x) + offset;
    if (borders & 4) y = static_cast<int>(screen.y) + offset;
    if (borders & 8) x = 0 - offset;
    return Vector2(static_cast<float>(x), static_cast<float>(y));
}

int IsOutsideSafezone(const Vector2& pos, const Vector2& screen) {
    Vector2 safezoneTopLeft(screen.x * 0.04f, screen.y * 0.04f);
    Vector2 safezoneBottomRight(screen.x * 0.96f, screen.y * 0.96f);

    int result = 0;
    if (pos.y < safezoneTopLeft.y) result |= 1;
    if (pos.x > safezoneBottomRight.x) result |= 2;
    if (pos.y > safezoneBottomRight.y) result |= 4;
    if (pos.x < safezoneTopLeft.x) result |= 8;
    return result;
}

uintptr_t GetClosestTarget() {
    uintptr_t result = 0;
    const float MAX_AIM_DISTANCE = 50.0f;
    float minDist = MAX_AIM_DISTANCE;

    auto localPawn = GamePlay::get_LocalPawn();
    if (localPawn) {
        auto myPos = localPawn->get_Position();
        auto enemies = GamePlay::get_AliveEnemies();
        
        for (auto pawn : enemies) {
            auto pos = pawn->get_Position();
            float dist = Vector3::Distance(myPos, pos);
            if (dist < minDist && dist <= MAX_AIM_DISTANCE) {
                result = reinterpret_cast<uintptr_t>(pawn);
                minDist = dist;
            }
        }
    }
    return result;
}

bool IsInsideFOVCheck(float x, float y, float fovSize) {
    if (fovSize <= 0.0f) return true;
    float centerX = static_cast<float>(get_width()) / 2.0f;
    float centerY = static_cast<float>(get_height()) / 2.0f;
    float dx = x - centerX;
    float dy = y - centerY;
    return (dx * dx + dy * dy) <= (fovSize * fovSize);
}

uintptr_t GetInsideFOVTarget() {
    uintptr_t result = 0;
    float maxDist = std::numeric_limits<float>::infinity();
    float fovSize = Config::Aim::size;

    auto localPawn = GamePlay::get_LocalPawn();
    if (localPawn) {
        auto enemies = GamePlay::get_AliveEnemies();
        
        for (auto pawn : enemies) {
            auto headPos = pawn->get_HeadPosition();
            auto screenPos = WorldToScreenPoint(headPos);
            
            if (IsInsideFOVCheck(screenPos.x, screenPos.y, fovSize)) {
                float centerX = static_cast<float>(get_width()) / 2.0f;
                float centerY = static_cast<float>(get_height()) / 2.0f;
                float dx = screenPos.x - centerX;
                float dy = screenPos.y - centerY;
                float distance = dx * dx + dy * dy;
                
                if (distance < maxDist) {
                    result = reinterpret_cast<uintptr_t>(pawn);
                    maxDist = distance;
                }
            }
        }
    }
    return result;
}

void DrawPlayerBox(ImDrawList* draw, const ImVec2& start, const ImVec2& end, const ImVec4& color) {
    ImU32 col = ToColor(color);
    draw->AddRectFilled(start, end, IM_COL32((int)(color.x * 255), (int)(color.y * 255), (int)(color.z * 255), 25));
    draw->AddRect(start, end, col, 4.0f, 0, 1.2f);
}

void DrawHealthBar(ImDrawList* draw, const ImVec2& pos, float width, float height, int currentHP, int maxHP) {
    if (maxHP <= 0) return;
    
    int r = std::min(((510 * (maxHP - currentHP)) / maxHP), 255);
    int g = std::min(((510 * currentHP) / maxHP), 255);
    ImU32 hpColor = IM_COL32(r, g, 0, 155);
    
    float fillWidth = (static_cast<float>(currentHP) * width) / static_cast<float>(maxHP);
    ImVec2 fillEnd = ImVec2(pos.x + fillWidth, pos.y + height);
    ImVec2 rectEnd = ImVec2(pos.x + width, pos.y + height);
    
    draw->AddRectFilled(pos, fillEnd, hpColor);
    draw->AddRect(pos, rectEnd, IM_COL32(0, 0, 0, 155));
}

void DrawAimLine(ImDrawList* draw, int screenWidth, int screenHeight) {
    if (!Config::ESP::Aimline) return;
    
    ImVec2 center(static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f);
    auto target = GetInsideFOVTarget();
    
    if (target) {
        auto pawn = reinterpret_cast<Pawn*>(target);
        auto headPos = pawn->get_HeadPosition();
        auto screenPos = WorldToScreenPoint(headPos);
        
        if (screenPos.z > 0) {
            draw->AddLine(center, ImVec2(screenPos.x, static_cast<float>(screenHeight) - screenPos.y), 
                         ToColor(Config::ESP::Colors::PlayerLine), 2.0f);
        }
    }
}

void DrawCrosshair(ImDrawList* draw, int screenWidth, int screenHeight) {
    if (!Config::ESP::Crosshair) return;
    
    ImVec2 center(static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f);
    float radius = Config::Aim::size;
    ImU32 color = IM_COL32(255, 255, 255, 200);
    draw->AddCircle(center, radius, color, 60, 1.5f);
}

void DrawEnemyCounter(ImDrawList* draw, int screenWidth, int screenHeight) {
    if (!Config::ESP::Count || !Config::ESP::Enabled) return;
    
    if (g_totalBots == 0 && g_totalEnemies == 0) return;
    
    float centerX = static_cast<float>(screenWidth) / 2.0f;
    float topY = 30.0f;
    float tileWidth = 100.0f;
    float tileHeight = 45.0f;
    float spacing = 15.0f;
    
    ImVec2 enemyPos1(centerX - tileWidth - spacing, topY);
    ImVec2 enemyPos2(enemyPos1.x + tileWidth, topY + tileHeight);
    ImVec2 botPos1(centerX + spacing, topY);
    ImVec2 botPos2(botPos1.x + tileWidth, topY + tileHeight);
    
    ImU32 enemyColor = IM_COL32(255, 60, 80, 140);
    ImU32 botColor = IM_COL32(40, 255, 100, 140);
    ImU32 borderColor = IM_COL32(255, 255, 255, 35);
    ImU32 textColor = IM_COL32(255, 255, 255, 255);
    
    char buffer[32];
    
    draw->AddRectFilled(enemyPos1, enemyPos2, enemyColor, 12.0f);
    draw->AddRect(enemyPos1, enemyPos2, borderColor, 12.0f, 0, 1.4f);
    snprintf(buffer, sizeof(buffer), ICON_FA_USERS " %d", g_totalEnemies);
    ImVec2 enemyTextSize = ImGui::CalcTextSize(buffer);
    draw->AddText(ImVec2(enemyPos1.x + (tileWidth - enemyTextSize.x) / 2, topY + 12), textColor, buffer);
    
    draw->AddRectFilled(botPos1, botPos2, botColor, 12.0f);
    draw->AddRect(botPos1, botPos2, borderColor, 12.0f, 0, 1.4f);
    snprintf(buffer, sizeof(buffer), ICON_FA_DESKTOP " %d", g_totalBots);
    ImVec2 botTextSize = ImGui::CalcTextSize(buffer);
    draw->AddText(ImVec2(botPos1.x + (tileWidth - botTextSize.x) / 2, topY + 12), textColor, buffer);
}

void DrawESP(ImDrawList* draw, int screenWidth, int screenHeight, float density) {
    if (!Config::ESP::Enabled) return;
    
    auto mainCamera = Camera::get_main();
    if (!mainCamera) return;
    
    auto matchGame = GamePlay::get_MatchGame();
    if (!matchGame) return;
    
    auto localPawn = GamePlay::get_LocalPawn();
    if (!localPawn) return;
    
    Vector3 myPos(0, 0, 0);
    auto localMesh = localPawn->get_Mesh();
    if (localMesh) {
        myPos = localMesh->get_position();
    }
    
    // Reset counters
    if (!g_isEspReady) {
        g_totalBots = 0;
        g_totalEnemies = 0;
        g_isEspReady = true;
    } else {
        g_totalBots = 0;
        g_totalEnemies = 0;
    }
    
    auto enemyList = GamePlay::get_EnemyPawns();
    if (!enemyList || enemyList->getSize() == 0) return;
    
    for (int i = 0; i < enemyList->getSize(); i++) {
        auto pawn = reinterpret_cast<Pawn*>(enemyList->getItems()[i]);
        if (!pawn || !pawn->is_Alive()) continue;
        
        auto playerInfo = pawn->get_PlayerInfo();
        if (!playerInfo) continue;
        
        bool isBot = pawn->is_Bot();
        if (isBot) {
            g_totalBots++;
        } else {
            g_totalEnemies++;
        }
        
        auto headPos = pawn->get_HeadPosition();
        auto rootPos = pawn->get_Position();
        auto headScreen = WorldToScreenPoint(headPos);
        auto rootScreen = WorldToScreenPoint(rootPos);
        
        float distance = Vector3::Distance(myPos, headPos);
        if (distance > 500.0f || headScreen.z <= 0 || rootScreen.z <= 0) continue;
        
        float headX = headScreen.x;
        float headY = static_cast<float>(screenHeight) - headScreen.y;
        float rootY = static_cast<float>(screenHeight) - rootScreen.y;
        
        // Draw line to player
        if (Config::ESP::isPlayerLine) {
            ImVec2 center(static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f);
            ImU32 lineColor = isBot ? ToColor(Config::ESP::Colors::BotLine) : ToColor(Config::ESP::Colors::PlayerLine);
            draw->AddLine(center, ImVec2(headX, headY), lineColor, Config::ESP::lineSize);
        }
        
        // Draw box
        if (Config::ESP::Box) {
            float boxHeight = std::abs(headScreen.y - rootScreen.y);
            float boxWidth = boxHeight * 0.65f;
            ImVec2 boxStart(headX - (boxWidth / 2.0f), headY);
            ImVec2 boxEnd(boxStart.x + boxWidth, boxStart.y + boxHeight);
            
            ImU32 boxColor = isBot ? ToColor(Config::ESP::Colors::BotBox) : ToColor(Config::ESP::Colors::PlayerBox);
            draw->AddRectFilled(boxStart, boxEnd, IM_COL32((int)(boxColor >> 16 & 0xFF), (int)(boxColor >> 8 & 0xFF), (int)(boxColor & 0xFF), 25));
            draw->AddRect(boxStart, boxEnd, boxColor, 4.0f, 0, 1.2f);
        }
        
        // Draw health
        if (Config::ESP::Health) {
            int currentHP = pawn->get_Health();
            int maxHP = pawn->get_MaxHealth();
            
            float boxHeight = std::abs(headScreen.y - rootScreen.y);
            float boxWidth = boxHeight * 0.65f;
            float barWidth = boxWidth;
            float barHeight = barWidth * 0.07f;
            
            ImVec2 barPos(headX - (barWidth / 2.0f), headY - (barHeight * 2.1f));
            DrawHealthBar(draw, barPos, barWidth, barHeight, currentHP, maxHP);
        }
        
        // Draw name
        if (Config::ESP::Name) {
            const char* name = pawn->get_Name();
            if (!name || strlen(name) == 0) name = "UNKNOWN";
            
            float boxHeight = std::abs(headScreen.y - rootScreen.y);
            float boxWidth = boxHeight * 0.65f;
            float textY = headY - (boxWidth * 0.19f * 1.83f);
            
            ImU32 nameColor = isBot ? ToColor(Config::ESP::Colors::BotName) : ToColor(Config::ESP::Colors::PlayerName);
            draw->AddText(nullptr, density / 30.0f, ImVec2(headX - (boxWidth / 3.0f), textY), nameColor, name);
        }
        
        // Draw distance
        if (Config::ESP::Distance) {
            float boxHeight = std::abs(headScreen.y - rootScreen.y);
            float boxWidth = boxHeight * 0.65f;
            float textY = headY - (boxWidth * 0.19f * 1.83f);
            
            char distStr[16];
            snprintf(distStr, sizeof(distStr), "%.0fM", distance);
            
            ImU32 distColor = isBot ? ToColor(Config::ESP::Colors::BotDistance) : ToColor(Config::ESP::Colors::PlayerDistance);
            draw->AddText(nullptr, density / 30.0f, ImVec2(headX + (boxWidth / 3.0f), textY), distColor, distStr);
        }
        
        // Draw 360 alert
        if (Config::ESP::Alert && IsOutsideSafezone(Vector2(headX, headY), Vector2(static_cast<float>(screenWidth), static_cast<float>(screenHeight)))) {
            Vector2 hintDot = PushToScreenBorder(Vector2(headX, headY), Vector2(static_cast<float>(screenWidth), static_cast<float>(screenHeight)), 
                                                  static_cast<int>((screenHeight / 1080.0f * 100.0f) / 3.0f), 0);
            Vector2 hintText = PushToScreenBorder(Vector2(headX, headY), Vector2(static_cast<float>(screenWidth), static_cast<float>(screenHeight)), 
                                                   -static_cast<int>(screenHeight / 1080.0f * 36.0f), 0);
            
            char distStr[16];
            snprintf(distStr, sizeof(distStr), "%.0fM", distance);
            
            draw->AddCircleFilled(ImVec2(hintDot.x, hintDot.y), screenHeight / 1080.0f * 100.0f, IM_COL32(255, 128, 0, 128), 0);
            draw->AddText(nullptr, 25.0f, ImVec2(hintText.x - 20.0f, hintText.y - 15.0f), IM_COL32(255, 255, 255, 255), distStr);
        }
    }
    
    DrawCrosshair(draw, screenWidth, screenHeight);
    DrawAimLine(draw, screenWidth, screenHeight);
    DrawEnemyCounter(draw, screenWidth, screenHeight);
}

// AimBot Function
void AimBotFunc() {
    if (!Config::Aim::Aimbot360) return;
    
    auto matchGame = GamePlay::get_MatchGame();
    if (!matchGame) return;
    
    auto localPawn = GamePlay::get_LocalPawn();
    if (!localPawn) return;
    
    bool triggerReady = (Config::Aim::Trigger == EAimTrigger::None);
    
    if (Config::Aim::Trigger == EAimTrigger::Shooting) {
        triggerReady = localPawn->is_Firing();
    } else if (Config::Aim::Trigger == EAimTrigger::Scoping) {
        triggerReady = localPawn->is_Aiming();
    }
    
    if (!triggerReady) return;
    
    uintptr_t target = 0;
    if (Config::Aim::By == EAim::Distance) {
        target = GetClosestTarget();
    } else if (Config::Aim::By == EAim::FOV) {
        target = GetInsideFOVTarget();
    }
    
    if (!target) return;
    
    auto targetPawn = reinterpret_cast<Pawn*>(target);
    Vector3 targetPos;
    
    switch (Config::Aim::Target) {
        case EAimTarget::Head:
            targetPos = targetPawn->get_HeadPosition();
            break;
        case EAimTarget::Chest:
            targetPos = targetPawn->get_HeadPosition();
            targetPos.y -= 0.2f;
            break;
        case EAimTarget::Body:
            targetPos = targetPawn->get_HeadPosition();
            targetPos.y -= 0.4f;
            break;
        default:
            targetPos = targetPawn->get_HeadPosition();
            break;
    }
    
    auto mainCamera = Camera::get_main();
    if (mainCamera) {
        auto cameraTransform = reinterpret_cast<Component*>(mainCamera)->get_transform();
        if (cameraTransform) {
            localPawn->set_AimRotation(Quaternion::LookRotation(targetPos - cameraTransform->get_position(), Vector3::Up()));
        }
    }
}

// Bullet track hooks
void (*old_CreateBulletLine)(uintptr_t thiz, Vector3 startPos, Vector3 dir, bool isDualFire);
void CreateBulletLine(uintptr_t thiz, Vector3 startPos, Vector3 dir, bool isDualFire) {
    if (Config::Aim::AimSilent) {
        auto localPawn = GamePlay::get_LocalPawn();
        if (localPawn) {
            bool triggerReady = (Config::Aim::Trigger == EAimTrigger::None);
            if (Config::Aim::Trigger == EAimTrigger::Shooting) {
                triggerReady = localPawn->is_Firing();
            } else if (Config::Aim::Trigger == EAimTrigger::Scoping) {
                triggerReady = localPawn->is_Aiming();
            }
            
            if (triggerReady) {
                uintptr_t target = 0;
                if (Config::Aim::By == EAim::Distance) {
                    target = GetClosestTarget();
                } else if (Config::Aim::By == EAim::FOV) {
                    target = GetInsideFOVTarget();
                }
                
                if (target) {
                    auto pawn = reinterpret_cast<Pawn*>(target);
                    Vector3 targetPos;
                    
                    switch (Config::Aim::Target) {
                        case EAimTarget::Head: targetPos = pawn->get_HeadPosition(); break;
                        case EAimTarget::Chest: targetPos = pawn->get_HeadPosition(); targetPos.y -= 0.2f; break;
                        default: targetPos = pawn->get_HeadPosition(); targetPos.y -= 0.4f; break;
                    }
                    
                    auto mainCamera = Camera::get_main();
                    if (mainCamera) {
                        auto transform = reinterpret_cast<Component*>(mainCamera)->get_transform();
                        if (transform) {
                            dir = targetPos - transform->get_position();
                        }
                    }
                }
            }
        }
    }
    old_CreateBulletLine(thiz, startPos, dir, isDualFire);
}

void (*old_CreateBulletProjectile)(void* thiz, Vector3 startPos, Vector3 dir, void* weaponImpact, int itemID, int flySmokeAssetID, bool enableVirtualStartPos, Vector3 virtualStartPos);
void CreateBulletProjectile(void* thiz, Vector3 startPos, Vector3 dir, void* weaponImpact, int itemID, int flySmokeAssetID, bool enableVirtualStartPos, Vector3 virtualStartPos) {
    if (Config::Aim::AimSilent) {
        auto localPawn = GamePlay::get_LocalPawn();
        if (localPawn) {
            bool triggerReady = (Config::Aim::Trigger == EAimTrigger::None);
            if (Config::Aim::Trigger == EAimTrigger::Shooting) {
                triggerReady = localPawn->is_Firing();
            } else if (Config::Aim::Trigger == EAimTrigger::Scoping) {
                triggerReady = localPawn->is_Aiming();
            }
            
            if (triggerReady) {
                uintptr_t target = 0;
                if (Config::Aim::By == EAim::Distance) {
                    target = GetClosestTarget();
                } else if (Config::Aim::By == EAim::FOV) {
                    target = GetInsideFOVTarget();
                }
                
                if (target) {
                    auto pawn = reinterpret_cast<Pawn*>(target);
                    Vector3 targetPos;
                    
                    switch (Config::Aim::Target) {
                        case EAimTarget::Head: targetPos = pawn->get_HeadPosition(); break;
                        case EAimTarget::Chest: targetPos = pawn->get_HeadPosition(); targetPos.y -= 0.2f; break;
                        default: targetPos = pawn->get_HeadPosition(); targetPos.y -= 0.4f; break;
                    }
                    
                    auto mainCamera = Camera::get_main();
                    if (mainCamera) {
                        auto transform = reinterpret_cast<Component*>(mainCamera)->get_transform();
                        if (transform) {
                            dir = targetPos - transform->get_position();
                        }
                    }
                }
            }
        }
    }
    old_CreateBulletProjectile(thiz, startPos, dir, weaponImpact, itemID, flySmokeAssetID, enableVirtualStartPos, virtualStartPos);
}

} // namespace ESP
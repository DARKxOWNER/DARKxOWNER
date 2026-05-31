#pragma once

bool SlideTackle = false;
float SlideRange = 10.0f;
bool SlideShow = false;
bool SnowBshow = false;
bool isExecute = false;
bool RedWallhack = false;
bool showSpeedHackSlider = false;
float speedHackMultiplier = 2.0f;
bool isJumpAdjustmentEnabled = false;
float jumpHeightMultiplier = 1.5f;
float SnowBsize = 50.0f;
bool ClearDisplay = false;

enum TouchPhase { Began = 0, Moved = 1, Stationary = 2, Ended = 3, Canceled = 4 };
enum TouchType { Direct = 0, Indirect = 1, Stylus = 2 };

struct Touch {
    int m_FingerId;
    Vector2 m_Position;
    Vector2 m_RawPosition;
    Vector2 m_PositionDelta;
    float m_TimeDelta;
    int m_TapCount;
    TouchPhase m_Phase;
    TouchType m_Type;
    float m_Pressure;
    float m_maximumPossiblePressure;
    float m_Radius;
    float m_RadiusVariance;
    float m_AltitudeAngle;
    float m_AzimuthAngle;
};

enum LineTarget { Top = 0, Center = 1, Bottom = 2 };
enum ChamsTarget { none = 0, enableWallhack = 1, enableWallhackW = 2, enableWallhackG = 3, enableWallhackO = 4 };
enum EAim { Distance = 0, FOV = 1 };
enum EAimTarget { Head = 0, Chest = 1, Body = 2 };
enum EAimTrigger { None = 0, Shooting = 1, Scoping = 2 };
enum EBulletTrack { Bt = 0, Bt1 = 1, Bt2 = 2 };

namespace Config {
    namespace ImGui {
        bool g_Initialized = false;
        bool clearMousePos = true;
        uintptr_t thiz = 0;
    }

    namespace Aim {
        bool Aimbot360 = false;
        bool AimLock = false;
        bool AimAssist = false;
        bool showAimAssistSlider = false;
        float AimAssistSize = 50.0f;
        bool AimSilent = false;
        bool BulletTrack = false;
        EBulletTrack BulletTrackType = EBulletTrack::Bt;

        EAimTarget Target = EAimTarget::Chest;
        EAimTrigger Trigger = EAimTrigger::None;
        EAim By = EAim::Distance;
        float size = 100.0f;

        bool FpsLevel = false;
        float FpsLevel1 = 60.0f;
        bool showFPSLevelSlider = false;
        bool FpsLevelUltra = false;
        float FpsLevelUltra1 = 240.0f;
        bool showFPSLevelUltraSlider = false;
    }

    namespace ESP {
        bool Enabled = false;
        bool Alert = false;
        bool Count = false;
        bool Name = false;
        bool isPlayerLine = false;
        bool Box = false;
        bool Health = false;
        bool Distance = false;
        bool Skeleton = false;
        bool Vehicles = false;
        bool Crosshair = false;
        bool Aimline = false;

        float lineSize = 1.5f;

        LineTarget target = LineTarget::Bottom;
        ChamsTarget chamsType = ChamsTarget::none;
        
        namespace Colors {
            ImVec4 PlayerLine = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImVec4 PlayerBox = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImVec4 PlayerName = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImVec4 PlayerHealth = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            ImVec4 PlayerDistance = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            ImVec4 PlayerSkeleton = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

            ImVec4 BotLine = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
            ImVec4 BotBox = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
            ImVec4 BotName = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
            ImVec4 BotHealth = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
            ImVec4 BotDistance = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
            ImVec4 BotSkeleton = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);

            ImVec4 Others = ImVec4(0.88f, 0.0f, 0.0f, 0.7f);
        }

    }

    namespace Visual {
        bool Blacksky = false;
        bool Smoke = false;
        bool WallHack = false;
        bool Outline = false;
        bool Chams = false;
    }

    namespace Memory {
        bool Recoil = false;
        bool Spread = false;
        bool Reload = false;
        bool Shake = false;
        bool Scope = false;
        bool Switch = false;
        bool Flash = false;
        bool Hit = false;
        bool Auto = false;
        bool Fire = false;
        bool Cost = false;
        bool Overheat = false;
        bool Range = false;
        bool Kinetic = false;
        bool AutoSnipe = false;
    }

    namespace Movement {
        bool Speed = false;
        float SpeedMultiplier = 2.0f;
        bool Jump = false;
        float JumpHeight = 1.5f;
        bool Slide = false;
        float SlideSpeed = 1.5f;
        bool Snowboard = false;
        float SnowboardSpeed = 50.0f;
    }

    namespace Features {
        bool Parachute = false;
        bool Diving = false;
        bool Radar = false;
        bool Spectate = false;
        bool Revive = false;
        bool Medicine = false;
        bool JetJump = false;
        bool JetSlide = false;
        bool LongJump = false;
        bool HighFPS = false;
    }

    namespace Skins {
        bool Enabled = false;

        namespace Weapon {
            bool Memento = false;
            bool Glorious = false;
            bool Neon = false;
            bool Celestial = false;
            bool Bam = false;
            bool Demon = false;
            bool Lotus = false;
            bool Phoenix = false;
        }

        namespace Character {
            bool Qq = false;
            bool Spectre = false;
            bool Templar = false;
            bool Darkside = false;
            bool Song = false;
            bool Siege = false;
        }
    }
}

void (*orig_SetUltraFrameRateDeviceInfo)(void* instance, bool enableUltraFrameRate, int ultraFrameRate, int ultraFrameRateBR, int ultraFrameRateQualityLimit, bool customizedFrameRate);
void hooked_SetUltraFrameRateDeviceInfo(void* instance, bool enableUltraFrameRate, int ultraFrameRate, int ultraFrameRateBR, int ultraFrameRateQualityLimit, bool customizedFrameRate) {
    if (instance != NULL) {
        if (Config::Aim::FpsLevel) {
            enableUltraFrameRate = true;
            ultraFrameRate = (int)Config::Aim::FpsLevelUltra1;
            ultraFrameRateBR = 144;
            ultraFrameRateQualityLimit = 240;
            customizedFrameRate = true;
        }
    }
    orig_SetUltraFrameRateDeviceInfo(instance, enableUltraFrameRate, ultraFrameRate, ultraFrameRateBR, ultraFrameRateQualityLimit, customizedFrameRate);
}
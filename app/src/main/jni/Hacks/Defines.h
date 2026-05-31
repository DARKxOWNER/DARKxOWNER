#pragma once

#include <cstdint>

#define m_UNITYLIB OBFUSCATE("libunity.so")
#define m_ANOGSLIB OBFUSCATE("libanogs.so")

#define SLEEP_TIME 1000LL / 60LL

uintptr_t m_unity;
void UpdateAllOffsets();

uintptr_t Class_Input__get_touchCount;
uintptr_t Class_Input__GetTouch;
uintptr_t Class_Input__get_mousePosition;
uintptr_t Class_Screen__get_width;
uintptr_t Class_Screen__get_height;
uintptr_t Class_Screen__get_density;
uintptr_t Class_Camera__get_main;
uintptr_t Class_Camera__WorldToScreenPoint;
uintptr_t Class_Component__get_transform;
uintptr_t Class_Transform__get_position;
uintptr_t Class_Gameplay__get_MatchGame;
uintptr_t Class_Gameplay__get_LocalPawn;
uintptr_t Class_Pawn__get_IsFiring;
uintptr_t Class_Pawn__get_CurrentWeapon;
uintptr_t Class_Pawn__set_AimRotation;
uintptr_t Class_Pawn__IsAiming;
uintptr_t Class_Pawn__m_IsAlive;
uintptr_t Class_Pawn__m_Mesh;
uintptr_t Class_Pawn__m_HeadBone;
uintptr_t Class_Pawn__m_PlayerInfo;
uintptr_t Class_Pawn__m_IsBot;
uintptr_t Class_BaseGame__EnemyPawns;
uintptr_t Class_PlayerInfo__m_NickName;
uintptr_t Class_AttackableTarget__m_AttackableInfo;
uintptr_t Class_AttackableTarget__m_Health;
uintptr_t Class_AttackableTarget__m_MaxHealth;
uintptr_t Class_Weapon__GetWeaponId;

inline void UpdateAllOffsets() {
    Class_Input__get_touchCount = m_unity + 0xC199028;
    Class_Input__GetTouch = m_unity + 0xC199038;
    Class_Input__get_mousePosition = m_unity + 0xC1989E0;
    Class_Screen__get_width = m_unity + 0xC1916B8;
    Class_Screen__get_height = m_unity + 0xC19171C;
    Class_Screen__get_density = m_unity + 0xC1C5CB0;
    Class_Camera__get_main = m_unity + 0xC168924;
    Class_Camera__WorldToScreenPoint = m_unity + 0xC168570;
    Class_Component__get_transform = m_unity + 0xC16D66C;
    Class_Transform__get_position = m_unity + 0xC1BA2B0;
    Class_Gameplay__get_MatchGame = m_unity + 0x58FDCCC;
    Class_Gameplay__get_LocalPawn = m_unity + 0x58EDCE4;
    Class_Pawn__get_IsFiring = m_unity + 0x91AA578;
    Class_Pawn__get_CurrentWeapon = m_unity + 0x9131050;
    Class_Pawn__set_AimRotation = m_unity + 0x91B5BC8;
    Class_Pawn__IsAiming = m_unity + 0x919FCE0;
    Class_Pawn__m_IsAlive = 0x548;
    Class_Pawn__m_Mesh = 0x628;
    Class_Pawn__m_HeadBone = 0x308;
    Class_Pawn__m_PlayerInfo = 0x5C0;
    Class_Pawn__m_IsBot = 0x5B9;
    Class_BaseGame__EnemyPawns = 0x178;
    Class_PlayerInfo__m_NickName = 0x158;
    Class_AttackableTarget__m_AttackableInfo = 0x78;
    Class_AttackableTarget__m_Health = 0x34;
    Class_AttackableTarget__m_MaxHealth = 0x38;
    Class_Weapon__GetWeaponId = m_unity + 0xB541234;
}
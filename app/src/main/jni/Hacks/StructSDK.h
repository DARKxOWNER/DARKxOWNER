#pragma once

#include <vector>

class Camera;
class Transform;
class Component;
class Weapon;
class Pawn;

// Screen functions
int get_width();
int get_height();
float get_density();
Vector3 WorldToScreenPoint(Vector3 position);

class Camera {
public:
    static Camera* get_main() {
        return reinterpret_cast<Camera* (__fastcall*)()>(Class_Camera__get_main)();
    }
};

inline Vector3 WorldToScreenPoint(Vector3 position) {
    return reinterpret_cast<Vector3(__fastcall*)(Camera*, Vector3)>(Class_Camera__WorldToScreenPoint)(Camera::get_main(), position);
}

class Transform {
public:
    Vector3 get_position() {
        return reinterpret_cast<Vector3(__fastcall*)(Transform*)>(Class_Transform__get_position)(this);
    }
};

class Component {
public:
    Transform* get_transform() {
        return reinterpret_cast<Transform* (__fastcall*)(Component*)>(Class_Component__get_transform)(this);
    }
};

class Weapon {
public:
    int get_WeaponID() {
        using GetWeaponIDFunc = int (*)(Weapon*);
        static GetWeaponIDFunc _Weapon__get_WeaponID = (GetWeaponIDFunc)(Class_Weapon__GetWeaponId);
        return _Weapon__get_WeaponID ? _Weapon__get_WeaponID(this) : 0;
    }
};

class Pawn {
public:
    Weapon* get_CurrentWeapon() {
        using GetCurrentWeaponFunc = Weapon* (*)(Pawn*);
        static GetCurrentWeaponFunc _Pawn__get_CurrentWeapon = (GetCurrentWeaponFunc)(Class_Pawn__get_CurrentWeapon);
        return _Pawn__get_CurrentWeapon ? _Pawn__get_CurrentWeapon(this) : nullptr;
    }
    
    Transform* get_Mesh() {
        return *(Transform**)((uintptr_t)this + Class_Pawn__m_Mesh);
    }
    
    Transform* get_HeadBone() {
        return *(Transform**)((uintptr_t)this + Class_Pawn__m_HeadBone);
    }
    
    bool is_Alive() {
        return *(bool*)((uintptr_t)this + Class_Pawn__m_IsAlive);
    }
    
    bool is_Bot() {
        return *(bool*)((uintptr_t)this + Class_Pawn__m_IsBot);
    }
    
    bool is_Firing() {
        return reinterpret_cast<bool(*)(Pawn*)>(Class_Pawn__get_IsFiring)(this);
    }
    
    bool is_Aiming() {
        return reinterpret_cast<bool(*)(Pawn*)>(Class_Pawn__IsAiming)(this);
    }
    
    void set_AimRotation(Quaternion rotation) {
        reinterpret_cast<void(*)(Pawn*, Quaternion)>(Class_Pawn__set_AimRotation)(this, rotation);
    }
    
    uintptr_t get_PlayerInfo() {
        return *(uintptr_t*)((uintptr_t)this + Class_Pawn__m_PlayerInfo);
    }
    
    int get_Health() {
        auto playerInfo = get_PlayerInfo();
        if (!playerInfo) return 0;
        auto attackableInfo = *(uintptr_t*)(playerInfo + Class_AttackableTarget__m_AttackableInfo);
        if (!attackableInfo) return 0;
        return (int)*(float*)(attackableInfo + Class_AttackableTarget__m_Health);
    }
    
    int get_MaxHealth() {
        auto playerInfo = get_PlayerInfo();
        if (!playerInfo) return 100;
        auto attackableInfo = *(uintptr_t*)(playerInfo + Class_AttackableTarget__m_AttackableInfo);
        if (!attackableInfo) return 100;
        return (int)*(float*)(attackableInfo + Class_AttackableTarget__m_MaxHealth);
    }
    
    Vector3 get_Position() {
        auto mesh = get_Mesh();
        if (mesh) return mesh->get_position();
        return Vector3(0, 0, 0);
    }
    
    Vector3 get_HeadPosition() {
        auto headBone = get_HeadBone();
        if (headBone) return headBone->get_position();
        return Vector3(0, 0, 0);
    }
    
    const char* get_Name() {
        auto playerInfo = get_PlayerInfo();
        if (!playerInfo) return "UNKNOWN";
        auto nickName = *(String**)(playerInfo + Class_PlayerInfo__m_NickName);
        if (nickName && nickName->CString()) return nickName->CString();
        return "UNKNOWN";
    }
};

class GamePlay {
public:
    static Pawn* get_LocalPawn() {
        using GetLocalPawnFunc = Pawn*(*)();
        static GetLocalPawnFunc _GamePlay__get_LocalPawn = (GetLocalPawnFunc)(Class_Gameplay__get_LocalPawn);
        return _GamePlay__get_LocalPawn ? _GamePlay__get_LocalPawn() : nullptr;
    }
    
    static uintptr_t get_MatchGame() {
        return reinterpret_cast<uintptr_t(*)()>(Class_Gameplay__get_MatchGame)();
    }
    
    static List<uintptr_t>* get_EnemyPawns() {
        auto matchGame = get_MatchGame();
        if (matchGame) {
            return *(List<uintptr_t>**)(matchGame + Class_BaseGame__EnemyPawns);
        }
        return nullptr;
    }
    
    static std::vector<Pawn*> get_AliveEnemies() {
        std::vector<Pawn*> enemies;
        auto enemyList = get_EnemyPawns();
        if (enemyList && enemyList->getSize() > 0) {
            for (int i = 0; i < enemyList->getSize(); i++) {
                auto pawn = (Pawn*)(*enemyList)[i];
                if (pawn && pawn->is_Alive()) {
                    enemies.push_back(pawn);
                }
            }
        }
        return enemies;
    }
};

inline int get_width() {
    return reinterpret_cast<int(*)()>(Class_Screen__get_width)();
}

inline int get_height() {
    return reinterpret_cast<int(*)()>(Class_Screen__get_height)();
}

inline float get_density() {
    return reinterpret_cast<float(*)()>(Class_Screen__get_density)();
}
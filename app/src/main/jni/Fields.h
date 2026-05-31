struct RoleSkinFields {
    uint8_t ClothType; //0x18 
     uint8_t ColorID; //0x19 
     uint8_t GoGetPos; //0x1A 
     uint8_t HatSubMode; //0x1B 
     uint8_t IsCanBeUsedForAllRole; //0x1C 
     bool IsCloseScreenSaver; //0x1D 
     uint8_t SecondTab; //0x1E 
     bool ShowInBag; //0x1F 
     bool ShowRare; //0x20 
     int BRBagAssetID; //0x24 
     int BRClothAssetID; //0x28 
     int BRHatAssetID; //0x2C 
     int BRLeftArmAssetID; //0x30 
     int BRSuitAssetID; //0x34 
     int BRWingAssetID; //0x38 
     int BagAssetID; //0x3C 
     int CamoID; //0x40 
     int ClothAssetID; //0x44 
     int FxAssetIDHandGestureUI; //0x48 
     int FxAssetIDShow; //0x4C 
     int FxAssetID_1P; //0x50 
     int FxAssetID_3P; //0x54 
     int FxAssetID_UI; //0x58 
     int HatAssetID; //0x5C 
     uint ID; //0x60 
     int LeftArm1PAssetID; //0x64 
     int LeftArmAssetID; //0x68 
     int ShowBagAssetID; //0x6C 
     int ShowClothAssetID; //0x70 
     int ShowHatAssetID; //0x74 
     int SuitAssetID; //0x78 
     int WingAssetID; //0x7C 
};
RoleSkinFields* roleskinFields;

struct CharacterModelFields {
    bool Female; //0x10 
     uint AvatarAnimationType; //0x14 
     int BRBagModel; //0x18 
     int BRHeadModel; //0x1C 
     int BRLobby; //0x20 
     int BRModel; //0x24 
     int BindEffect1P; //0x28 
     int ChangeClipEffect1P; //0x2C 
     int DefaultModelID; //0x30 
     int Guarder1P; //0x34 
     int Guarder3P; //0x38 
     int GuarderBagModel; //0x3C 
     int GuarderHeadModel; //0x40 
     int GuarderLobby; //0x44 
     int HeadIgnoreGUPSkin; //0x48 
     int SpReceiveAnimType; //0x4C 
     int Traitor1P; //0x50 
     int Traitor3P; //0x54 
     float* BagOffset; //0x58 
     string* Desc; //0x60 
     float* VehicleOffset; //0x68 
     int* VestOtherModelID; //0x70 
     uintptr_t ItemID; //0x78 
     uintptr_t LitePackDefault; //0x80 
};
CharacterModelFields* characterfields;



struct RolePackFields {
     int mMatchRoomEasterEggType; // 0x10
     uint mMatchRoomEasterEggCharacterID; // 0x14
     String * mMatchRoomEasterEggActionName; // 0x18
     bool EntryDefaultAppearance; // 0x20
     bool UseVFWFeature; // 0x21
     int AirItem1; // 0x24
     int AnimationWhenSell; // 0x28
     int CabinIndivID; // 0x2C
     int DeadBoxID; // 0x30
     int DynamicRoomAssetID; // 0x34
     int EntryAnimID; // 0x38
     int EntryUIEffectAssetID; // 0x3C
     int GestureId; // 0x40
     int HandEffectUI; // 0x44
     int KillStreakSkinID; // 0x48
     int LoadingFrame; // 0x4C //done
     int LobbySceneType; // 0x50
     int MVPIndivID; // 0x54
     int MatchRoomSort; // 0x58
     int OpenSceneIndivID; // 0x5C
     int PendantShowID; // 0x60
     int PetID; // 0x64
     int PetSwitchEffectAssetID; // 0x68
     uint RolePackID; // 0x6C
     int SkillSkinID; // 0x70
     int SkillSkinUI; // 0x74
     int SpecialSceneFrameGroupAssetID; // 0x78
     int VTOLShowAssetID; // 0x7C
     String * AirPoseBR; // 0x80
     String * EntryAnim; // 0x88
     String * EntryIdleAnim; // 0x90
     void* EntryIdleItems; // 0x98
     void* EntryItems; // 0xA0
     void* EntryItemsAnim; // 0xA8
     void* EntryItemsIdleAnim; // 0xB0
     String * EntryUIAnimationStateName; // 0xB8
};
RolePackFields* packfields;


struct RoleConfFields {
     int roleLeftArmID; //0x14 
     int roleFinalSuitID; //0x18 
     int roleBasicHologramID; //0x1C 
     uint8_t ArchiveSwitch; //0x20 
     uint8_t ArmorType; //0x21 
     bool BlockBack; //0x22 
     uint8_t ColorID; //0x23 
     uint8_t ColorSubID; //0x24 
     uint8_t DefaultPerk_1; //0x25 
     uint8_t DefaultPerk_2; //0x26 
     uint8_t DefaultPerk_3; //0x27 
     bool DefaultShow; //0x28 
     uint8_t DefaultSkill_1; //0x29 
     bool ExternalUnVisible; //0x2A 
     bool IsVFW; //0x2B 
     uint8_t Loadout_Magnifier_Flag; //0x2C 
     uint8_t LockedShowOrder; //0x2D 
     uint8_t RoleLvGroupID; //0x2E 
     bool ShowRare; //0x2F 
     int ID; //0x30 
     int JetPackInfoID; //0x34 
     float RTPCMaxValue; //0x38 
     int RolePackID; //0x3C 
     void *AudioSwitch; //0x40 
    void *BRBankNameEX; //0x48 
    void *BasicAudioBank; //0x50 
    void *BasicAudioBank2; //0x58 
    void *BasicAudioBank3; //0x60 
    void *DefaultAudioBank; //0x68 
    void *DefaultAudioBank2; //0x70 
    void *DefaultAudioEventEx; //0x78 
    void *FoleyAudioBank; //0x80 
    void *FoleyAudioNameEx; //0x88 
    void *LOCID_Desc; //0x90 
    void *LOCID_Name; //0x98 
    void *LitePackageKeyName; //0xA0 
    void *MPBankNameEx; //0xA8 
    void *PVEBankNameEX; //0xB0 
    void *RTPCName; //0xB8 
    void *RoleSkinBox; //0xC0 
    void *SpecialAudioBank; //0xC8 
    void *SpecialAudioBank2; //0xD0 
    void *SpecialAudioEventEx; //0xD8 
    void *SpecialTips; //0xE0 
    void *WinnerCircle; //0xE8 
};
RoleConfFields* roleFields;


struct ItemResourceFields {
     int FxAssetID; //0x10 
     int ID; //0x14 
     int InventoryModelID; //0x18 
     int ModelAssetIDRaw; //0x1C 
     void *UIMiniSpriteName; //0x20 
     void *UISmallSpriteName; //0x28 
     void *UISpriteName; //0x30 
     void *UISquareSpriteName; //0x38 
     uintptr_t AvatarModelID; //0x40 
};
ItemResourceFields* itemFields;

struct WeaponConfFields {
     uint8_t Accuracy; //0x20 
     bool CanEquipOptic; //0x21 
     uint8_t ColorID; //0x22 
     uint8_t ColorSubID; //0x23 
     uint8_t Control; //0x24 
     uint8_t Damage; //0x25 
     bool ExternalUnVisible; //0x26 
     uint8_t FireRate; //0x27 
     uint8_t LOCID_HowToGet; //0x28 
     uint8_t MaxAttach; //0x29 
     uint8_t MaxSkill; //0x2A 
     uint8_t Mobility; //0x2B 
     uint8_t Range; //0x2C 
     bool ShowRare; //0x2D 
     uint8_t SuperShowGroup; //0x2E 
     uint8_t WeaponBluePrintType; //0x2F 
     uint8_t WinnerCircleAnimID; //0x30 
     uint BaseWeaponId; //0x34 
     int DefWeaponSkinID; //0x38 
     int DefaultKillBrocast; //0x3C 
     uint ID; //0x40 
     int WinnerCircleWeaponMountType; //0x44 
     void *EnableCamoBeta; //0x48 
     void *LOCID_Desc; //0x50 
     void *LOCID_Name; //0x58 
     void *ModelRotation; //0x60 
     void *UIProperty; //0x68 
     void *UIPropertyUnlockLevel; //0x70 

};
WeaponConfFields* weaponconfFields;




struct Item2InventoryFields {
    int ItemID; //0x20 
    int WeaponAssetGroupID; //0x24 
    int WeaponIconID; //0x28 
};
Item2InventoryFields* item2Fields;


struct WeaponAssetGroupFields {
     int FireEffectGroupID; //0x40 
     int Id; //0x44 
};
WeaponAssetGroupFields* weaponAssetFields;


struct WeaponFireEffectFields {
     int AssetIdFor1P; //0x10 
     int AssetIdFor3P; //0x14 
     int AssetIdForBulletSmoke1P; //0x18 
     int AssetIdForBulletSmoke3P; //0x1C 
     int AssetIdForBulletTrack1P; //0x20 
     int AssetIdForBulletTrack3P; //0x24 
     int AssetIdForCharging1P; //0x28 
     int AssetIdForCrossHair; //0x2C 
     int AssetIdForDecalEffect; //0x30 
     int AssetIdForExtra1P; //0x34 
     int AssetIdForExtraBulletSmoke1P; //0x38 
     int AssetIdForExtraBulletSmoke3P; //0x3C 
     int AssetIdForHip1P; //0x40 
     int AssetIdForHip3P; //0x44 
     int AssetIdForHitEnemy; //0x48 
     int AssetIdForInspectionScreenEffect; //0x4C 
     int AssetIdForSecondBulletSmoke1P; //0x50 
     int AssetIdForSecondBulletSmoke3P; //0x54 
     int AssetIdForSecondHip1P; //0x58 
     int AssetIdForSecondHip3P; //0x5C 
     int AssetIdForShellDrop1P; //0x60 
     int AssetIdForShellDrop3P; //0x64 
     int AttachAssetIdFor1P; //0x68 
     int AttachAssetIdFor3P; //0x6C 
     int AttachAssetIdForHip1P; //0x70 
     int AttachAssetIdForHip3P; //0x74 
     int DefaultKillEffectLevelUI; //0x78 
     float HitEffectDelayDestroyTime; //0x7C 
     int HitEffectType; //0x80 
     int Id; //0x84 
};
WeaponFireEffectFields* weaponfireFields;



struct WeaponConfExtraFields {
     uint8_t DefaultMythicArmor; //0x10 
     uint8_t DefaultMythicSig; //0x11 
     uint8_t GoGetPos; //0x12 
     uint8_t SecondTab; //0x13 
     uint8_t ThirdTab; //0x14 
     int DefaultDeadReplayEffectId; //0x18 
     int DefaultKillEffectId; //0x1C 
     int FireEffectGroupID; //0x20 
     uint ID; //0x24 
};
WeaponConfExtraFields* weaponextraFields;


struct MythicArmorFields {
     int AssetID; //0x14 
     int ColorID; //0x18 
     int ColorSubID; //0x1C 
     int GoGetPos; //0x20 
     uint ID; //0x24 
     int MythicArmorLevel; //0x28 
     int SecondTab; //0x2C 
     int ThirdTab; //0x30 
};
MythicArmorFields* mythicarmorFields;



struct MythicSightFields {
     bool ShowInBag; //0x10 
     char pad_11[3];
     int AssetID; //0x14 
     int ColorID; //0x18 
     int ColorSubID; //0x1C 
     uint ID; //0x20 
     int RelateMythicSight; //0x24 
     void* LOCID_Desc; //0x28 
     void* LOCID_Name; //0x30 
     Array<int> **WeaponCanEquip; //0x38
};
MythicSightFields* mythicsightFields;



struct KillEffectItemFields {
     Array<int> **RealAssetIDs; //0x10 
     bool IsAutoDemolition; //0x18 
     bool ShowInBag; //0x19 
     int ColorID; //0x1C 
     int ColorSubID; //0x20 
     uint CurrencyID; //0x24 
     int GoGetPos; //0x28 
     uint ID; //0x2C 
     int SecondTab; //0x30 
     int SellPrice; //0x34 
     int SellProtection; //0x38 
     int ThirdTab; //0x3C 
     Array<int> **AssetID; //0x40 
     Array<int> **AssetID_CN; //0x48 
     Array<int> **AssetID_GA; //0x50 
     Array<int> **AssetID_KR; //0x58 
     Array<int> **AssetID_VNG; //0x60 
     string* GoGetUrl; //0x68 
     Array<int> **KillCount; //0x70 
     void* LOCID_Desc; //0x78 
     void* LOCID_HowToGet; //0x80 
     void* LOCID_Name; //0x88 
     Array<int> **WeaponCanEquip; //0x90 
};
KillEffectItemFields* killeffectFields;



struct BRDeadboxSkinFields {
    int ColorID; //0x10 
    int DeadBoxEffectAsset; //0x14 
    int Flag; //0x18 
    int FlagAsset; //0x1C 
    uint ID; //0x20 
    int ModelAsset3P; //0x24 
    int ModelAssetUI; //0x28 
};
BRDeadboxSkinFields* deadboxFields;


struct BRDropPlaneSkinFields {
     int ColorID; //0x10 
     uint ID; //0x14 
     int ModelAsset1P; //0x18 
     int ModelAsset3P; //0x1C 
     int ModelAssetCutScene; //0x20 
     int ModelAssetUI; //0x24 
     int Priority; //0x28 
};
BRDropPlaneSkinFields* dropplaneFields;










static char searchQuery[256] = "";   
static char searchQuery2[256] = "";   

bool loadskinhack = false;
bool loadCharacter = false;
bool isLoad = false;

std::string lastKnownName = "";
int emptyNameCount = 0; 


struct itemInfo {
std::string itemName;
std::array<int, 4> Item2Inventory;
std::array<int, 4> WeaponConf;
std::array<int, 5> WeaponExtra;
std::array<int, 3> WeaponAsset;
std::array<int, 3> ItemResInt;
std::array<void *, 4> ItemRes;
};
std::vector<itemInfo> itemData;

std::unordered_map<std::string, int> nameCountMap;
std::unordered_map<std::string, int> nameCountChar;



struct charInfo {
    std::string charName;
    std::array<int, 12> charModel;
    std::array<int, 3> charRes;
    std::array<void *, 4> charRes2;
    std::array<int, 8> charRole;
    std::array<int, 2> charPack;
};

std::vector<charInfo> charData;



struct watcher {
    std::string watchname;
    int watchvalue;
};
std::vector<watcher> watch;

struct deadbox {
    std::string deadname;
    std::array<int, 6> dead;
};
std::vector<deadbox> deadboxF;


struct planeID {
    std::string planename;
    std::array<int, 6> plane;
};
std::vector<planeID> dropplane;




uintptr_t location = 0;

uintptr_t getRealOffset(uintptr_t offset) {
    while (location <= 0) {
        location = Tools::GetBaseAddress("libunity.so");
        if (location <= 0) {
            usleep(1000); 
        }
    }
    return location + offset;
}




std::string GetNameString(uintptr_t off, void *getadd) {
    auto getC = (String *(*)(void *)) (getRealOffset(off));
    if (Tools::IsPtrValid((void *) getC)) {
    auto getV = getC(getadd);
    std::string getB = getV->CString();
    return getB;
    }
    return "";
}


std::string ToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void KeyboardWindow(char s[4096], bool h) {
    static bool useLowercase = true;
    
    if (h) {
      /*      
    ImGui::SetNextWindowSize(ImVec2(500, 480)); 
    ImGui::Begin("Custom Keyboard", &h);
*/

    ImVec2 contentSize = ImGui::GetContentRegionAvail(); 
    float buttonWidth = contentSize.x / 10.0f; 
    float buttonHeight = 50.0f; 
    float padding = 5.0f; 

    std::vector<std::vector<char>> keyboardLayoutLowercase = {
        {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
        {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
        {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'},
        {'z', 'x', 'c', 'v', 'b', 'n', 'm'},
    };

    std::vector<std::vector<char>> keyboardLayoutUppercase = {
        {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
        {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
        {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'},
        {'Z', 'X', 'C', 'V', 'B', 'N', 'M'},
    };

    const std::vector<std::vector<char>>& keyboardLayout = (useLowercase ? keyboardLayoutUppercase : keyboardLayoutLowercase);

    for (const auto& row : keyboardLayout) {
        for (const char& c : row) {
            std::string label(1, c);
            if (ImGui::Button(label.c_str(), ImVec2(buttonWidth - padding, buttonHeight))) {
                if (strlen(s) < 4095) {
                    strncat(s, &c, 1); 
                }
            }
            ImGui::SameLine(0, padding);
        }
        ImGui::NewLine();
    }
    
    if (ImGui::Button("[M]", ImVec2(buttonWidth - padding + 6, buttonHeight))) {
        if (strlen(s) < 4095) {
            strncat(s, "[M]", 3); 
        }
    }

    ImGui::SameLine();
    
    if (ImGui::Button("[L]", ImVec2(buttonWidth - padding + 6, buttonHeight))) {
        if (strlen(s) < 4095) {
            strncat(s, "[L]", 3); 
        }
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("[E]", ImVec2(buttonWidth - padding + 6, buttonHeight))) {
        if (strlen(s) < 4095) {
            strncat(s, "[E]", 3); 
        }
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("[C]", ImVec2(buttonWidth - padding + 6, buttonHeight))) {
        if (strlen(s) < 4095) {
            strncat(s, "[C]", 3); 
        }
    }
    
    
    ImGui::NewLine();
    
    if (ImGui::Button("Space", ImVec2(buttonWidth * 3 - padding, buttonHeight))) {
        if (strlen(s) < 4095) {
            strncat(s, " ", 1); 
        }
    }

    ImGui::SameLine();
    
    if (ImGui::Button("<<", ImVec2(buttonWidth * 2 - padding, buttonHeight))) {
        size_t len = strlen(s);
        if (len > 0) {
            s[len - 1] = '\0'; 
        }
    }

    ImGui::SameLine();
    
    if (ImGui::Button("Shift", ImVec2(buttonWidth * 2 - padding, buttonHeight))) {
        useLowercase = !useLowercase;
    }
    
   
    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(buttonWidth * 2 - padding, buttonHeight))) {
        s[0] = '\0'; 
    }

   // ImGui::End();
    
    
    }
}





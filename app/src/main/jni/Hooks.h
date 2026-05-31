std::unordered_map<std::string, bool> sBool;
std::unordered_map<int, int> activeKillEffects;

int (*orig_GetCurrentWeaponKillEffect)(Weapon);
int _GetCurrentWeaponKillEffect(Weapon weapon) {
    Pawn* localPawn = GamePlay::get_LocalPawn();
    if (!localPawn) return 0;

    Weapon* currentWeapon = localPawn->get_CurrentWeapon();
    if (!currentWeapon) return 0;

    int currentID = currentWeapon->get_WeaponID();

    auto it = activeKillEffects.find(currentID);
    if (it != activeKillEffects.end()) {
        return it->second;
    }

    return 0;
}

std::vector<void *> itemInventoryInstance;
std::vector<void *> weaponExtraInstance;
std::vector<void *> weaponFireEffectInstance;
std::vector<void *> weaponConfInstance;
std::vector<void *> weaponAssetGroupInstance;
std::vector<void *> mythicArmorInstance;
std::vector<void *> mythicSightInstance;
std::vector<void *> killEffectItemInstance;
std::vector<void *> weaponSkinConfigInstance;
std::vector<void *> itemResourceConfigInstance;
std::vector<void *> CharacterModelConfigInstance;
std::vector<void *> RoleConfConfigInstance;
std::vector<void *> RoleSkinConfigInstance;
std::vector<void *> RolePackConfConfigInstance;
std::vector<void *> BRDeadboxSkinConfigInstance;
std::vector<void *> BRDropPlaneSkinConfigInstance;

uintptr_t Item2InventoryAddress = 0x522DB58;
uintptr_t WeaponConfAddress = 0x529C8D0;
uintptr_t WeaponConfExtraAddress = 0x529DEA4;
uintptr_t WeaponFireEffectAddress = 0x529F6E8;
uintptr_t CharacterModelAddress = 0x51E3438;
uintptr_t BRDeadboxSkinAddress = 0x51CEDC8;
uintptr_t BRDropPlaneSkinAddress = 0x51D14F0;
uintptr_t WeaponAssetGroupAddress = 0x5298418;
uintptr_t MythicArmorConfigAddress = 0x52580F8;
uintptr_t MythicSightConfigAddress = 0x52585C8;
uintptr_t KillEffectItemConfConfigAddress = 0x52433EC;
uintptr_t WeaponSkinAddress = 0x52A87F0;
uintptr_t ItemResourceAddress = 0x5241058;
uintptr_t RoleConfAddress = 0x526A650;
uintptr_t RoleSkinAddress = 0x527054C;
uintptr_t RolePackConfAddress = 0x526EE70;
uintptr_t WeaponConfName = 0x52308B0;
uintptr_t GetDropPlaneName = 0x51D1D30;
uintptr_t GetNameRoleSkin = 0x5270674;
uintptr_t GetRoleConfName = 0x526BF58;
uintptr_t GetDeadBoxIDAddress = 0x526B17C;
uintptr_t GetCurrentWeaponAddress = Class_Pawn__get_CurrentWeapon;

#define ReadInt(base, offset) (*(int*)((uintptr_t)(base) + (offset)))
#define ReadBool(base, offset) (*(bool*)((uintptr_t)(base) + (offset)))
#define ReadFloat(base, offset) (*(float*)((uintptr_t)(base) + (offset)))
#define ReadByte(base, offset) (*(uint8_t*)((uintptr_t)(base) + (offset)))
#define READ_PTR(type, base, offset) (*(type**)((uintptr_t)(base) + (offset)))

std::unordered_map<std::string, bool> getplane;
std::unordered_map<std::string, bool> getguns;

void *GameUpdate(void *) {
    while (true) {
        // Character model loading and processing
        if (!loadCharacter && CharacterModelConfigInstance.size() > 0 && itemResourceConfigInstance.size() > 0
            && RoleConfConfigInstance.size() > 0 && RoleSkinConfigInstance.size() > 0
            && RolePackConfConfigInstance.size() > 0 && BRDeadboxSkinConfigInstance.size() > 0) {

            // Process RoleSkin configurations
            for (auto &a : RoleSkinConfigInstance) {
                if (!a) continue;

                auto *fx = (RoleSkinFields *) ((uintptr_t) a + 0x18); // public byte ClothType;
                if ((fx->FxAssetID_1P ^ 0) != 0) {
                    std::string n = GetNameString(GetNameRoleSkin, (void *) a);
                    watch.push_back({n, fx->FxAssetID_1P});
                }
            }

            std::string _g = "";
            // Process BRDeadboxSkin configurations
            for (auto &z : BRDeadboxSkinConfigInstance) {
                if (!z) continue;

                auto *y = (BRDeadboxSkinFields *) ((uintptr_t) z + 0x10); // public int ColorID;
                bool __b = false;

                for (auto &q : RoleConfConfigInstance) {
                    if (!q) continue;

                    auto *f = (RoleConfFields *) ((uintptr_t) q + 0x14); // public int DeadBoxEffectAsset;
                    std::string s = GetNameString(GetRoleConfName, q);

                    if ((y->ID & 0xFFFFFFF) != 0) {
                        auto fx = (int (*)(void *))(getRealOffset(GetDeadBoxIDAddress));
                        int k = fx(q);

                        if (y->ID == k && !__b) {
                            auto dump = [&](const std::string &nm) {
                                deadboxF.push_back({nm, {
                                    y->ColorID,
                                    y->DeadBoxEffectAsset,
                                    y->Flag,
                                    y->FlagAsset,
                                    y->ModelAsset3P,
                                    y->ModelAssetUI
                                }});
                            };

                            if (y->ColorID == 5) {
                                _g = s;
                                dump(s);
                                __b = true;
                            } else {
                                dump(s);
                            }
                        }

                        if (__b) break;

                        if (k + 1 == y->ID) {
                            if (y->ColorID == 5 && y->FlagAsset != 0 && !_g.empty()) {
                                std::string v = _g + " (Variant)";
                                deadboxF.push_back({v, {
                                    y->ColorID,
                                    y->DeadBoxEffectAsset,
                                    y->Flag,
                                    y->FlagAsset,
                                    y->ModelAsset3P,
                                    y->ModelAssetUI
                                }});
                            }
                        }
                    }
                }
            }

            // Process CharacterModel configurations
            for (auto X0 : CharacterModelConfigInstance) {
                if (!X0) continue;
                auto *X1 = (CharacterModelFields *)((uintptr_t)X0 + 0x10); // public bool Female:
                auto A = X1->ItemID;

                auto B = X1->BRBagModel;
                auto C = X1->BRHeadModel;
                auto D = X1->BRLobby;
                auto E = X1->BRModel;
                auto F = X1->BindEffect1P;
                auto G = X1->ChangeClipEffect1P;
                auto H = X1->DefaultModelID;
                auto I = X1->Guarder1P;
                auto J = X1->Guarder3P;
                auto K = X1->GuarderBagModel;
                auto L = X1->GuarderHeadModel;
                auto M = X1->GuarderLobby;
                auto N = X1->Traitor1P;

                for (auto Y0 : itemResourceConfigInstance) {
                    if (!Y0) continue;
                    auto *Y1 = (ItemResourceFields *)((uintptr_t)Y0 + 0x10); // public int FxAssetID;

                    auto A_ = Y1->AvatarModelID;

                    if (A != A_) continue;

                    auto P = Y1->FxAssetID;
                    auto Q = Y1->InventoryModelID;
                    auto R = Y1->ModelAssetIDRaw;

                    auto S = Y1->UIMiniSpriteName;
                    auto T = Y1->UISmallSpriteName;
                    auto U = Y1->UISpriteName;
                    auto V = Y1->UISquareSpriteName;

                    auto W = Y1->ID;

                    for (auto Z0 : RoleConfConfigInstance) {
                        if (!Z0) continue;
                        auto *Z1 = (RoleConfFields *)((uintptr_t)Z0 + 0x14); // private uint roleLeftArmID;

                        if (Z1->ID != W) continue;

                        auto a = Z1->roleLeftArmID;
                        auto b = Z1->roleFinalSuitID;
                        auto c = Z1->roleBasicHologramID;
                        auto d = Z1->ColorID;
                        auto e = Z1->ColorSubID;
                        auto f = Z1->ShowRare;
                        auto g = Z1->RoleLvGroupID;
                        auto h = Z1->RolePackID;

                        int i = 0, j = 0;
                        for (auto RP : RolePackConfConfigInstance) {
                            if (!RP) continue;
                            auto *rpF = (RolePackFields *)((uintptr_t)RP + 0x10); // private uint mMatchRoomPoseID;
                            if (rpF->RolePackID != h) continue;
                            if (rpF->LoadingFrame && rpF->LobbySceneType == 0) {
                                i = rpF->RolePackID;
                                j = rpF->LoadingFrame;
                            }
                        }

                        std::string Zz, n0 = GetNameString(GetRoleConfName, Z0);
                        if (n0.empty()) {
                            if (!lastKnownName.empty()) {
                                char c_ = 'A' + emptyNameCount++;
                                n0 = lastKnownName + " " + c_ + "+";
                            } else n0 = "UnknownName";
                        } else {
                            lastKnownName = n0;
                            emptyNameCount = 0;
                        }

                        std::string key = n0 + "_" + std::to_string(A);
                        if (nameCountChar.count(key)) {
                            nameCountChar[key]++;
                            n0 += " +" + std::to_string(nameCountChar[key]);
                        } else nameCountChar[key] = 0;

                        // Color-based prefix assignment
                        if (d == 5) Zz = "[M] " + n0;
                        else if (d == 4) Zz = "[L] " + n0;
                        else if (d == 3) Zz = "[E] " + n0;
                        else Zz = "[C] " + n0;

                        if (Zz.find("A+") != std::string::npos) h--;

                        charData.push_back({
                            Zz,
                            {B, C, D, E, F, G, H, I, J, K, L, M},
                            {P, Q, R},
                            {S, T, U, V},
                            {a, b, c, d, e, f, g, h},
                            {i, j}
                        });
                    }
                    break;
                }
                loadCharacter = true;
            }
        }

        // Weapon skin and configuration loading
        if (!loadskinhack 
            && weaponConfInstance.size() > 0 
            && itemInventoryInstance.size() > 0 
            && weaponAssetGroupInstance.size() > 0 
            && weaponFireEffectInstance.size() > 0 
            && weaponExtraInstance.size() > 0 
            && killEffectItemInstance.size() > 0
            && mythicArmorInstance.size() > 0
            && mythicSightInstance.size() > 0
            && itemResourceConfigInstance.size() > 0) {

            for (auto conf : weaponConfInstance) {
                if (!conf) continue;

                auto baseID = *(int *) ((uintptr_t) conf + 0x34); // weaponconfFields->BaseWeaponId; //public uint BaseWeaponId;
                auto confID = *(int *) ((uintptr_t) conf + 0x40); // public uint ID;

                for (auto item : itemInventoryInstance) {
                    if (!item) continue;

                    auto itemIDbase = *(int *) ((uintptr_t) item + 0x20); // classname: item2inventory //public int ItemID;
                    int itemBaseModified;

                    int itemBase;
                    if (baseID == itemIDbase) {
                        itemBase = *(int *) ((uintptr_t) item + 0x20); // public int ItemID;
                        itemBaseModified = itemBase + 200;
                    }

                    if (confID == itemIDbase) {
                        auto confcolorID = *(uint8_t *) ((uintptr_t) conf + 0x22); // classname: class WeaponConfConfig //public byte ColorID;
                        auto itemIDskin2 = *(int *) ((uintptr_t) item + 0x24); // public int WeaponAssetGroupID;
                        auto itemIDskin3 = *(int *) ((uintptr_t) item + 0x28); // public int WeaponIconID;

                        std::string AName;
                        std::string getName = GetNameString(WeaponConfName, conf);

                        // Color-based naming
                        if (confcolorID == 5) {
                            AName += "[M] " + getName;
                        } else if (confcolorID == 4) {
                            AName += "[L] " + getName;
                        } else if (confcolorID == 3) {
                            AName += "[E] " + getName;
                        } else {
                            AName += "[C] " + getName;
                        }

                        if (nameCountMap.find(AName) != nameCountMap.end()) {
                            nameCountMap[AName]++;
                            AName += " +" + std::to_string(nameCountMap[AName]);
                        } else {
                            nameCountMap[AName] = 0;
                        }

                        if (!AName.empty()) {
                            // Process weapon fire effects and assets
                            int fireIds = 0, fireIds2 = 0, assetIds = 0;
                            int originalFireID = 0;

                            for (auto asset : weaponAssetGroupInstance) {
                                if (!asset) continue;

                                auto assetID = *(int *) ((uintptr_t) asset + 0x44); // public int Id;

                                if (itemIDskin2 == assetID) {
                                    auto fireEffectID = *(int *) ((uintptr_t) asset + 0x40); // public int FireEffectGroupID;

                                    for (auto fireConf : weaponFireEffectInstance) {
                                        if (!fireConf) continue;

                                        int fireID = *(int *) ((uintptr_t) fireConf + 0x84); // classname: WeaponFireEffectConfigConfig //public int Id;
                                        int assetIdBulletSmoke = *(int *) ((uintptr_t) fireConf + 0x18); // public int AssetIdForBulletSmoke1P;

                                        if (AName.find("[M]") != std::string::npos) {
                                            if (fireEffectID == fireID) {
                                                if (assetIdBulletSmoke != 0) {
                                                    fireIds = fireID;
                                                    originalFireID = fireID;
                                                    break;
                                                }

                                                int nextFireID = fireID + 1;
                                                bool found = false;

                                                for (int i = 0; i < 10; i++) {
                                                    for (auto nextFireConf : weaponFireEffectInstance) {
                                                        if (!nextFireConf) continue;

                                                        int nextFireIDCheck = *(int *) ((uintptr_t) nextFireConf + 0x84);
                                                        int nextAssetIdBulletSmoke = *(int *) ((uintptr_t) nextFireConf + 0x18);

                                                        if (nextFireIDCheck == nextFireID && nextAssetIdBulletSmoke != 0) {
                                                            fireIds = nextFireIDCheck;
                                                            found = true;
                                                            break;
                                                        }
                                                    }
                                                    if (found) break;
                                                    nextFireID++;
                                                }

                                                if (fireIds == 0) fireIds = fireID;

                                                fireIds2 = fireID;
                                                assetIds = assetID;
                                            }
                                        }
                                    }
                                }
                            }

                            // ============ WeaponConfConfig
                            int confbaseskin = 0, confSkinID = 0, confBrocastID = 0, confColorID = 0;
                            if (confID == itemIDbase) {
                                confbaseskin = ReadInt(conf, 0x34); // public uint BaseWeaponId;
                                confColorID = ReadByte(conf, 0x22); // public byte ColorID;
                                confSkinID = ReadInt(conf, 0x38); // public int DefWeaponSkinID;
                                confBrocastID = ReadInt(conf, 0x3C); // public int DefaultKillBrocast;
                            }

                            int mythicArmor = 0, deadReplay = 0, killEffect = 0, extraOrig = 0;
                            for (auto extra : weaponExtraInstance) {
                                if (!extra) continue;
                                weaponextraFields = (WeaponConfExtraFields*) ((uintptr_t) extra + 0x10); // public byte DefaultMythicArmor;

                                auto extraID = weaponextraFields->ID;

                                if (baseID == extraID) {
                                    extraOrig = weaponextraFields->ID;
                                }

                                if (confID == extraID) {
                                    deadReplay = weaponextraFields->DefaultDeadReplayEffectId;
                                    killEffect = weaponextraFields->DefaultKillEffectId;
                                }
                            }

                            // Process mythic armor
                            for (auto armor : mythicArmorInstance) {
                                if (!armor) continue;
                                mythicarmorFields = (MythicArmorFields*) ((uintptr_t) armor + 0x14); // public int AssetID; //0x14

                                auto armorassetID = mythicarmorFields->AssetID;
                                auto secondTab = mythicarmorFields->SecondTab;
                                auto thirdTab = mythicarmorFields->ThirdTab;

                                if (itemIDskin3 == secondTab) {
                                    if (thirdTab == 5) {
                                        if (AName.find("[M]") != std::string::npos) {
                                            mythicArmor = armorassetID;
                                        }
                                    }
                                }
                            }

                            // Process mythic sight
                            int sightMythic = 0;
                            for (auto sight : mythicSightInstance) {
                                if (!sight) continue;
                                mythicsightFields = (MythicSightFields*) ((uintptr_t) sight + 0x10); // public bool ShowInBag;

                                auto sightID = *(int *) ((uintptr_t) sight + 0x14); // public int AssetID;
                                auto equipArray = *(Array<int>**) ((uintptr_t) sight + 0x38); // public int[] WeaponCanEquip;

                                if (equipArray && equipArray->getLength() > 0) {
                                    if (itemIDskin3 == equipArray->m_Items[equipArray->getLength()]) {
                                        if (AName.find("[M]") != std::string::npos) {
                                            sightMythic = sightID;
                                        }
                                    }
                                }
                            }

                            // Process kill effects
                            for (auto wKilling : killEffectItemInstance) {
                                if (!wKilling) continue;
                                killeffectFields = (KillEffectItemFields*) ((uintptr_t) wKilling + 0x10); // public int[] RealAssetIDs;

                                auto equipArray = *(Array<int> **) ((uintptr_t) wKilling + 0x90); // *killeffectFields->WeaponCanEquip; //public int[] WeaponCanEquip;

                                int canEquip = 0;
                                if (equipArray && equipArray->getLength() > 0) {
                                    canEquip = equipArray->m_Items[equipArray->getLength()];
                                }

                                auto realAssetIDs_1 = *(Array<int> **) ((uintptr_t) wKilling + 0x10); // *killeffectFields->RealAssetIDs;  // public int[] RealAssetIDs;
                                int lastAssetID = 0;
                                if (realAssetIDs_1 && realAssetIDs_1->getLength() > 0) {
                                    lastAssetID = realAssetIDs_1->m_Items[realAssetIDs_1->getLength()];
                                }

                                if (itemIDskin3 == canEquip) {
                                    if (AName.find("[M]") != std::string::npos) {
                                        killEffect = lastAssetID;
                                    }
                                }
                            }

                            // Process item resources
                            void *spr1 = NULL;
                            void *spr2 = NULL;
                            void *spr3 = NULL;
                            void *spr4 = NULL;
                            int xItem1 = 0, xItem2 = 0, xItem3 = 0;

                            for (auto itemResource : itemResourceConfigInstance) {
                                if (!itemResource) continue;
                                itemFields = (ItemResourceFields*) ((uintptr_t) itemResource + 0x10); // public int FxAssetID;

                                auto checkItem = itemFields->ID;
                                auto sItem1 = itemFields->FxAssetID;
                                auto sItem2 = itemFields->InventoryModelID;
                                auto sItem3 = itemFields->ModelAssetIDRaw;
                                auto sprite1 = itemFields->UISmallSpriteName;
                                auto sprite2 = itemFields->UIMiniSpriteName;
                                auto sprite3 = itemFields->UISpriteName;
                                auto sprite4 = itemFields->UISquareSpriteName;

                                if (checkItem == confID) {
                                    xItem1 = sItem1;
                                    xItem2 = sItem2;
                                    xItem3 = sItem3;
                                    spr1 = sprite1;
                                    spr2 = sprite2;
                                    spr3 = sprite3;
                                    spr4 = sprite4;
                                }
                            }

                            // Push collected weapon data
                            itemData.push_back({
                                AName,
                                {itemBase, itemIDskin2, itemIDskin3, itemBaseModified}, // Item2Inventory
                                {confbaseskin, confColorID, confID, confBrocastID}, // WeaponConf
                                {extraOrig, mythicArmor, sightMythic, deadReplay, killEffect}, // WeaponExtra
                                {assetIds, fireIds, fireIds2}, // WeaponAsset
                                {xItem1, xItem2, xItem3},
                                {spr1, spr2, spr3, spr4}
                            });
                        }
                    }
                }
            }
            loadskinhack = true;
        }

        // Process drop plane skins
        if (BRDropPlaneSkinConfigInstance.size() > 0) {
            for (auto planedrop : BRDropPlaneSkinConfigInstance) {
                if (!planedrop) continue;

                dropplaneFields = (BRDropPlaneSkinFields*)((uintptr_t)planedrop + 0x10); // public int ColorID
                std::string plane = GetNameString(GetDropPlaneName, planedrop);

                if (dropplaneFields->ModelAsset1P != 0 && !getplane[plane]) {
                    getplane[plane] = true;

                    dropplane.push_back({plane, {
                        dropplaneFields->ColorID,
                        dropplaneFields->ModelAsset1P,
                        dropplaneFields->ModelAsset3P,
                        dropplaneFields->ModelAssetCutScene,
                        dropplaneFields->ModelAssetUI,
                        dropplaneFields->Priority
                    }});

                    if (getplane.size() == BRDropPlaneSkinConfigInstance.size()) {
                        break;
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
    return NULL;
}
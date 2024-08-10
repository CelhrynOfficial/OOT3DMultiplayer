#pragma once

#include "base_menu.hpp"
#include <vector>
#include <string>
#include <string_view>

namespace PersonalizationMenu {

extern std::vector<std::string> gauntletOptions;
extern std::vector<std::string> childTunicOptions;
extern std::vector<std::string> tunicOptions;
extern std::vector<std::string> naviOuterOptionNames;
extern std::vector<std::string> naviInnerOptionNames;
extern std::vector<std::string> weaponTrailOuterOptionNames;
extern std::vector<std::string> weaponTrailInnerOptionNames;
extern std::vector<std::string> trailDurationOptionNames;
extern std::vector<std::string> chuTrailDurationOptionNames;
extern std::vector<std::string_view> cosmeticDescriptions;


extern Option ChildTunicColor;
extern Option KokiriTunicColor;
extern Option GoronTunicColor;
extern Option ZoraTunicColor;

extern Option ChildTunicColor_1;
extern Option KokiriTunicColor_1;
extern Option GoronTunicColor_1;
extern Option ZoraTunicColor_1;


extern Option SilverGauntletsColor;
extern Option GoldGauntletsColor;
extern Option CustomNaviColors;
extern Option IdleNaviInnerColor;
extern Option NPCNaviInnerColor;
extern Option EnemyNaviInnerColor;
extern Option PropNaviInnerColor;
extern Option IdleNaviOuterColor;
extern Option NPCNaviOuterColor;
extern Option EnemyNaviOuterColor;
extern Option PropNaviOuterColor;
extern Option CustomTrailEffects;
extern Option ChosenSimpleMode;
extern Option SwordTrailInnerColor;
extern Option SwordTrailOuterColor;
extern Option SwordTrailDuration;
extern Option BoomerangTrailColor;
extern Option BoomerangTrailDuration;
extern Option BombchuTrailInnerColor;
extern Option BombchuTrailOuterColor;
extern Option BombchuTrailDuration;
extern Option ColoredKeys;
extern Option ColoredBossKeys;
extern Option BetaSoldOut;

extern std::string finalChildTunicColor;
extern std::string finalKokiriTunicColor;
extern std::string finalGoronTunicColor;
extern std::string finalZoraTunicColor;

extern std::string finalChildTunicColor_1;
extern std::string finalKokiriTunicColor_1;
extern std::string finalGoronTunicColor_1;
extern std::string finalZoraTunicColor_1;

extern std::string finalSilverGauntletsColor;
extern std::string finalGoldGauntletsColor;
extern std::string finalIdleNaviInnerColor;
extern std::string finalNPCNaviInnerColor;
extern std::string finalEnemyNaviInnerColor;
extern std::string finalPropNaviInnerColor;
extern std::string finalIdleNaviOuterColor;
extern std::string finalNPCNaviOuterColor;
extern std::string finalEnemyNaviOuterColor;
extern std::string finalPropNaviOuterColor;
extern std::string finalSwordTrailOuterColor;
extern std::string finalSwordTrailInnerColor;
extern Cosmetics::Color_RGBA8 finalBoomerangColor;

extern u8 boomerangTrailColorMode;
extern std::string finalChuTrailInnerColor;
extern std::string finalChuTrailOuterColor;

extern std::vector<Option *> cosmeticOptions;

extern Menu cosmetics;

#define RAINBOW_NAVI (PersonalizationMenu::naviOuterOptionNames.size() - 2)
#define SAME_AS_INNER_NAVI (PersonalizationMenu::naviOuterOptionNames.size() - 1)

#define RAINBOW_TUNIC (PersonalizationMenu::childTunicOptions.size() - 2)
#define SAME_AS_KOKIRI (PersonalizationMenu::childTunicOptions.size() - 1)

#define RAINBOW_TRAIL (PersonalizationMenu::weaponTrailOuterOptionNames.size() - 2)
#define SAME_AS_INNER_TRAIL (PersonalizationMenu::weaponTrailOuterOptionNames.size() - 1)

}
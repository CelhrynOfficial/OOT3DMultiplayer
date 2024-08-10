#include "settings.hpp"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <functional>

#include "settings/menus.hpp"
#include "menu_registry.hpp"
#include "multiplayer.hpp"
#include "category.hpp"
#include "descriptions.hpp"

namespace Settings {
std::string seed;
std::string version = MULTIPLAYER_VERSION "-" COMMIT_NUMBER;
std::array<u8, 5> hashIconIndexes;

u8 PlayOption;
u8 Region;

SettingsContext FillContext() {
    
    SettingsContext ctx = {};
    memset(reinterpret_cast<void*>(&ctx), 0, sizeof(SettingsContext)); // set all fields to false

    ctx.hashIndexes[0] = hashIconIndexes[0];
    ctx.hashIndexes[1] = hashIconIndexes[1];
    ctx.hashIndexes[2] = hashIconIndexes[2];
    ctx.hashIndexes[3] = hashIconIndexes[3];
    ctx.hashIndexes[4] = hashIconIndexes[4];
    ctx.region         = Region;

    ctx.mp_Enabled        = 1;
    ctx.mp_SyncId         = 1;
    ctx.mp_SharedProgress = (MultiplayerMenu::MP_SharedProgress) ? 1 : 1;
    ctx.mp_SharedAmmo     = (MultiplayerMenu::MP_SharedAmmo) ? 1 : 0;
    ctx.mp_SharedHealth   = (MultiplayerMenu::MP_SharedHealth) ? 1 : 0;
    ctx.mp_SharedRupees   = (MultiplayerMenu::MP_SharedRupees) ? 1 : 0;

    ctx.startingAge         = 1;
    ctx.resolvedStartingAge = 1;

    ctx.playMusic = 1;
    ctx.playSFX = 1;

    ctx.customTunicColors           = 1;
    // ctx.rainbowChildTunic           = (PersonalizationMenu::ChildTunicColor.Value<u8>() == RAINBOW_TUNIC) ? 1 : 0;
    // ctx.rainbowKokiriTunic          = (PersonalizationMenu::KokiriTunicColor.Value<u8>() == RAINBOW_TUNIC) ? 1 : 0;
    // ctx.rainbowGoronTunic           = (PersonalizationMenu::GoronTunicColor.Value<u8>() == RAINBOW_TUNIC) ? 1 : 0;
    // ctx.rainbowZoraTunic            = (PersonalizationMenu::ZoraTunicColor.Value<u8>() == RAINBOW_TUNIC) ? 1 : 0;
    ctx.rainbowChildTunic           = 0;
    ctx.rainbowKokiriTunic          = 0;
    ctx.rainbowGoronTunic           = 0;
    ctx.rainbowZoraTunic            = 0;
    
    Cosmetics::Color_RGBAf childTunicColor  = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalChildTunicColor);
    Cosmetics::Color_RGBAf kokiriTunicColor = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalKokiriTunicColor);
    Cosmetics::Color_RGBAf goronTunicColor  = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalGoronTunicColor);
    Cosmetics::Color_RGBAf zoraTunicColor   = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalZoraTunicColor);

    ctx.childTunicColor.r  = childTunicColor.r;
    ctx.childTunicColor.g  = childTunicColor.g;
    ctx.childTunicColor.b  = childTunicColor.b;
    ctx.childTunicColor.a  = childTunicColor.a;

    ctx.kokiriTunicColor.r = kokiriTunicColor.r;
    ctx.kokiriTunicColor.g = kokiriTunicColor.g;
    ctx.kokiriTunicColor.b = kokiriTunicColor.b;
    ctx.kokiriTunicColor.a = kokiriTunicColor.a;

    ctx.goronTunicColor.r  = goronTunicColor.r;
    ctx.goronTunicColor.g  = goronTunicColor.g;
    ctx.goronTunicColor.b  = goronTunicColor.b;
    ctx.goronTunicColor.a  = goronTunicColor.a;

    ctx.zoraTunicColor.r   = zoraTunicColor.r;
    ctx.zoraTunicColor.g   = zoraTunicColor.g;
    ctx.zoraTunicColor.b   = zoraTunicColor.b;
    ctx.zoraTunicColor.a   = zoraTunicColor.a;

    return ctx;
}

std::string TitleId() {
    if (Region == REGION_EUR) {
        return "0004000000033600";
    } else { // REGION_NA
        return "0004000000033500";
    }
}

// Function to make options vectors for Navi and Tunic colors without the "Same as ..." option
std::vector<std::string> VectorCopyExceptLastElement(std::vector<std::string> vect) {
    std::vector<std::string> copy(vect.begin(), std::prev(vect.end()));
    return copy;
}

std::vector<std::string> NumOpts(int min, int max, int step, std::string textBefore, std::string textAfter) {
    std::vector<std::string> options;
    options.reserve((max - min) / step + 1);
    for (int i = min; i <= max; i += step) {
        options.push_back(textBefore + std::to_string(i) + textAfter);
    }
    return options;
}

// If this is an option menu, return the options
// Else, recursively call each sub menu of this sub menu
const std::vector<Menu*> GetMenusRecursive(Menu* menu) {
    std::vector<Menu*> menus;
    if (menu->mode == OPTION_MENU) {
        menus.push_back(menu);
    } else if (menu->mode == SUB_MENU) {
        for (Menu* subMenu : *menu->itemsList) {
            std::vector<Menu*> foundMenus = GetMenusRecursive(subMenu);
            menus.insert(menus.end(), foundMenus.begin(), foundMenus.end());
        }
    }
    return menus;
}

// Recursively look through each menu from the main menu to get all settings
const std::vector<Menu*> GetAllOptionMenus() {
    std::vector<Menu*> allMenus;
    for (Menu* menu : MenuRegistry::GetMenuList()) {
        std::vector<Menu*> foundMenus = GetMenusRecursive(menu);
        allMenus.insert(allMenus.end(), foundMenus.begin(), foundMenus.end());
    }
    return allMenus;
}

// One-time initialization
void InitSettings() {
    SetDefaultSettings();
}

void UpdateSettings() {
    UpdateCosmetics();
}

template <typename colorsArray>
static void ChooseFinalColor(Option& cosmeticOption, std::string& colorStr, const colorsArray& colors) {
    if (cosmeticOption.Is(CUSTOM_COLOR)) {
        colorStr = GetCustomColor(cosmeticOption.GetSelectedOptionText());
    } else {
        colorStr = colors[cosmeticOption.GetSelectedOptionIndex() - NON_COLOR_COUNT];
    }
}

static void ChooseFinalNaviColor(Option& innerColorOption, Option& outerColorOption, std::string& innerColorStr,
                                 std::string& outerColorStr) {
    ChooseFinalColor(innerColorOption, innerColorStr, naviInnerColors);

    // If the outer color is "Same as Inner", only random and custom colors are copied directly via the hex code.
    // If instead a listed color is chosen (even through Random Choice), the outer color will use the same index as the
    // inner one to preserve color palettes (e.g. Midna giving black inner and gold outer).
    if (outerColorOption.Is(SAME_AS_INNER_NAVI) &&
        (innerColorOption.Is(RANDOM_COLOR) || innerColorOption.Is(CUSTOM_COLOR))) {
        outerColorStr = innerColorStr;
    } else if (outerColorOption.Is(SAME_AS_INNER_NAVI)) {
        outerColorOption.SetSelectedIndex(innerColorOption.Value<u8>());
        ChooseFinalColor(outerColorOption, outerColorStr, naviOuterColors);
    } else {
        ChooseFinalColor(outerColorOption, outerColorStr, naviOuterColors);
    }
}

using namespace PersonalizationMenu;

// Function to update cosmetics options depending on choices
void UpdateCosmetics() {
    // Tunics
    ChooseFinalColor(KokiriTunicColor, finalKokiriTunicColor, tunicColors);
    ChooseFinalColor(GoronTunicColor, finalGoronTunicColor, tunicColors);
    ChooseFinalColor(ZoraTunicColor, finalZoraTunicColor, tunicColors);

    if (ChildTunicColor.Is(SAME_AS_KOKIRI)) {
        finalChildTunicColor = finalKokiriTunicColor;
        ChildTunicColor.SetSelectedIndex(KokiriTunicColor.Value<u8>());
    } else {
        ChooseFinalColor(ChildTunicColor, finalChildTunicColor, tunicColors);
    }

    ChooseFinalColor(KokiriTunicColor_1, finalKokiriTunicColor_1, tunicColors);
    ChooseFinalColor(GoronTunicColor_1, finalGoronTunicColor_1, tunicColors);
    ChooseFinalColor(ZoraTunicColor_1, finalZoraTunicColor_1, tunicColors);

    if (ChildTunicColor.Is(SAME_AS_KOKIRI)) {
        finalChildTunicColor_1 = finalKokiriTunicColor_1;
        ChildTunicColor.SetSelectedIndex(KokiriTunicColor_1.Value<u8>());
    } else {
        ChooseFinalColor(ChildTunicColor_1, finalChildTunicColor_1, tunicColors);
    }
}

// Set default settings for all settings
void SetDefaultSettings() {
    std::function<void(std::vector<Menu*>*)> setAllOptionsToDefault = [&](std::vector<Menu*>* menuPtr) {
        for (auto menu : *menuPtr) {
            if (menu->mode == SUB_MENU) {
                setAllOptionsToDefault(menu->itemsList);
            } else if (menu->mode == OPTION_MENU) {
                for (auto option : *menu->settingsList) {
                    if (option->IsCategory(OptionCategory::Cosmetic)) {
                        continue;
                    }
                    option->SetToDefault();
                }
            }
        }
    };

    setAllOptionsToDefault(&MenuRegistry::GetMenuList());

    for (Menu* menu : Settings::GetAllOptionMenus()) {
        menu->ResetMenuIndex();
    }
}

/* Menus Static Init */
Menu generateMultiplayer = Menu::Action("Generate Multiplayer", GENERATE_MODE);
REGISTER_MENU(generateMultiplayer);

} // namespace Settings
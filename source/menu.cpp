#include <3ds.h>
#include <dirent.h>

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#include "multiplayer.hpp"

#include "menu.hpp"
#include "patch.hpp"
#include "settings.hpp"

namespace {
bool seedChanged;
bool chosePlayOption;
u16 pastSeedLength;
PrintConsole topScreen, bottomScreen;
std::vector<std::string> presetEntries;
std::vector<Menu*> menuList;
Option* currentSetting;
Menu* currentMenu;
} // namespace

void PrintTopScreen() {
    consoleSelect(&topScreen);
    consoleClear();
    printf("\x1b[2;11H%sOcarina of Time 3D Multiplayer%s", CYAN, RESET);
    printf("\x1b[3;18H%s%s-%s%s", CYAN, MULTIPLAYER_VERSION, COMMIT_NUMBER, RESET);
    printf("\x1b[4;10HA/B/D-pad: Navigate Menu\n");
    printf("            Select: Exit to Homebrew Menu\n");
    // printf("                 Y: New Random Seed\n");
    // printf("                 X: Input Custom Seed\n");
    // printf("\x1b[11;7HCurrent Seed: %s", Settings::seed.c_str());
}

void MenuInit() {
    Settings::InitSettings();

    seedChanged     = false;
    chosePlayOption = false;
    pastSeedLength  = Settings::seed.length();

    Menu* main = new Menu("Main", MenuType::MainMenu, &MenuRegistry::GetMenuList(), MAIN_MENU);
    menuList.push_back(main);
    currentMenu = main;

    srand(time(NULL));
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    consoleSelect(&topScreen);

    PrintTopScreen();

    consoleSelect(&bottomScreen);
    PrintMainMenu();
}

void PrintMainMenu() {
    printf("\x1b[0;%dHMain Settings", 1 + (BOTTOM_WIDTH - 13) / 2);

    for (u8 i = 0; i < MAX_SUBMENUS_ON_SCREEN; i++) {
        if (i >= MenuRegistry::GetMenuList().size())
            break;

        Menu* menu = MenuRegistry::GetMenuList()[i];

        u8 row = 3 + i;
        // make the current menu green
        if (currentMenu->menuIdx == i) {
            printf("\x1b[%d;%dH%s>", row, 2, GREEN);
            printf("\x1b[%d;%dH%s%s", row, 3, menu->name.c_str(), RESET);
        } else {
            printf("\x1b[%d;%dH%s", row, 3, menu->name.c_str());
        }
    }

    PrintDescription(currentMenu->itemsList->at(currentMenu->menuIdx)->description);
}

void PrintDescription(std::string_view description) {
    ClearDescription();
    printf("\x1b[20;0H%s", description.data());
}

void GenerateMultiplayer() {
    consoleSelect(&topScreen);
    consoleClear();

    Settings::UpdateSettings();

    printf("\x1b[13;10HWriting Patch...");

    if (WriteAllPatches()) {
        printf("Done");
        if (Settings::PlayOption == PATCH_CONSOLE) {
            printf("\x1b[15;10HQuit out using the home menu. Then\n");
            printf("\x1b[16;10Henable game patching and launch OoT3D!\n");
        } else if (Settings::PlayOption == PATCH_CITRA) {
            printf("\x1b[15;10HCopy code.ips, exheader.bin and romfs to\n");
            printf("\x1b[16;10Hthe OoT3D mods folder, then launch OoT3D!\n");
        }
    } else {
        printf("Failed\n\nPress Select to exit.\n");
    }
}

void ClearDescription() {
    consoleSelect(&topScreen);

    // clear the previous description
    std::string spaces = "";
    spaces.append(11 * TOP_WIDTH, ' ');
    printf("\x1b[20;0H%s", spaces.c_str());
}

void UpdateOptionSubMenu(u32 kDown, u32 kHeld) {
    bool fastScrolling = kHeld & KEY_A;
    currentSetting->ScrollOptionIndex(kDown, fastScrolling);
    currentSetting->SetVariable();
    // Settings::ForceChange(kDown, currentSetting);
    // UpdateCustomCosmeticColors(kDown);
}

void PrintOptionSubMenu() {
    // bounds checking incase settings go off screen
    // this is complicated to account for hidden settings and there's probably a better way to do it
    u16 hiddenSettings  = 0;
    u16 visibleSettings = 0;
    for (u16 i = currentMenu->settingBound; visibleSettings < MAX_SUBMENU_SETTINGS_ON_SCREEN; i++) {
        if (i >= currentMenu->settingsList->size()) {
            break;
        }
        if (currentMenu->settingsList->at(i)->IsHidden()) {
            hiddenSettings++;
        } else {
            visibleSettings++;
        }
    }
    bool isLastVisibleSetting = true;
    for (size_t i = currentMenu->menuIdx + 1; i < currentMenu->settingsList->size(); i++) {
        if (!currentMenu->settingsList->at(i)->IsHidden()) {
            isLastVisibleSetting = false;
            break;
        }
    }
    if (currentMenu->menuIdx >=
        currentMenu->settingBound - (isLastVisibleSetting ? 0 : 1) + MAX_SUBMENU_SETTINGS_ON_SCREEN + hiddenSettings) {
        currentMenu->settingBound = currentMenu->menuIdx;
        u8 offset                 = 0;
        // skip over hidden settings
        while (offset < MAX_SUBMENU_SETTINGS_ON_SCREEN - (isLastVisibleSetting ? 1 : 2)) {
            currentMenu->settingBound--;
            if (currentMenu->settingBound == 0) {
                break;
            }
            offset += currentMenu->settingsList->at(currentMenu->settingBound)->IsHidden() ? 0 : 1;
        }
    } else if (currentMenu->menuIdx < currentMenu->settingBound + 1) {
        currentMenu->settingBound = std::max(currentMenu->menuIdx - 1, 0);
    }

    // print menu name
    printf("\x1b[0;%dH%s", 1 + (BOTTOM_WIDTH - currentMenu->name.length()) / 2, currentMenu->name.c_str());

    // keep count of hidden settings to not make blank spaces appear in the list
    hiddenSettings = 0;

    for (u8 i = 0; i - hiddenSettings < MAX_SUBMENU_SETTINGS_ON_SCREEN; i++) {
        // break if there are no more settings to print
        if (i + currentMenu->settingBound >= currentMenu->settingsList->size())
            break;

        Option* setting = currentMenu->settingsList->at(i + currentMenu->settingBound);

        u8 row = 3 + ((i - hiddenSettings) * 2);

        const auto PrintSettingAndOption = [&](std::string color) {
            size_t newlinePos;
            bool hasNewline;

            printf(color.data());

            // Selection Arrow
            if (color == GREEN) {
                printf("\x1b[%d;%dH>", row, 1);
            }

            // Setting
            newlinePos = setting->GetName().find('\n');
            hasNewline = newlinePos != std::string::npos;
            printf("\x1b[%d;%dH%.*s%s", row, 2 + setting->GetIndent(), newlinePos, setting->GetName().data(),
                   hasNewline ? "" : ":");
            if (hasNewline) {
                printf("\x1b[%d;%dH%s:", row + 1, 2 + setting->GetIndent(), setting->GetName().data() + newlinePos + 1);
            }

            // Option
            newlinePos = setting->GetSelectedOptionText().find('\n');
            hasNewline = newlinePos != std::string::npos;
            printf("\x1b[%d;%dH%.*s", row, 26, newlinePos, setting->GetSelectedOptionText().data());
            if (hasNewline) {
                printf("\x1b[%d;%dH%s", row + 1, 26, setting->GetSelectedOptionText().data() + newlinePos + 1);
            }

            printf(RESET);
        };

        // make the current setting green
        if (currentMenu->menuIdx == i + currentMenu->settingBound) {
            PrintSettingAndOption(GREEN);
        }
        // dim to make a locked setting grey
        else if (setting->IsLocked()) {
            PrintSettingAndOption(DIM);
        }
        // don't display hidden settings
        else if (setting->IsHidden()) {
            hiddenSettings++;
            continue;
        } else {
            PrintSettingAndOption(WHITE);
        }
    }

    PrintDescription(currentSetting->GetSelectedOptionDescription());
}

void UpdateResetToDefaultsMenu(u32 kDown) {
    consoleSelect(&topScreen);
    // clear any potential message
    ClearDescription();
    if (kDown & KEY_A) {
        Settings::SetDefaultSettings();
        printf("\x1b[24;7HSettings have been reset to defaults.");
    }
}

void PrintResetToDefaultsMenu() {
    consoleSelect(&bottomScreen);
    printf("\x1b[10;2HPress A to reset to default settings.");
    printf("\x1b[12;2HPress B to return to the preset menu.");
}

void MoveCursor(u32 kDown, bool updatedByHeld) {
    // Option sub menus need special checking for locked options
    if (currentMenu->mode == OPTION_MENU) {
        // Cancel if holding and reached first/last selectable option
        if (updatedByHeld) {
            bool noSelectableOption = true;
            if (kDown & KEY_UP) {
                for (int i = currentMenu->menuIdx - 1; i >= 0; i--) {
                    if (!currentMenu->settingsList->at(i)->IsHidden() &&
                        !currentMenu->settingsList->at(i)->IsLocked()) {
                        noSelectableOption = false;
                        break;
                    }
                }
            }
            if (kDown & KEY_DOWN) {
                for (size_t i = currentMenu->menuIdx + 1; i < currentMenu->settingsList->size(); i++) {
                    if (!currentMenu->settingsList->at(i)->IsHidden() &&
                        !currentMenu->settingsList->at(i)->IsLocked()) {
                        noSelectableOption = false;
                        break;
                    }
                }
            }
            if (noSelectableOption) {
                return;
            }
        }
        // Loop through settings until an unlocked one is reached
        do {
            if ((kDown & KEY_UP) != 0) {
                currentMenu->menuIdx--;
            }
            if ((kDown & KEY_DOWN) != 0) {
                currentMenu->menuIdx++;
            }

            // Bounds checking
            if (currentMenu->menuIdx == currentMenu->settingsList->size()) {
                currentMenu->menuIdx = 0;
            } else if (currentMenu->menuIdx == 0xFFFF) {
                currentMenu->menuIdx = static_cast<u16>(currentMenu->settingsList->size() - 1);
            }

            currentSetting = currentMenu->settingsList->at(currentMenu->menuIdx);
        } while (currentSetting->IsLocked() || currentSetting->IsHidden());
    }
    // All other menus except reset-to-defaults confirmation
    else if (currentMenu->mode != RESET_TO_DEFAULTS) {
        u16 max = -1;
        if (currentMenu->mode == GENERATE_MODE) { // Generate menu: 2 options
            max = 2;
        } else if (currentMenu->itemsList != nullptr) {
            max = currentMenu->itemsList->size(); // Default max: Number of items in menu
        }

        // Cancel if holding and reached first/last menu
        if (updatedByHeld) {
            if ((kDown & KEY_UP && currentMenu->menuIdx == 0) ||
                (kDown & KEY_DOWN && currentMenu->menuIdx == max - 1)) {
                return;
            }
        }

        // If this is a sub-menu, loop through items until an unlocked one is reached
        do {
            if (kDown & KEY_UP) {
                currentMenu->menuIdx--;
            }
            if (kDown & KEY_DOWN) {
                currentMenu->menuIdx++;
            }

            // Bounds checking
            if (currentMenu->menuIdx == max) {
                currentMenu->menuIdx = 0;
            } else if (currentMenu->menuIdx == 0xFFFF) {
                currentMenu->menuIdx = max - 1;
            }
        } while (currentMenu->mode == SUB_MENU && currentMenu->itemsList->at(currentMenu->menuIdx)->IsLocked());

        // Scroll Check
        u16 max_entries_on_screen = MAX_SUBMENUS_ON_SCREEN;
        if (currentMenu->menuIdx >
            currentMenu->settingBound + (max_entries_on_screen - (currentMenu->menuIdx == max - 1 ? 1 : 2))) {
            currentMenu->settingBound =
                currentMenu->menuIdx - (max_entries_on_screen - (currentMenu->menuIdx == max - 1 ? 1 : 2));
        } else if (currentMenu->menuIdx < currentMenu->settingBound + 1) {
            currentMenu->settingBound = std::max(currentMenu->menuIdx - 1, 0);
        }
    }
}

void ModeChangeInit() {
    if (currentMenu->mode == OPTION_MENU) {
        // loop through until we reach an unlocked setting
        while (currentMenu->settingsList->at(currentMenu->menuIdx)->IsLocked() ||
               currentMenu->settingsList->at(currentMenu->menuIdx)->IsHidden()) {
            currentMenu->menuIdx++;
            if (currentMenu->menuIdx >= currentMenu->settingsList->size()) {
                currentMenu->menuIdx = 0;
            }
        }
        currentSetting = currentMenu->settingsList->at(currentMenu->menuIdx);

    } else if (currentMenu->mode == GENERATE_MODE) {
    }
}

void PrintSubMenu() {
    printf("\x1b[0;%dH%s", 1 + (BOTTOM_WIDTH - currentMenu->name.length()) / 2, currentMenu->name.c_str());

    for (u8 i = 0; i < MAX_SUBMENUS_ON_SCREEN; i++) {
        if (i >= currentMenu->itemsList->size())
            break;

        u8 row             = 3 + i;
        Menu* selectedMenu = currentMenu->itemsList->at(currentMenu->settingBound + i);
        // make the current menu green
        if (currentMenu->menuIdx == currentMenu->settingBound + i) {
            printf("\x1b[%d;%dH%s>", row, 2, GREEN);
            printf("\x1b[%d;%dH%s%s", row, 3, selectedMenu->name.c_str(), RESET);
        } else if (selectedMenu->IsLocked()) {
            // Make locked menus gray.
            printf("\x1b[%d;%dH%s%s%s", row, 3, DIM, selectedMenu->name.c_str(), RESET);
        } else {
            printf("\x1b[%d;%dH%s", row, 3, selectedMenu->name.c_str());
        }
    }

    PrintDescription(currentMenu->itemsList->at(currentMenu->menuIdx)->description);
}

void UpdateGenerateMenu(u32 kDown) {
    if (!chosePlayOption) {
        if ((kDown & KEY_A) != 0) {
            Settings::PlayOption = currentMenu->menuIdx;
            consoleSelect(&bottomScreen);
            consoleClear();
            chosePlayOption = true;
        }
    } else {
        if ((kDown & KEY_B) != 0) {
            consoleSelect(&bottomScreen);
            consoleClear();
            chosePlayOption = false;
        } else if ((kDown & KEY_A) != 0) {
            Settings::Region = currentMenu->menuIdx;
            consoleSelect(&bottomScreen);
            consoleClear();
            GenerateMultiplayer();
            // This is just a dummy mode to stop the prompt from appearing again
            currentMenu->mode = POST_GENERATE;
            chosePlayOption   = false;
        }
    }
}

void PrintGenerateMenu() {

    consoleSelect(&bottomScreen);

    if (!chosePlayOption) {
        printf("\x1b[3;%dHHow will you play?", 1 + (BOTTOM_WIDTH - 18) / 2);
        std::vector<std::string> playOptions = { "3ds Console", "Citra Emulator" };

        for (u8 i = 0; i < playOptions.size(); i++) {

            std::string option = playOptions[i];
            u8 row             = 6 + (i * 2);
            // make the current selection green
            if (currentMenu->menuIdx == i) {
                printf("\x1b[%d;%dH%s>", row, 14, GREEN);
                printf("\x1b[%d;%dH%s%s", row, 15, option.c_str(), RESET);
            } else {
                printf("\x1b[%d;%dH%s", row, 15, option.c_str());
            }
        }
    } else {
        printf("\x1b[3;%dHSelect your game region", 1 + (BOTTOM_WIDTH - 18) / 2);
        std::vector<std::string> regionOptions = { "North America (33500)", "Europe (33600)" };

        for (u8 i = 0; i < regionOptions.size(); i++) {

            std::string option = regionOptions[i];
            u8 row             = 6 + (i * 2);
            // make the current selection green
            if (currentMenu->menuIdx == i) {
                printf("\x1b[%d;%dH%s>", row, 14, GREEN);
                printf("\x1b[%d;%dH%s%s", row, 15, option.c_str(), RESET);
            } else {
                printf("\x1b[%d;%dH%s", row, 15, option.c_str());
            }
        }
    }
}

void MenuUpdate(u32 kDown, bool updatedByHeld, u32 kHeld) {
    consoleSelect(&bottomScreen);
    if (currentMenu->mode != POST_GENERATE || (kDown & KEY_B)) {
        // Don't clear console if ValidateSettings printed error
        consoleClear();
    }

    if (kDown & KEY_A && currentMenu->mode != OPTION_MENU && currentMenu->type != MenuType::Action) {
        if (currentMenu->itemsList->size() > currentMenu->menuIdx) {
            Menu* newMenu;
            newMenu = currentMenu->itemsList->at(currentMenu->menuIdx);
            menuList.push_back(newMenu);
            currentMenu = menuList.back();
            ModeChangeInit();
            kDown = 0;
        }

        // If they pressed B on any menu other than main, go backwards to the previous menu
    } else if (kDown & KEY_B && currentMenu->mode != MAIN_MENU && !chosePlayOption) {
        // Want to reset generate menu when leaving
        if (currentMenu->mode == POST_GENERATE) {
            currentMenu->mode = GENERATE_MODE;
        }

        consoleSelect(&topScreen);
        PrintTopScreen();
        menuList.pop_back();
        currentMenu = menuList.back();
        ModeChangeInit();
        kDown = 0;
    }

    // Print current menu (if applicable)
    consoleSelect(&bottomScreen);
    MoveCursor(kDown, updatedByHeld); // Move cursor, if applicable
    if (currentMenu->mode == MAIN_MENU) {
        PrintMainMenu();
    } else if (currentMenu->mode == OPTION_MENU) {
        UpdateOptionSubMenu(kDown, kHeld);
        PrintOptionSubMenu();
    } else if (currentMenu->mode == RESET_TO_DEFAULTS) {
        UpdateResetToDefaultsMenu(kDown);
        PrintResetToDefaultsMenu();
    } else if (currentMenu->mode == SUB_MENU) {
        PrintSubMenu();
    } else if (currentMenu->mode == GENERATE_MODE) {
        UpdateGenerateMenu(kDown);
        if (currentMenu->mode != POST_GENERATE) {
            PrintGenerateMenu();
        }
    }
}
#pragma once

#include <3ds.h>

#include <array>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

#include "../code/src/settings.h"
#include "category.hpp"
#include "cosmetics.hpp"
#include "menu.hpp"
#include "utils.hpp"

class Option {
  public:
    static Option Bool(std::string name_, std::vector<std::string> options_,
                       std::vector<std::string_view> optionDescriptions_,
                       OptionCategory category_ = OptionCategory::Setting, u8 defaultOption_ = 0,
                       bool defaultHidden_ = false) {
        return Option{ false,     u8{ 0 },        std::move(name_), std::move(options_), std::move(optionDescriptions_),
                       category_, defaultOption_, defaultHidden_ };
    }

    static Option U8(std::string name_, std::vector<std::string> options_,
                     std::vector<std::string_view> optionDescriptions_,
                     OptionCategory category_ = OptionCategory::Setting, u8 defaultOption_ = 0,
                     bool defaultHidden_ = false) {
        return Option{ u8{ 0 },   u8{ 0 },        std::move(name_), std::move(options_), std::move(optionDescriptions_),
                       category_, defaultOption_, defaultHidden_ };
    }

    static Option Bool(u8 indent_, std::string name_, std::vector<std::string> options_,
                       std::vector<std::string_view> optionDescriptions_,
                       OptionCategory category_ = OptionCategory::Setting, u8 defaultOption_ = 0,
                       bool defaultHidden_ = false) {
        return Option{ false,     indent_,        std::move(name_), std::move(options_), std::move(optionDescriptions_),
                       category_, defaultOption_, defaultHidden_ };
    }

    static Option U8(u8 indent_, std::string name_, std::vector<std::string> options_,
                     std::vector<std::string_view> optionDescriptions_,
                     OptionCategory category_ = OptionCategory::Setting, u8 defaultOption_ = 1,
                     bool defaultHidden_ = false) {
        return Option{ u8{ 0 },   indent_,        std::move(name_), std::move(options_), std::move(optionDescriptions_),
                       category_, defaultOption_, defaultHidden_ };
    }

    template <typename T> T Value() const {
        return std::get<T>(var);
    }

    template <typename T> bool Is(T other) const {
        static_assert(std::is_integral_v<T> || std::is_enum_v<T>, "T must be an integral type or an enum.");

        if constexpr ((std::is_integral_v<T> && !std::is_same_v<bool, T>) || std::is_enum_v<T>) {
            return Value<u8>() == static_cast<u8>(other);
        } else {
            return Value<bool>() == static_cast<bool>(other);
        }
    }

    template <typename T> bool IsNot(T other) const {
        return !Is(other);
    }

    explicit operator bool() const {
        if (std::holds_alternative<bool>(var)) {
            return Value<bool>();
        } else {
            return Value<u8>() != 0;
        }
    }

    void SetOptions(std::vector<std::string> o) {
        options = std::move(o);
        SetToDefault();
    }

    const std::vector<std::string>& GetOptions() {
        return options;
    }

    size_t GetOptionCount() const {
        return options.size();
    }

    u8 GetIndent() const {
        return indent;
    }

    const std::string& GetName() const {
        return name;
    }

    const std::string& GetSelectedOptionText() const {
        return options[selectedOption];
    }

    void SetSelectedOptionText(std::string newText) {
        options[selectedOption] = std::move(newText);
    }

    bool IsDefaultSelected() {
        return selectedOption == defaultOption;
    }

    void SetToDefault() {
        SetSelectedIndex(defaultOption);
        hidden = defaultHidden;
    }

    std::string_view GetSelectedOptionDescription() const {
        // bounds checking
        if (selectedOption >= optionDescriptions.size()) {
            return optionDescriptions[optionDescriptions.size() - 1];
        }
        return optionDescriptions[selectedOption];
    }

    u8 GetSelectedOptionIndex() const {
        return selectedOption;
    }

    void ScrollOptionIndex(u32 kDown, bool fastScrolling = false) {
        u32 scrollSpeed = (fastScrolling && options.size() > 2) ? 10 : 1;

        if ((kDown & KEY_RIGHT) != 0) {
            selectedOption += scrollSpeed;
            if (selectedOption >= options.size()) {
                selectedOption %= options.size();
            }
        } else if ((kDown & KEY_LEFT) != 0) {
            selectedOption -= scrollSpeed;
            if (selectedOption >= options.size()) {
                selectedOption = (options.size() - 1) - ((UINT8_MAX - selectedOption) % options.size());
            }
        }
    }

    void SetVariable() {
        if (std::holds_alternative<bool>(var)) {
            var.emplace<bool>(selectedOption != 0);
        } else {
            var.emplace<u8>(selectedOption);
        }
    }

    void SetDelayedOption() {
        delayedOption = selectedOption;
    }

    void RestoreDelayedOption() {
        selectedOption = delayedOption;
        SetVariable();
    }

    void SetSelectedIndex(size_t idx) {
        selectedOption = idx;
        if (selectedOption >= options.size()) {
            printf("\x1b[30;0HERROR: Incompatible selection for %s\n", name.c_str());
            selectedOption = 0;
        }

        SetVariable();
    }

    void SetSelectedIndexByString(std::string newSetting) {
        using namespace Cosmetics;

        // Special case for custom cosmetic settings
        if (options.size() > CUSTOM_COLOR) {
            if (newSetting.compare(0, 8, CUSTOM_COLOR_PREFIX) == 0 &&
                options[CUSTOM_COLOR].compare(0, 8, CUSTOM_COLOR_PREFIX) == 0) {
                SetSelectedIndex(CUSTOM_COLOR);
                SetSelectedOptionText(newSetting);
                return;
            }
        }

        for (size_t i = 0; i < options.size(); i++) {
            std::string settingName = SanitizedString(options[i]);
            if (settingName == newSetting) {
                SetSelectedIndex(i);
                return;
            }
        }
    }

    void Lock() {
        locked = true;
    }

    void Unlock() {
        locked = false;
    }

    bool IsLocked() const {
        return locked;
    }

    void Hide() {
        hidden = true;
    }

    void Unhide() {
        hidden = false;
    }

    bool IsHidden() const {
        return hidden;
    }

    bool IsCategory(OptionCategory category) const {
        return category == this->category;
    }

  private:
    Option(u8 var_, u8 indent_, std::string name_, std::vector<std::string> options_,
           std::vector<std::string_view> optionDescriptions_, OptionCategory category_, u8 defaultOption_,
           bool defaultHidden_)
        : var(var_), indent(indent_), name(std::move(name_)), options(std::move(options_)),
          optionDescriptions(std::move(optionDescriptions_)), category(category_), defaultOption(defaultOption_),
          defaultHidden(defaultHidden_) {
        selectedOption = defaultOption;
        hidden         = defaultHidden;
        SetVariable();
    }

    Option(bool var_, u8 indent_, std::string name_, std::vector<std::string> options_,
           std::vector<std::string_view> optionDescriptions_, OptionCategory category_, u8 defaultOption_,
           bool defaultHidden_)
        : var(var_), indent(indent_), name(std::move(name_)), options(std::move(options_)),
          optionDescriptions(std::move(optionDescriptions_)), category(category_), defaultOption(defaultOption_),
          defaultHidden(defaultHidden_) {
        selectedOption = defaultOption;
        hidden         = defaultHidden;
        SetVariable();
    }

    std::variant<bool, u8> var;
    u8 indent = 0;
    std::string name;
    std::vector<std::string> options;
    std::vector<std::string_view> optionDescriptions;
    u8 selectedOption = 0;
    u8 delayedOption  = 0;
    bool locked       = false;
    bool hidden       = false;
    OptionCategory category;
    u8 defaultOption   = 0;
    bool defaultHidden = false;
};

enum class MenuType {
    MainMenu,
    SubMenu,
    Action,
};

class Menu {
  public:
    static Menu SubMenu(std::string name_, std::vector<Option*>* settingsList_, std::string_view description_ = "",
                        bool printInSpoiler_ = true) {
        return Menu{ std::move(name_), MenuType::SubMenu,       std::move(settingsList_),
                     OPTION_MENU,      std::move(description_), printInSpoiler_ };
    }

    static Menu SubMenu(std::string name_, std::vector<Menu*>* itemsList_, std::string_view description_ = "",
                        bool printInSpoiler_ = true) {
        return Menu{ std::move(name_), MenuType::SubMenu,       std::move(itemsList_),
                     SUB_MENU,         std::move(description_), printInSpoiler_ };
    }

    static Menu Action(std::string name_, u8 mode_, std::string_view description_ = "") {
        return Menu{ std::move(name_), MenuType::Action, std::move(mode_), std::move(description_) };
    }

    Menu(std::string name_, MenuType type_, std::vector<Option*>* settingsList_, u8 mode_,
         std::string_view description_ = "", bool printInSpoiler_ = true)
        : name(std::move(name_)), type(type_), settingsList(std::move(settingsList_)), mode(mode_),
          description(std::move(description_)), printInSpoiler(printInSpoiler_) {
    }

    Menu(std::string name_, MenuType type_, std::vector<Menu*>* itemsList_, u8 mode_,
         std::string_view description_ = "", bool printInSpoiler_ = true)
        : name(std::move(name_)), type(type_), itemsList(std::move(itemsList_)), mode(mode_),
          description(std::move(description_)), printInSpoiler(printInSpoiler_) {
    }

    Menu(std::string name_, MenuType type_, u8 mode_, std::string_view description_ = "")
        : name(std::move(name_)), type(type_), mode(mode_), description(std::move(description_)) {
    }

    void ResetMenuIndex() {
        if (mode == OPTION_MENU) {
            for (size_t i = 0; i < settingsList->size(); i++) {
                if (!settingsList->at(i)->IsLocked() && !settingsList->at(i)->IsHidden()) {
                    menuIdx      = i;
                    settingBound = i;
                    return;
                }
            }
        }
        menuIdx      = 0;
        settingBound = 0;
    }

    void Lock() {
        locked = true;
    }

    void Unlock() {
        locked = false;
    }

    bool IsLocked() const {
        return locked;
    }

    std::string name;
    MenuType type;
    std::vector<Option*>* settingsList;
    std::vector<Menu*>* itemsList;
    u8 mode;
    u16 menuIdx                  = 0;
    u16 settingBound             = 0;
    int selectedSetting          = 0;
    std::string_view description = "";
    bool printInSpoiler          = true;
    bool locked                  = false;
};

namespace Settings {
SettingsContext FillContext();    

extern std::string seed;
extern std::string version;
extern std::array<u8, 5> hashIconIndexes;

extern u8 PlayOption;
extern u8 Region;

std::string TitleId();

std::vector<std::string> VectorCopyExceptLastElement(std::vector<std::string> vect);
std::vector<std::string> NumOpts(int min, int max, int step = 1, std::string textBefore = {}, std::string textAfter = {});

void InitSettings();
void UpdateSettings();
void UpdateCosmetics();
void SetDefaultSettings();
const std::vector<Menu*> GetAllOptionMenus();
} // namespace Settings

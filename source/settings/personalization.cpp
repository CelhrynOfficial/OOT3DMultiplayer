#include "personalization.hpp"

namespace PersonalizationMenu {

std::vector<std::string> childTunicOptions = {    std::string(RANDOM_CHOICE_STR),
    std::string(RANDOM_COLOR_STR),
    std::string(CUSTOM_COLOR_STR),
    "Kokiri Green", "Goron Red", "Zora Blue",  "Black",       "White",          "Orange",   "Yellow",     "Cyan",
    "Indigo",       "Purple",    "Pink",       "Dark Gray",   "Salmon",         "Wine Red", "Beige",      "Brown",
    "Sand",         "Tea Green", "Dark Green", "Powder Blue", "Teal",           "Sky Blue", "Faded Blue", "Lavender",
    "Magenta",      "Mauve",     "Silver",     "Gold",        "Same as Kokiri",
};

std::vector<std::string> tunicOptions = VectorCopyExceptLastElement(childTunicOptions);

std::vector<std::string_view> cosmeticDescriptions = {
    RANDOM_CHOICE_DESC,
    RANDOM_COLOR_DESC,
    CUSTOM_COLOR_DESC,
    "This will only affect the color on Link's model.",
};

Option ChildTunicColor = Option::U8("Child Tunic Color", childTunicOptions, cosmeticDescriptions, OptionCategory::Cosmetic, SAME_AS_KOKIRI);
Option KokiriTunicColor = Option::U8("Kokiri Tunic Color", tunicOptions, cosmeticDescriptions, OptionCategory::Cosmetic, 0); // Kokiri Green
Option GoronTunicColor = Option::U8("Goron Tunic Color", tunicOptions, cosmeticDescriptions, OptionCategory::Cosmetic, 1); // Goron Red
Option ZoraTunicColor = Option::U8("Zora Tunic Color", tunicOptions, cosmeticDescriptions, OptionCategory::Cosmetic, 2); // Zora Blue

Option ChildTunicColor_1  = Option::U8(2, "Child Tunic Color (P2)", childTunicOptions, cosmeticDescriptions, OptionCategory::Cosmetic, SAME_AS_KOKIRI);
Option KokiriTunicColor_1 = Option::U8(2, "Kokiri Tunic Color (P2)", tunicOptions, cosmeticDescriptions, OptionCategory::Cosmetic, 3); // Kokiri Green
Option GoronTunicColor_1  = Option::U8(2, "Goron Tunic Color (P2)", tunicOptions, cosmeticDescriptions, OptionCategory::Cosmetic, 4); // Goron Red
Option ZoraTunicColor_1   = Option::U8(2, "Zora Tunic Color (P2)", tunicOptions, cosmeticDescriptions, OptionCategory::Cosmetic, 5); // Zora Blue

std::string finalChildTunicColor  = ChildTunicColor.GetSelectedOptionText();
std::string finalKokiriTunicColor = KokiriTunicColor.GetSelectedOptionText();
std::string finalGoronTunicColor  = GoronTunicColor.GetSelectedOptionText();
std::string finalZoraTunicColor   = ZoraTunicColor.GetSelectedOptionText();

std::string finalChildTunicColor_1  = ChildTunicColor.GetSelectedOptionText();
std::string finalKokiriTunicColor_1 = KokiriTunicColor.GetSelectedOptionText();
std::string finalGoronTunicColor_1  = GoronTunicColor.GetSelectedOptionText();
std::string finalZoraTunicColor_1   = ZoraTunicColor.GetSelectedOptionText();

std::vector<Option*> cosmeticOptions = {
    &ChildTunicColor, &KokiriTunicColor, &GoronTunicColor, &ZoraTunicColor,

    // &ChildTunicColor_1,
    // &KokiriTunicColor_1,
    // &GoronTunicColor_1,
    // &ZoraTunicColor_1,
};

Menu cosmetics = Menu::SubMenu("Cosmetic Settings", &cosmeticOptions);

REGISTER_MENU(cosmetics);

} // namespace PersonalizationMenu
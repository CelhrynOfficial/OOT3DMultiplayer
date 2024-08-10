#include "multiplayer.hpp"

namespace MultiplayerMenu {

Option MP_SharedProgress = Option::Bool("Shared Progress", {"Off", "On"},         {mp_SharedProgressDesc}, OptionCategory::Setting, 1);
Option MP_SharedHealth   = Option::Bool("Shared Health",   {"Off", "On"},         {mp_SharedHealthDesc});
Option MP_SharedRupees   = Option::Bool("Shared Rupees",   {"Off", "On"},         {mp_SharedRupeesDesc});
Option MP_SharedAmmo     = Option::Bool("Shared Ammo",     {"Off", "On"},         {mp_SharedAmmoDesc});

std::vector<Option*> multiplayerOptions = {
    &MP_SharedProgress,
    &MP_SharedHealth,
    &MP_SharedRupees,
    &MP_SharedAmmo,
};

Menu multiplayerSettings      = Menu::SubMenu("Multiplayer Settings",       &multiplayerOptions);

REGISTER_MENU(multiplayerSettings);

}
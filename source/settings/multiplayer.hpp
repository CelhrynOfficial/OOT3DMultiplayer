#pragma once

#include "base_menu.hpp"
#include <vector>

namespace MultiplayerMenu {

extern Option MP_SharedProgress;
extern Option MP_SyncId;
extern Option MP_SharedHealth;
extern Option MP_SharedRupees;
extern Option MP_SharedAmmo;

extern std::vector<Option*> multiplayerOptions;

extern Menu multiplayerSettings;

}
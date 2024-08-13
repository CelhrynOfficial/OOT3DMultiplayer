/*
*   This file is a modified part of Luma3DS
*   Copyright (C) 2016-2019 Aurora Wright, TuxSH
*   Modified 2020 Gamestabled
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "3dsDebug/os.h"
#include "menusDebug.h"
#include "menuDebug.h"
#include "drawDebug.h"
#include "z3DDebug/z3D.h"

#include "menusDebug/warps.h"
#include "menusDebug/scene.h"
#include "menusDebug/cheats.h"
#include "menusDebug/inventory.h"
#include "menusDebug/equips.h"
#include "menusDebug/file.h"
#include "menusDebug/watches.h"
#include "menusDebug/debug.h"
#include "menusDebug/commands.h"
#include "menusDebug/settings.h"

Menu gz3DMenu = {
    "Practice Menu",
    .nbItems = 10,
    .initialCursorPos = 0,
    {
        { "Warps", MENU, .menu = &WarpsMenu },
        { "Scene", MENU, .menu = &SceneMenu },
        { "Cheats", METHOD, .method = Cheats_ShowCheatsMenu },
        { "Inventory", MENU, .menu = &InventoryMenu },
        { "Equips", METHOD, .method = Equips_ShowEquipsMenu },
        { "File", METHOD, .method = File_ShowFileMenu },
        { "Watches", METHOD, .method = &WatchesMenuFunc },
        { "Debug", MENU, .menu = &DebugMenu },
        { "Commands", METHOD, .method = Commands_ShowCommandsMenu },
        { "Settings", MENU, .menu = &SettingsMenu },
    }
};

#pragma once

#include <vector>
#include <menu.hpp>

class Menu;

class MenuRegistry {
public:
    static void Register(Menu* menu) {
        GetMenuList().push_back(menu);
    }

    static std::vector<Menu*>& GetMenuList() {
        static std::vector<Menu*> menus;
        return menus;
    }
};

#define REGISTER_MENU(menu) static MenuRegistrar _registrar_##menu(&menu)

class MenuRegistrar {
public:
    MenuRegistrar(Menu* menu) {
        MenuRegistry::Register(menu);
    }
};
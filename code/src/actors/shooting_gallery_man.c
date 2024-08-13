#include "shooting_gallery_man.h"
#include "entrance.h"
#include "settings.h"

#define EnSyatekiMan_Init ((ActorFunc)GAME_ADDR(0x283F94))

#define EnSyatekiMan_Update ((ActorFunc)GAME_ADDR(0x2A60BC))

void EnSyatekiMan_rInit(Actor* thisx, GlobalContext* globalCtx) {
    EnSyatekiMan_Init(thisx, globalCtx);
}

void EnSyatekiMan_rUpdate(Actor* thisx, GlobalContext* globalCtx) {
    static u8 previouslyGettingItem = 0;
    u8 currentlyGettingItem         = ((PLAYER->stateFlags1 >> 10) & 1);

    EnSyatekiMan_Update(thisx, globalCtx);

    if (previouslyGettingItem && !currentlyGettingItem &&
        (gSaveContext.linkAge == 0 && (gSaveContext.upgrades & 0x7) == 0)) {
        DisplayTextbox(gGlobalContext, 0x9140, 0);
    }

    previouslyGettingItem = currentlyGettingItem;
}

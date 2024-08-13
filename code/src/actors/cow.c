#include "z3D/z3D.h"
#include "settings.h"
#include "entrance.h"
#include "cow.h"

#define EnCow_Init ((ActorFunc)GAME_ADDR(0x189FD4))

#define EnCow_Destroy ((ActorFunc)GAME_ADDR(0x18A3E4))

static s32 sNumCows = 0;

void EnCow_rInit(Actor* thisx, GlobalContext* globalCtx) {
    EnCow_Init(thisx, globalCtx);
}

void EnCow_rDestroy(Actor* thisx, GlobalContext* globalCtx) {
    sNumCows = 0;
    EnCow_Destroy(thisx, globalCtx);
}

u32 EnCow_BottleCheck(Actor* cow) {
    s16 cowId = cow->home.rot.x;

    // If cow doesn't give an item, or the collect flag is set, check for bottle
    // Otherwise, we give the item, return true
    if (!gSettingsContext.shuffleCows || (cowId == 0) || (gGlobalContext->actorCtx.flags.collect & (cowId << 0x18))) {
        return Inventory_HasEmptyBottle();
    } else {
        return 1;
    }
}

s32 EnCow_ItemOverride(Actor* cow) {
    s16 cowId = cow->home.rot.x;

    // If cow doesn't give an item, or the collect flag is set, give milk refill
    if (!gSettingsContext.shuffleCows || (cowId == 0) || (gGlobalContext->actorCtx.flags.collect & (cowId << 0x18))) {
        return GI_MILK;
    } else {
        gGlobalContext->actorCtx.flags.collect |= (cowId << 0x18);
        return GI_MILK_BOTTLE + cowId;
    }
}

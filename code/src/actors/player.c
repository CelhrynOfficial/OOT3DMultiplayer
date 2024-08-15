#include "z3D/z3D.h"
#include "objects.h"
#include "custom_models.h"
#include "settings.h"
#include "player.h"
#include "settings.h"
#include "fairy.h"
#include "icetrap.h"
#include "arrow.h"
#include "grotto.h"
#include "item_override.h"
#include "colors.h"
#include "common.h"

#include "actor.h"
<<<<<<< Updated upstream

ActorInit vanillaActorInit_Player = {0};

#define OBJECT_LINK_OPENING 0x19F

u16 healthDecrement = 0;

=======
#include "input.h"


ActorInit vanillaActorInit_Player = {0};
LinkExtraData gLinkExtraData = {0, ROOM_CLIENT};

void PlayerActor_rInit(Actor* thisx, GlobalContext* globalCtx) {
    vanillaActorInit_Player.init(thisx, globalCtx);
}

#include "notification.h"
void PlayerActor_rUpdate(Actor* thisx, GlobalContext* globalCtx) {    
    vanillaActorInit_Player.update(thisx, globalCtx);

    if (thisx->draw == vanillaActorInit_Player.draw) {
        thisx->draw = PlayerActor_rDraw;
    }

    if (rInputCtx.up.l) {
        Actor* actor = Actor_Spawn(&gGlobalContext->actorCtx, globalCtx, 0x10, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 0, 0, 0, 0, FALSE);
    } else if (rInputCtx.up.r) {
        Notification__Show("Debug", "ID: %d\nRole: %d\nLocation: %d", fullSyncerID, (u32)gLinkExtraData.responsability, (u32)gLinkExtraData.location);
    }
}

void PlayerActor_rDestroy(Actor* thisx, GlobalContext* globalCtx) {
    vanillaActorInit_Player.destroy(thisx, globalCtx);
}

void PlayerActor_rDraw(Actor* thisx, GlobalContext* globalCtx) {
    vanillaActorInit_Player.draw(thisx, globalCtx);
}

f32 Player_GetSpeedMultiplier(void) {
    f32 speedMultiplier = 1;
    return speedMultiplier;
}

s32 Player_IsAdult() {
    return gSaveContext.linkAge == AGE_ADULT;
}

s32 Player_ShouldApplyAdultItemsCMABs() {
    return gSaveContext.linkAge == AGE_ADULT || gSettingsContext.hoverbootsAsChild || gSettingsContext.hookshotAsChild;
}

s32 Player_ShouldUseSlingshot() {
    if (PLAYER->heldItemActionParam == 0xF) { // Slingshot
        return gSaveContext.linkAge == 1 || gSettingsContext.slingshotAsAdult;
    } else {
        return gSaveContext.linkAge == 1 && !gSettingsContext.bowAsChild;
    }
}

s32 Player_CanPickUpThisActor(Actor* interactedActor) {
    return 1;
}

#define OBJECT_LINK_OPENING 0x19F

>>>>>>> Stashed changes
void** Player_EditAndRetrieveCMB(ZARInfo* zarInfo, u32 objModelIdx) {
    void** cmbMan = ZAR_GetCMBByIndex(zarInfo, objModelIdx);
    void* cmb     = *cmbMan;

    if (gActorOverlayTable[0].initInfo->objectId == OBJECT_LINK_OPENING) {
        // Title Screen Link uses a different object, so don't apply the custom tunic patches
        // to avoid displaying a broken tunic.
        return cmbMan;
    }

    if (gSettingsContext.customTunicColors == ON) {
        if (gSaveContext.linkAge == AGE_ADULT) {
            CustomModel_EditLinkToCustomTunic(cmb);
        } else {
            CustomModel_EditChildLinkToCustomTunic(cmb);
        }
    }

    if (gSettingsContext.stickAsAdult) {
        // The unused deku stick will use the same materialIndex as the bow, to make it appear brown.
        // This also avoids issues with its combiners being repurposed by the custom tunic patches.
        ((char*)cmb)[0x4C52] = 5;
    }

    return cmbMan;
}

void* Player_GetCustomTunicCMAB(ZARInfo* originalZarInfo, u32 originalIndex) {
    if (gSettingsContext.customTunicColors == OFF || gActorOverlayTable[0].initInfo->objectId == OBJECT_LINK_OPENING) {
        return ZAR_GetCMABByIndex(originalZarInfo, originalIndex);
    }

    s16 exObjectBankIdx = Object_GetIndex(&rExtendedObjectCtx, OBJECT_CUSTOM_GENERAL_ASSETS);
    if (exObjectBankIdx < 0) {
        exObjectBankIdx = Object_Spawn(&rExtendedObjectCtx, OBJECT_CUSTOM_GENERAL_ASSETS);
    }

    if (gSaveContext.linkAge == 0) {
        return ZAR_GetCMABByIndex(&rExtendedObjectCtx.status[exObjectBankIdx].zarInfo, TEXANIM_LINK_BODY);
    } else {
        return ZAR_GetCMABByIndex(&rExtendedObjectCtx.status[exObjectBankIdx].zarInfo, TEXANIM_CHILD_LINK_BODY);
    }
}

void Player_SetChildCustomTunicCMAB(void) {
    if (gSettingsContext.customTunicColors == OFF) {
        return;
    }
    s16 exObjectBankIdx = Object_GetIndex(&rExtendedObjectCtx, OBJECT_CUSTOM_GENERAL_ASSETS);
    void* cmabMan;
    if (exObjectBankIdx < 0) {
        exObjectBankIdx = Object_Spawn(&rExtendedObjectCtx, OBJECT_CUSTOM_GENERAL_ASSETS);
    }
    cmabMan = ZAR_GetCMABByIndex(&rExtendedObjectCtx.status[exObjectBankIdx].zarInfo, TEXANIM_CHILD_LINK_BODY);
    TexAnim_Spawn(PLAYER->skelAnime.unk_28->unk_0C, cmabMan);
<<<<<<< Updated upstream
}


void PlayerActor_rInit(Actor* thisx, GlobalContext* globalCtx) {
    vanillaActorInit_Player.init(thisx, globalCtx);
}

#include "notification.h"
#include "input.h"

void PlayerActor_rUpdate(Actor* thisx, GlobalContext* globalCtx) {    
    vanillaActorInit_Player.update(thisx, globalCtx);

    if (thisx->draw == vanillaActorInit_Player.draw) {
        thisx->draw = PlayerActor_rDraw;
    }
}

void PlayerActor_rDestroy(Actor* thisx, GlobalContext* globalCtx) {
    vanillaActorInit_Player.destroy(thisx, globalCtx);
}

void PlayerActor_rDraw(Actor* thisx, GlobalContext* globalCtx) {
    vanillaActorInit_Player.draw(thisx, globalCtx);
}

f32 Player_GetSpeedMultiplier(void) {
    f32 speedMultiplier = 1;
    return speedMultiplier;
}

s32 Player_IsAdult() {
    return gSaveContext.linkAge == AGE_ADULT;
}

s32 Player_ShouldApplyAdultItemsCMABs() {
    return gSaveContext.linkAge == AGE_ADULT || gSettingsContext.hoverbootsAsChild || gSettingsContext.hookshotAsChild;
}

s32 Player_ShouldUseSlingshot() {
    if (PLAYER->heldItemActionParam == 0xF) { // Slingshot
        return gSaveContext.linkAge == 1 || gSettingsContext.slingshotAsAdult;
    } else {
        return gSaveContext.linkAge == 1 && !gSettingsContext.bowAsChild;
    }
}

s32 Player_CanPickUpThisActor(Actor* interactedActor) {
    return 1;
=======
>>>>>>> Stashed changes
}
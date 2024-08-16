#include "z3D/z3D.h"
#include "common.h"
#include "actor.h"
#include "savefile.h"
#include "enemy_souls.h"
#include "owl.h"
#include "item00.h"
#include "heart_container.h"
#include "token.h"
#include "item_etcetera.h"
#include "en_ex_item.h"
#include "demo_effect.h"
#include "business_scrubs.h"
#include "ocarina.h"
#include "cow.h"
#include "string.h"
#include "ganondorf_organ.h"
#include "checkable_spot.h"
#include "twinrova.h"
#include "nabooru.h"
#include "custom_models.h"
#include "obj_switch.h"
#include "gerudos.h"
#include "shops.h"
#include "chest.h"
#include "gossip_stone.h"
#include "drawbridge.h"
#include "king_zora.h"
#include "collapsing_castle.h"
#include "demo_kankyo.h"
#include "lake_hylia_objects.h"
#include "graveyard_objects.h"
#include "windmill_man.h"
#include "deku_tree_mouth.h"
#include "well_stone.h"
#include "well_water.h"
#include "liftable_rock.h"
#include "player.h"
#include "rupee_trap.h"
#include "item_override.h"
#include "songs_visual_effects.h"
#include "shooting_gallery_man.h"
#include "gtg_gate.h"
#include "chest_minigame.h"
#include "door.h"
#include "malon.h"
#include "jabu.h"
#include "dampe.h"
#include "hookshot.h"
#include "web.h"
#include "boulder_red.h"
#include "skulltula.h"
#include "collapsing_platform.h"
#include "carpenter.h"
#include "pushblock.h"
#include "spin_attack.h"
#include "deku_scrubs.h"
#include "bean_plant.h"
#include "sheik.h"
#include "skulltula_people.h"
#include "red_ice.h"
#include "shabom.h"
#include "anubis.h"
#include "link_puppet.h"
#include "fishing.h"

#define OBJECT_GI_KEY 170
#define OBJECT_GI_BOSSKEY 185
#define OBJECT_GI_HEARTS 189
#define OBJECT_GI_OCARINA 222
#define OBJECT_GI_OCARINA_0 270
#define OBJECT_GI_SHOP_FAIRY 375
#define OBJECT_GI_SOLD_OUT 328
#define OBJECT_TRIFORCE 149

typedef void (*TitleCard_Update_proc)(GlobalContext* globalCtx, TitleCardContext* titleCtx);
#define TitleCard_Update ((TitleCard_Update_proc)GAME_ADDR(0x47953C))

void Actor_Init() {
    memcpy(&vanillaActorInit_Player, gActorOverlayTable[0x0].initInfo, sizeof(ActorInit));
    
    gActorOverlayTable[0x0].initInfo->init    = PlayerActor_rInit;
    gActorOverlayTable[0x0].initInfo->update  = PlayerActor_rUpdate;
    gActorOverlayTable[0x0].initInfo->destroy = PlayerActor_rDestroy;
    gActorOverlayTable[0x0].initInfo->draw    = PlayerActor_rDraw;

    gActorOverlayTable[0x1].initInfo = &EnLinkPuppet_InitVars;

    gActorOverlayTable[0x4].initInfo->init         = ShopsanityItem_Init;
    gActorOverlayTable[0x4].initInfo->instanceSize = sizeof(ShopsanityItem);

    gActorOverlayTable[0x9].initInfo->update = (ActorFunc)EnDoor_rUpdate;

    gActorOverlayTable[0xA].initInfo->init   = EnBox_rInit;
    gActorOverlayTable[0xA].initInfo->update = EnBox_rUpdate;

    gActorOverlayTable[0xF].initInfo->update = (ActorFunc)BgYdanSp_rUpdate;

    gActorOverlayTable[0x15].initInfo->init    = EnItem00_rInit;
    gActorOverlayTable[0x15].initInfo->destroy = EnItem00_rDestroy;
    gActorOverlayTable[0x15].initInfo->update  = EnItem00_rUpdate;
    gActorOverlayTable[0x15].initInfo->draw    = EnItem00_rDraw;

    // gActorOverlayTable[0x2D].initInfo->update = EnBubble_rUpdate;

    gActorOverlayTable[0x2E].initInfo->init   = DoorShutter_rInit;
    gActorOverlayTable[0x2E].initInfo->update = (ActorFunc)DoorShutter_rUpdate;

    gActorOverlayTable[0x3D].initInfo->destroy = EnOssan_rDestroy;

    gActorOverlayTable[0x3E].initInfo->init = BgTreemouth_rInit;

    gActorOverlayTable[0x4A].initInfo->update = BgSpot00Hanebasi_rUpdate;

    gActorOverlayTable[0x57].initInfo->init = EnMThunder_rInit;

    gActorOverlayTable[0x5F].initInfo->init    = ItemBHeart_rInit;
    gActorOverlayTable[0x5F].initInfo->destroy = ItemBHeart_rDestroy;
    gActorOverlayTable[0x5F].initInfo->update  = ItemBHeart_rUpdate;
    gActorOverlayTable[0x5F].initInfo->draw    = ItemBHeart_rDraw;

    gActorOverlayTable[0x66].initInfo->init = ArmsHook_rInit;

    gActorOverlayTable[0x85].initInfo->update = EnTk_rUpdate;

    gActorOverlayTable[0x8B].initInfo->init    = DemoEffect_rInit;
    gActorOverlayTable[0x8B].initInfo->destroy = DemoEffect_rDestroy;
    gActorOverlayTable[0x8B].initInfo->update  = DemoEffect_rUpdate;

    gActorOverlayTable[0x8C].initInfo->update = DemoKankyo_rUpdate;

    gActorOverlayTable[0x95].initInfo->init   = EnSw_rInit;
    gActorOverlayTable[0x95].initInfo->update = EnSw_rUpdate;

    gActorOverlayTable[0x9C].initInfo->update = BgSpot02Objects_rUpdate;

    gActorOverlayTable[0xC1].initInfo->init   = EnSyatekiMan_rInit;
    gActorOverlayTable[0xC1].initInfo->update = EnSyatekiMan_rUpdate;

    gActorOverlayTable[0xC3].initInfo->draw = EnNb_rDraw;

    gActorOverlayTable[0xD5].initInfo->update  = BgSpot06Objects_rUpdate;
    gActorOverlayTable[0xD5].initInfo->destroy = BgSpot06Objects_rDestroy;

    gActorOverlayTable[0xDC].initInfo->init    = Boss_Tw_rInit;
    gActorOverlayTable[0xDC].initInfo->update  = Boss_Tw_rUpdate;
    gActorOverlayTable[0xDC].initInfo->draw    = Boss_Tw_rDraw;
    gActorOverlayTable[0xDC].initInfo->destroy = Boss_Tw_rDestroy;

    gActorOverlayTable[0xE0].initInfo->update = EnAnubice_rUpdate;

    gActorOverlayTable[0xE6].initInfo->init = BgBdanSwitch_rInit;

    gActorOverlayTable[0xE7].initInfo->init = EnMa1_rInit;

    gActorOverlayTable[0xF1].initInfo->init    = ItemOcarina_rInit;
    gActorOverlayTable[0xF1].initInfo->destroy = ItemOcarina_rDestroy;
    gActorOverlayTable[0xF1].initInfo->draw    = ItemOcarina_rDraw;

    gActorOverlayTable[0xFE].initInfo->init   = Fishing_rInit;
    gActorOverlayTable[0xFE].initInfo->update = Fishing_rUpdateFish;
    gActorOverlayTable[0xFE].initInfo->draw   = Fishing_rDrawFish;

    gActorOverlayTable[0xFF].initInfo->update = ObjOshihiki_rUpdate;

    gActorOverlayTable[0x104].initInfo->init = BgSpot01Idomizu_rInit;

    gActorOverlayTable[0x107].initInfo->update = BgSpot15Rrbox_rUpdate;

    gActorOverlayTable[0x10F].initInfo->init    = ItemEtcetera_rInit;
    gActorOverlayTable[0x10F].initInfo->destroy = ItemEtcetera_rDestroy;
    gActorOverlayTable[0x10F].initInfo->update  = ItemEtcetera_rUpdate;

    gActorOverlayTable[0x11A].initInfo->update = EnDns_rUpdate;

    gActorOverlayTable[0x11B].initInfo->update = NULL;

    gActorOverlayTable[0x126].initInfo->init   = (ActorFunc)ObjBean_rInit;
    gActorOverlayTable[0x126].initInfo->update = (ActorFunc)ObjBean_rUpdate;

    gActorOverlayTable[0x12A].initInfo->init = ObjSwitch_rInit;

    gActorOverlayTable[0x12C].initInfo->update = (ActorFunc)ObjLift_rUpdate;

    gActorOverlayTable[0x131].initInfo->update = EnExRuppy_rUpdate;

    gActorOverlayTable[0x133].initInfo->update = (ActorFunc)EnDaiku_rUpdate;

    gActorOverlayTable[0x138].initInfo->init   = EnGe1_rInit;
    gActorOverlayTable[0x138].initInfo->update = EnGe1_rUpdate;

    gActorOverlayTable[0x145].initInfo->init = BgSpot01Idosoko_rInit;

    gActorOverlayTable[0x147].initInfo->update = EnWonderTalk_rUpdate;

    gActorOverlayTable[0x14D].initInfo->init   = EnOwl_DespawnInit; // Despawns unnecessary owls
    gActorOverlayTable[0x14D].initInfo->update = EnOwl_rUpdate;

    gActorOverlayTable[0x14E].initInfo->init   = EnIshi_rInit;
    gActorOverlayTable[0x14E].initInfo->update = (ActorFunc)EnIshi_rUpdate;

    gActorOverlayTable[0x153].initInfo->update = EnFu_rUpdate;

    gActorOverlayTable[0x155].initInfo->init = EnChanger_rInit;

    gActorOverlayTable[0x15E].initInfo->init = EnGanonOrgan_rInit;

    gActorOverlayTable[0x164].initInfo->update = EnKz_rUpdate;

    gActorOverlayTable[0x168].initInfo->init    = EnExItem_rInit;
    gActorOverlayTable[0x168].initInfo->destroy = EnExItem_rDestroy;

    gActorOverlayTable[0x172].initInfo->update = (ActorFunc)DoorGerudo_rUpdate;

    gActorOverlayTable[0x174].initInfo->update = DemoGt_rUpdate;

    gActorOverlayTable[0x17C].initInfo->init = EnTakaraMan_rInit;

    gActorOverlayTable[0x17E].initInfo->update = OceffSpot_rUpdate;

    gActorOverlayTable[0x185].initInfo->update = EnWonderTalk2_rUpdate;

    gActorOverlayTable[0x188].initInfo->update = EnSsh_rUpdate;

    gActorOverlayTable[0x18A].initInfo->update = OceffWipe_rUpdate;
    gActorOverlayTable[0x18B].initInfo->update = OceffStorm_rUpdate;

    gActorOverlayTable[0x191].initInfo->init   = BgSpot12Saku_rInit;
    gActorOverlayTable[0x191].initInfo->update = BgSpot12Saku_rUpdate;

    gActorOverlayTable[0x195].initInfo->init = EnShopnuts_rInit;

    gActorOverlayTable[0x198].initInfo->update = OceffWipe2_rUpdate;
    gActorOverlayTable[0x199].initInfo->update = OceffWipe3_rUpdate;

    gActorOverlayTable[0x19C].initInfo->init    = EnSi_rInit;
    gActorOverlayTable[0x19C].initInfo->destroy = EnSi_rDestroy;
    gActorOverlayTable[0x19C].initInfo->update  = EnSi_rUpdate;
    gActorOverlayTable[0x19C].initInfo->draw    = EnSi_rDraw;

    gActorOverlayTable[0x1A3].initInfo->update = EnDntNomal_rUpdate;

    gActorOverlayTable[0x1B9].initInfo->init = EnGs_rInit;

    gActorOverlayTable[0x1CB].initInfo->update = OceffWipe4_rUpdate;

    gActorOverlayTable[0x1C6].initInfo->init    = EnCow_rInit;
    gActorOverlayTable[0x1C6].initInfo->destroy = EnCow_rDestroy;

    gActorOverlayTable[0x1D2].initInfo->update = (ActorFunc)ObjHamishi_rUpdate;

    // Define object 4 to be by default the same as object 189
    strncpy(gObjectTable[OBJECT_CUSTOM_DOUBLE_DEFENSE].filename, gObjectTable[OBJECT_GI_HEARTS].filename, 0x40);

    // Define object 5 to be by default the same as object 270
    strncpy(gObjectTable[OBJECT_CUSTOM_CHILD_SONGS].filename, gObjectTable[OBJECT_GI_OCARINA_0].filename, 0x40);

    // Define object 16 to be by default the same as object 222
    strncpy(gObjectTable[OBJECT_CUSTOM_ADULT_SONGS].filename, gObjectTable[OBJECT_GI_OCARINA].filename, 0x40);

    // Define all the small key objects to be by default the same as object 170
    strncpy(gObjectTable[OBJECT_CUSTOM_SMALL_KEY_FOREST].filename, gObjectTable[OBJECT_GI_KEY].filename, 0x40);
    strncpy(gObjectTable[OBJECT_CUSTOM_SMALL_KEY_FIRE].filename, gObjectTable[OBJECT_GI_KEY].filename, 0x40);
    strncpy(gObjectTable[OBJECT_CUSTOM_SMALL_KEY_WATER].filename, gObjectTable[OBJECT_GI_KEY].filename, 0x40);
    strncpy(gObjectTable[OBJECT_CUSTOM_SMALL_KEY_SHADOW].filename, gObjectTable[OBJECT_GI_KEY].filename, 0x40);
    strncpy(gObjectTable[OBJECT_CUSTOM_SMALL_KEY_BOTW].filename, gObjectTable[OBJECT_GI_KEY].filename, 0x40);
    strncpy(gObjectTable[OBJECT_CUSTOM_SMALL_KEY_SPIRIT].filename, gObjectTable[OBJECT_GI_KEY].filename, 0x40);
    strncpy(gObjectTable[OBJECT_CUSTOM_SMALL_KEY_FORTRESS].filename, gObjectTable[OBJECT_GI_KEY].filename, 0x40);
    strncpy(gObjectTable[OBJECT_CUSTOM_SMALL_KEY_GTG].filename, gObjectTable[OBJECT_GI_KEY].filename, 0x40);
    strncpy(gObjectTable[OBJECT_CUSTOM_SMALL_KEY_GANON].filename, gObjectTable[OBJECT_GI_KEY].filename, 0x40);

    // Define object 128 to be by default the same as object 185
    strncpy(gObjectTable[OBJECT_CUSTOM_BOSS_KEYS].filename, gObjectTable[OBJECT_GI_BOSSKEY].filename, 0x40);

    // Define object 228 to be by default the same as object 375
    strncpy(gObjectTable[OBJECT_CUSTOM_ENEMY_SOUL].filename, gObjectTable[OBJECT_GI_SHOP_FAIRY].filename, 0x40);

    // Define object 291 to be by default the same as object 328
    strncpy(gObjectTable[OBJECT_CUSTOM_OCARINA_BUTTON].filename, gObjectTable[OBJECT_GI_SOLD_OUT].filename, 0x40);

    // Define object 366 to be by default the same as object 149
    strncpy(gObjectTable[OBJECT_CUSTOM_TRIFORCE_PIECE].filename, gObjectTable[OBJECT_TRIFORCE].filename, 0x40);
}

void ActorSetup_Extra() {
    Sheik_Spawn();
    GsQueue_Update();
}

void TitleCard_rUpdate(GlobalContext* globalCtx, TitleCardContext* titleCtx) {
    if (ItemOverride_IsAPendingOverride()) {
        titleCtx->delayTimer    = 0;
        titleCtx->durationTimer = 0;
        titleCtx->alpha         = 0;
        titleCtx->intensity     = 0;
    }

    TitleCard_Update(globalCtx, titleCtx);
}

u8 ActorSetup_ShouldSkipEntry(ActorEntry* actorEntry) {
    // Alternate Gold Skulltula Locations
    if (actorEntry->id == 0x95 && (actorEntry->params & 0xE000) && Gs_HasAltLoc(actorEntry, GS_PPT_ACTORENTRY, TRUE)) {
        return TRUE;
    }

    return FALSE;
}

void ActorSetup_After(void) {
    Gs_QueueAlternateLocated();
}


s32 Actor_IsBoss(Actor* actor) {
    return (actor->id == 0x28) ||                                           // Gohma
           (actor->id == 0x27 || actor->id == 0x30) ||                      // King Dodongo + Fire Breath
           (actor->id == 0xBA && actor->params == -1) ||                    // Barinade
           (actor->id == 0x52 || actor->id == 0x67 || actor->id == 0x6D) || // Phantom Ganon + Horse + Lightning
           (actor->id == 0x96 || actor->id == 0xA2 || actor->id == 0xAD) || // Volvagia + Rock Attack
           (actor->id == 0xC4) ||                                           // Morpha
           (actor->id == 0xE9 && actor->params == -1) ||                    // Bongo Bongo
           (actor->id == 0xDC) ||                                           // Twinrova
           (actor->id == 0xE8) ||                                           // Ganondorf
           (actor->id == 0x17A);                                            // Ganon
}

void Actor_rUpdate(Actor* actor, GlobalContext* globalCtx) {
    actor->update(actor, globalCtx);
}

#include "multiplayer.h"
#include "notification.h"

ActorType getActorType(u16 actorId) {
    return (ActorType) gActorOverlayTable[actorId].initInfo->type;
}

extern Actor* ActorSpawnOrig(ActorContext* actorCtx, GlobalContext* globalCtx, s16 actorId, float posX, float posY, float posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params, s32 initImmediately) __attribute__((pcs("aapcs-vfp")));

__attribute__((pcs("aapcs-vfp")))
Actor* Actor_rSpawn(ActorContext* actorCtx, GlobalContext* globalCtx, s16 actorId, float posX, float posY, float posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params, s32 initImmediately) {
    if (!IsInGame() || actorId == 0x0 || actorId == 0x1) {
        return ActorSpawnOrig(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params, initImmediately);
    }

    if (gLinkExtraData.responsability == ROOM_OWNER) {
        PosRot p;
        p.pos.x = posX;
        p.pos.y = posY;
        p.pos.z = posZ;
        p.rot.x = rotX;
        p.rot.y = rotY;
        p.rot.z = rotZ;

        Multiplayer_Send_ActorSpawn(actorId, p, params);

        // Ajouter les informations de l'acteur à gLinkExtraData
        if (gLinkExtraData.numActorsInRoom < MAX_ACTORS_IN_ROOM) {
            ActorInfo* actorInfo = &gLinkExtraData.actorInfos[gLinkExtraData.numActorsInRoom];
            actorInfo->actorId = actorId;
            actorInfo->posX = posX;
            actorInfo->posY = posY;
            actorInfo->posZ = posZ;
            actorInfo->rotX = rotX;
            actorInfo->rotY = rotY;
            actorInfo->rotZ = rotZ;
            actorInfo->params = params;
            
            gLinkExtraData.numActorsInRoom++;
        }

        return ActorSpawnOrig(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params, initImmediately);
    }

    ActorType actorType = getActorType(actorId);
    if ((actorType == ACTORTYPE_NPC || actorType == ACTORTYPE_ENEMY)) {
        return ActorSpawnOrig(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params, initImmediately);
    }

    if (ableToSpawnActors) {
        return ActorSpawnOrig(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params, initImmediately);
    }

    return NULL;
}


void Actor_rDraw(Actor* actor, GlobalContext* globalCtx) {
    actor->draw(actor, globalCtx);
}

s32 Actor_CollisionATvsAC(Collider* at, Collider* ac) {
    RedIce_CheckIceArrow(at, ac);

    if (ac->actor != 0 && !EnemySouls_CheckSoulForActor(ac->actor)) {
        return 0; // ignore this collision
    }

    return 1; // continue as normal
}
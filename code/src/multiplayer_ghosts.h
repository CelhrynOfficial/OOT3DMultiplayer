#ifndef _MULTIPLAYER_GHOSTS_H_
#define _MULTIPLAYER_GHOSTS_H_

#include "3ds/types.h"
#include "z3D/z3D.h"
#include "colors.h"
#include "multiplayer.h"

#define LINK_LIMB_COUNT 25

// Filler struct to match allocated size in SkelAnime_InitLink
typedef struct {
    u8 data[0x34];
} LimbData;

typedef struct {
    s32 currentScene;
    s32 age;
    
    Vec3f position;
    Vec3s rotation;
    
    u32 meshGroups1;
    u32 meshGroups2;

    s32 currentTunic;
    
    Color_RGBAf childTunicColor;
    Color_RGBAf kokiriTunicColor;
    Color_RGBAf goronTunicColor;
    Color_RGBAf zoraTunicColor;

    u16 puppetID;

    LimbData jointTable[LINK_LIMB_COUNT]; // ALWAYS KEEP LAST
} GhostData;

typedef struct {
    bool inUse;
    u64 lastTick;
    u16 networkID;
    u8 isInGame;
    GhostData ghostData;
    LinkExtraData extraData;
    u8 otherPlayerIndex;
} LinkGhost;

static LinkGhost ghosts[16];

void Multiplayer_Ghosts_Tick(void);
void Multiplayer_Ghosts_UpdateGhostData(u16 networkID, GhostData* ghostData, u8 isInGame);
void Multiplayer_Ghosts_UpdateGhostData_JointTable(u16 networkID, LimbData* limbData);
LinkGhost* Multiplayer_Ghosts_GetGhostByIndex(size_t index);
LinkGhost* Multiplayer_Ghosts_GetGhost(u16 networkID);
GhostData* Multiplayer_Ghosts_GetGhostData(u16 networkID);
void Multiplayer_Ghosts_SpawnPuppets(void);

#endif //_MULTIPLAYER_GHOSTS_H_

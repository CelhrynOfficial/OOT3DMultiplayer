#ifndef _SONGS_VISUAL_EFFECTS_
#define _SONGS_VISUAL_EFFECTS_

#include "z3DDebug/z3D.h"

extern ActorInit vanillaActorInit_OceffSpot;
extern ActorInit vanillaActorInit_OceffWipe;
extern ActorInit vanillaActorInit_OceffStorm;
extern ActorInit vanillaActorInit_OceffWipe2;
extern ActorInit vanillaActorInit_OceffWipe3;
extern ActorInit vanillaActorInit_OceffWipe4;
void OceffSpot_rUpdate(Actor* thisx, GlobalContext* globalCtx);
void OceffWipe_rUpdate(Actor* thisx, GlobalContext* globalCtx);
void OceffStorm_rUpdate(Actor* thisx, GlobalContext* globalCtx);
void OceffWipe2_rUpdate(Actor* thisx, GlobalContext* globalCtx);
void OceffWipe3_rUpdate(Actor* thisx, GlobalContext* globalCtx);
void OceffWipe4_rUpdate(Actor* thisx, GlobalContext* globalCtx);

#endif //_SONGS_VISUAL_EFFECTS_

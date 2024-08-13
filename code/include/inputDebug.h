#include "z3DDebug/z3D.h"
#include "hidDebug.h"
#include "3dsDebug/services/irrst.h"

#define ADDITIONAL_FLAG_BUTTON rInputCtx.cur.r

typedef struct {
    btn_t cur;
    btn_t up;
    btn_t pressed;
    btn_t old;
    circlePosition cStick;
} InputContext;

void Input_Update(void);
u32 Input_WaitWithTimeout(u32 msec);
u32 Input_Wait(void);

extern InputContext rInputCtx;
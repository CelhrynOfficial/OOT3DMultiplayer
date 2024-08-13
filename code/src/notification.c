#include "notification.h"

#include "gfx.h"
#include "draw.h"

#include "3ds/os.h"

#include "lib/string.h"
#include "lib/printf.h"

#define NOTIF_WIDTH 150
#define NOTIF_HEIGHT 70
#define NOTIF_ANIM_SPEED 25
#define NOTIF_DISPLAY_FRAMES 60

extern u32 rGameplayFrames;

typedef enum {
    ENTER,
    DISPLAY,
    EXIT
} NotificationState;

typedef struct {
    NotificationState state;
    u32 x;
    u32 y;
    u64 startTime;
    u64 displayStartTime;
    char emitter[64];
    char message[128];
    u8 isActive;
} Notification;

static Notification NotificationInstance = { 0, SCREEN_TOP_WIDTH, 10, 0, 0, "N/A", "N/A" };

void Notification__Init() {
    NotificationInstance.state            = ENTER;
    NotificationInstance.x                = SCREEN_TOP_WIDTH;
    NotificationInstance.y                = 10;
    NotificationInstance.startTime        = 0;
    NotificationInstance.displayStartTime = 0;

    memset(NotificationInstance.emitter, 0, sizeof(NotificationInstance.emitter));
    memset(NotificationInstance.message, 0, sizeof(NotificationInstance.message));

    NotificationInstance.isActive = 0;
}

void Notification__Draw() {
    if (!NotificationInstance.isActive) {
        return;
    }

    Draw_DrawRectTop(NotificationInstance.x, NotificationInstance.y, NOTIF_WIDTH, NOTIF_HEIGHT, COLOR_BLACK);
    Draw_DrawStringTop(NotificationInstance.x + 10, NotificationInstance.y + 10, COLOR_BLUE, NotificationInstance.emitter);
    Draw_DrawStringTop(NotificationInstance.x + 10, NotificationInstance.y + 30, COLOR_WHITE, NotificationInstance.message);
}

void Notification__Update() {
    u64 currentTime = rGameplayFrames;

    if (!NotificationInstance.isActive) {
        return;
    }

    switch (NotificationInstance.state) {
        case ENTER:
            if (NotificationInstance.x > SCREEN_TOP_WIDTH - NOTIF_WIDTH) {
                NotificationInstance.x -= NOTIF_ANIM_SPEED;
            } else {
                NotificationInstance.x                = SCREEN_TOP_WIDTH - NOTIF_WIDTH;
                NotificationInstance.state            = DISPLAY;
                NotificationInstance.displayStartTime = currentTime;
            }
            break;

        case DISPLAY:
            if ((currentTime - NotificationInstance.displayStartTime) >= NOTIF_DISPLAY_FRAMES) {
                NotificationInstance.state = EXIT;
            }
            break;

        case EXIT:
            if (NotificationInstance.x < SCREEN_TOP_WIDTH - 40) {
                NotificationInstance.x += NOTIF_ANIM_SPEED;
            } else {
                NotificationInstance.x        = SCREEN_TOP_WIDTH;
                NotificationInstance.isActive = 0;
            }
            
            break;
    }
}

void Notification__Show(const char* emitter, const char* message, ...) {
    char buf[128];
    va_list args;

    va_start(args, message);
    vsnprintf(buf, 128, message, args);
    va_end(args);

    memset(NotificationInstance.emitter, 0, sizeof(NotificationInstance.emitter));
    memset(NotificationInstance.message, 0, sizeof(NotificationInstance.message));

    strncpy(NotificationInstance.emitter, emitter, sizeof(NotificationInstance.emitter) - 1);
    strncpy(NotificationInstance.message, buf, sizeof(buf) - 1);

    NotificationInstance.startTime        = rGameplayFrames;
    NotificationInstance.displayStartTime = rGameplayFrames;
    NotificationInstance.state            = ENTER;
    NotificationInstance.x                = SCREEN_TOP_WIDTH;
    NotificationInstance.isActive         = 1;

    // PlaySound(0x100035C);
}
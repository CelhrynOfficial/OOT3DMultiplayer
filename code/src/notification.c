#include "notification.h"

#include "gfx.h"
#include "draw.h"

#include "3ds/os.h"

#include "lib/string.h"
#include "lib/printf.h"

#define NOTIF_WIDTH 150
#define NOTIF_HEIGHT 70
#define NOTIF_ANIM_SPEED 25
#define NOTIF_DISPLAY_TIME 5

extern u32 rGameplayFrames;

typedef struct {
    u8 isDisplaying;
    u32 x;
    u32 y;
    u64 startTime;
    char emitter[64];
    char message[128];
} Notification;

static Notification NotificationInstance = {0, SCREEN_TOP_WIDTH, 10, 0, "N/A", "N/A"};

void Notification__Init() {
    NotificationInstance.isDisplaying = 0;
}

void Notification__Draw() {
    if (!NotificationInstance.isDisplaying) {
        return;
    }

    // Draw_ClearBackbuffer();

    Draw_DrawRectTop(NotificationInstance.x, NotificationInstance.y, NOTIF_WIDTH, NOTIF_HEIGHT, COLOR_BLACK);

    Draw_DrawStringTop(NotificationInstance.x + 10, NotificationInstance.y + 10, COLOR_BLUE, NotificationInstance.emitter);
    Draw_DrawStringTop(NotificationInstance.x + 10, NotificationInstance.y + 30, COLOR_WHITE, NotificationInstance.message);

    // Draw_CopyBackBuffer();
}

void Notification__Update() {
    if (!NotificationInstance.isDisplaying) {
        return;
    }

    u64 currentTime = rGameplayFrames;
    double elapsedTime = (double) (currentTime - NotificationInstance.startTime) / 60;

    if (NotificationInstance.x > SCREEN_TOP_WIDTH - NOTIF_WIDTH) {
        NotificationInstance.x -= NOTIF_ANIM_SPEED;
    }
    else if (elapsedTime < NOTIF_DISPLAY_TIME) {
    }
    else if (NotificationInstance.x < SCREEN_TOP_WIDTH) {
        NotificationInstance.x += NOTIF_ANIM_SPEED;
    }
    else {
        NotificationInstance.isDisplaying = 0;
    }
}

void Notification__Show(const char* emitter, const char* message) {
    memset(NotificationInstance.emitter, 0, 64);
    memset(NotificationInstance.message, 0, 128);

    strncpy(NotificationInstance.emitter, emitter, sizeof(NotificationInstance.emitter) - 1);
    strncpy(NotificationInstance.message, message, sizeof(NotificationInstance.message) - 1);
    
    NotificationInstance.startTime = rGameplayFrames;
    NotificationInstance.isDisplaying = 1;
    NotificationInstance.x = SCREEN_TOP_WIDTH;
}
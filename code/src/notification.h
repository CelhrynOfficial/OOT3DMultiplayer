#ifndef _NOTIFICATION_H_
#define _NOTIFICATION_H_

void Notification__Init();
void Notification__Draw();
void Notification__Update();
void Notification__Show(const char* emitter, const char* message);

#endif //_NOTIFICATION_H_
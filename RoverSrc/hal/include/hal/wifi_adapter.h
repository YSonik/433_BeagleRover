#ifndef __WIFI_ADAPTER_H
#define __WIFI_ADAPTER_H

void WifiAdapter_init(void);
void WifiAdapter_cleanUp(void);

void WifiAdapter_connect(const char* ssid, const char* password);
void WifiAdapter_disconnect(void);

void WifiAdapter_send(const char* message);
void WifiAdapter_receive(char* buffer, int bufferLength);

#endif // __WIFI_ADAPTER_H
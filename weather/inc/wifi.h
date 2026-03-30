#ifndef WiFI_H
#define WiFI_H

#define MAX_SSID_LEN 32
#define MAX_PASSWORD_LEN 63
#define WIFI_SCAN_MAX_TIMEOUT_MS 5000
#define WIFI_CONNECT_MAX_TIMEOUT_MS 3000

int WiFiMenuCreate(char *pWifiMenuStr, const int numWifiMenuStrSize);

int WiFiConnect(const char *pPassword, const char *pSsid, const TEM_WIFI_SEC_METHOD secMode);

const TST_WIFI_METADATA *WiFiGetMetaList(void);

int WiFiMenuFlow(void);

void DisPlayMessage(uint uiX, uint uiY, const char *pDisplayMessage, ...);

#endif

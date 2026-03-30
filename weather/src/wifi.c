#include "tfx_sysinfo.h"
#include "tfx_scr.h"
#include "tfx_ui.h"
#include "tfx_keyboard.h"
#include "tfx_debug.h"
#include "tfx_clock.h"
#include "tfx_power.h"
#include "tfx_wireless.h"
#include "tfx_wifi.h"
#include "tfx_net.h"

#include "main.h"
#include "posdef.h"
#include "debug.h"
#include "vkeyboard.h"
#include "wifi.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <execinfo.h>
#include <sys/types.h>
#include <gnu/libc-version.h>
#include <sys/utsname.h>
#include <stdarg.h>

static TST_WIFI_METADATA stMetaList[MAX_WIFINETS + 1];
/*
 * 扫描并生成 Wi-Fi 菜单字符串。
 */
int WiFiMenuCreate(char *pszWifiMenuStr, const int numWifiMenuStrSize)
{
    int iRet = 0;
    char szNum[8];
    int i = 0;
    int numWifiNets = 0;
    int numN = 0;
    struct timeval stStartTime, stCurrentTime;
    int iTimeOut = 0;
    int numUsed = 0;

    if (pszWifiMenuStr == NULL || numWifiMenuStrSize <= 0)
    {
        return APP_ERRBUFLEN;
    }

    memset(stMetaList, 0, sizeof(stMetaList));

    if (TfxIsWiFiUp() == FALSE)
    {
        iRet = TfxWiFiOpen();
        if (iRet != 0)
        {
            return iRet;
        }
        gettimeofday(&stStartTime, NULL);
        while (1)
        {
            if (TfxIsWiFiUp() == true)
            {
                break;
            }

            gettimeofday(&stCurrentTime, NULL);
            iTimeOut = (stCurrentTime.tv_sec - stStartTime.tv_sec) * 1000;

            if (iTimeOut > WIFI_CONNECT_MAX_TIMEOUT_MS)
            {
                TfxDelay(1);
                return TFX_ERR_TIMEOUT;
            }
        }
    }
    DebugPrintf("WiFi Opened\n");

    gettimeofday(&stStartTime, NULL);

    do
    {
        gettimeofday(&stCurrentTime, NULL);
        iTimeOut = (stCurrentTime.tv_sec - stStartTime.tv_sec) * 1000;

        if (iTimeOut > WIFI_SCAN_MAX_TIMEOUT_MS)
        {
            TfxDelay(1);
            return TFX_ERR_TIMEOUT;
        }

        iRet = TfxWiFiScan();

        TfxDelay(1);

        iRet = TfxGetWiFiNetListInMeta(stMetaList, MAX_WIFINETS, &numWifiNets);

    } while (numWifiNets <= 1 || iRet != 0);

    DebugPrintf("WiFi GetNetListed %d\n", numWifiNets);
#if 0
    iRet = TfxWiFiClose();
    if (iRet != 0)
    {
    	AppFree();
    	return APP_SUCC;
    }
    DebugPrintf("WiFi Closed\n");
#endif

    for (i = 0; i < numWifiNets; i++)
    {
        DebugPrintf("SSID %d: %s, Signal: %d dBm, Freq: %d MHz, SecMode: %d\n", i + 1,
                    stMetaList[i].sSsid,
                    stMetaList[i].iSignalStrength,
                    stMetaList[i].iFrequency,
                    stMetaList[i].temSecMode);

        numUsed = strnlen(pszWifiMenuStr, numWifiMenuStrSize);
        if (numUsed >= numWifiMenuStrSize)
        {
            return APP_ERRBUFLEN;
        }

        numN = 0;
        if (i != 0)
        {
            numN = snprintf(pszWifiMenuStr + numUsed, numWifiMenuStrSize - numUsed - 1, "|");
            if (numN < 0 || numN >= (numWifiMenuStrSize - numUsed - 1))
            {
                return APP_ERRBUFLEN;
            }
            numUsed += numN;
        }

        numN = 0;
        snprintf(szNum, sizeof(szNum), "%d", i + 1);
        numN = snprintf(pszWifiMenuStr + numUsed, numWifiMenuStrSize - numUsed - 1, "%s.%s", szNum, stMetaList[i].sSsid);
        if (numN < 0 || numN >= (numWifiMenuStrSize - numUsed - 1))
        {
            return APP_ERRBUFLEN;
        }
        numUsed += numN;
    }

    return APP_SUCC;
}

/*
 * 连接指定 SSID 的 Wi-Fi。
 * 返回：APP_SUCC 成功，否则为错误码。
 */
int WiFiConnect(const char *pPassword, const char *pSsid, const TEM_WIFI_SEC_METHOD secMode)
{
    int iRet = 0;
    int iTimeOut = 0;
    TST_WIFI_PARAM stWifiParam;
    struct timeval stStartTime, stCurrentTime;
    size_t numPasswordLen = 0;

    memset(&stWifiParam, 0, sizeof(TST_WIFI_PARAM));

    if (pSsid == NULL)
    {
        return APP_ERRBUFLEN;
    }

    if (secMode == WIFI_SEC_OPEN) // no password
    {
        iRet = TfxWiFiConnect(pSsid, "", secMode);
    }
    else if (pPassword != NULL)
    {
        numPasswordLen = strnlen(pPassword, MAX_PASSWORD_LEN + 1);
        if (numPasswordLen == 0 || numPasswordLen > MAX_PASSWORD_LEN)
        {
            return APP_ERRBUFLEN;
        }
        iRet = TfxWiFiConnect(pSsid, pPassword, secMode);
    }
    else
    {
        return APP_ERRBUFLEN;
    }
    gettimeofday(&stStartTime, NULL);

    while (1)
    {
        if (TfxIsWiFiConnected() == true)
        {
            break;
        }

        gettimeofday(&stCurrentTime, NULL);
        iTimeOut = (stCurrentTime.tv_sec - stStartTime.tv_sec) * 1000;

        if (iTimeOut > WIFI_CONNECT_MAX_TIMEOUT_MS)
        {
            // timeout
            DisPlayMessage(0, 200, "WiFi Connect Timeout!");
            TfxDelay(2);
            return APP_TIMEOUT;
        }
    }

    TfxEnableDhcp(NET_TYPE_WIFI);
    TfxDelay(2);
    // 1. 分配缓冲区（IPv4 地址最大长度 15，+1 留空字符）
    char ip[16] = {0};
    char subnet_mask[16] = {0};
    char gateway[16] = {0};

    // 2. 调用函数获取 WiFi 配置
    int ret = TfxNetGetContext(
        NET_TYPE_WIFI,        // 要获取的网络类型：WiFi
        ip, sizeof(ip),      // 接收 IP 地址
        subnet_mask, sizeof(subnet_mask), // 接收子网掩码
        gateway, sizeof(gateway)          // 接收网关
    );

    // 3. 处理结果
    if (ret != 0 || ip==NULL) {
        TfxScrDisplayStrInline(14, ALIGN_CENTER, "获取网络配置失败！");
        return 9;
    }

    // 4. 显示配置信息（调试/诊断用）
    DisPlayMessage(0,180,"IP: %s 网关: %s", ip, gateway);

    TfxWiFiGetParam(&stWifiParam);
    DebugPrintf("WiFi Connect ret: %d, SSID: %s, SecMode: %d\n", iRet, stWifiParam.sSsid, stWifiParam.temSecMode);
    // connected
    DisPlayMessage(0, 200, "WiFi Connected to SSID: %s", stWifiParam.sSsid);
    TfxDelay(2);
    return APP_SUCC;
}

/*
 * 获取最近一次扫描的 Wi-Fi 元数据列表。
 */
const TST_WIFI_METADATA *WiFiGetMetaList(void)
{
    return stMetaList;
}

/*
 * Wi-Fi 菜单流程：扫描、选择并输入密码连接。
 */
int WiFiMenuFlow(void)
{
    int iRet = 0;
    int iSelect = 0x01;

    char szWifiMenuStr[512];

    char szWiFiPassword[64];
    int numWiFiPasswordLen = 0;

    memset(szWifiMenuStr, 0, sizeof(szWifiMenuStr));
    memset(szWiFiPassword, 0, sizeof(szWiFiPassword));

    iRet = WiFiMenuCreate(szWifiMenuStr, sizeof(szWifiMenuStr));
    if (iRet != APP_SUCC)
    {
        return iRet;
    }

    while (1)
    {
        iRet = TfxMenuSelect(szWifiMenuStr, "Wi-Fi Menu", NULL, iSelect, &iSelect);
        if (iRet == APP_QUIT)
        {
            return APP_SUCC;
        }

        switch (iSelect)
        {
        case 1:
            break;

        default:
            break;
        }

        iRet = KeyBoardGetCode(szWiFiPassword, sizeof(szWiFiPassword), &numWiFiPasswordLen);
        if (iRet == KEY_ENT)
        {
            iRet = WiFiConnect(szWiFiPassword, stMetaList[iSelect - 1].sSsid, stMetaList[iSelect - 1].temSecMode);
            if(iRet == APP_SUCC)
            {
                return APP_QUIT;
            }
        }
        memset(szWiFiPassword, 0, sizeof(szWiFiPassword));
        numWiFiPasswordLen = 0;
    }

    return APP_SUCC;
}

/*
 * 在屏幕指定位置显示格式化提示信息。
 */
void DisPlayMessage(uint uiX, uint uiY, const char *pDisplayMessage, ...)
{
    char szText[128];
    va_list args;
    va_start(args, pDisplayMessage);

    TfxScrRectangle(uiX, uiY, TfxScrGetWidth(), 20, true, 0xffffff);
    vsnprintf(szText, sizeof(szText), pDisplayMessage, args);
    TfxScrDispText(uiX, uiY, szText, strlen(szText));
    TfxScrUpdate();
    va_end(args);
}
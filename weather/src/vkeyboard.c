#include "tfx_sysinfo.h"
#include "tfx_scr.h"
#include "tfx_ui.h"
#include "tfx_keyboard.h"
#include "tfx_debug.h"
#include "tfx_clock.h"
#include "tfx_power.h"
#include "tfx_wireless.h"
#include "tfx_wifi.h"

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

typedef struct
{
    unsigned int x;
    unsigned int y;
    const char *pLabel;
} KeyBoard;

KeyBoard stKeyboardAbc[] = {
    {20, 270, "q"},
    {50, 270, "w"},
    {80, 270, "e"},
    {110, 270, "r"},
    {140, 270, "t"},
    {170, 270, "y"},
    {200, 270, "u"},
    {230, 270, "i"},
    {260, 270, "o"},
    {290, 270, "p"},

    {35, 320, "a"},
    {65, 320, "s"},
    {95, 320, "d"},
    {125, 320, "f"},
    {155, 320, "g"},
    {185, 320, "h"},
    {215, 320, "j"},
    {245, 320, "k"},
    {275, 320, "l"},

    {35, 370, "123"},
    {80, 370, "z"},
    {110, 370, "x"},
    {140, 370, "c"},
    {170, 370, "v"},
    {200, 370, "b"},
    {230, 370, "n"},
    {260, 370, "m"},

    {50, 420, "esc"},
    {130, 420, "space"},
    {210, 420, "del"},
    {270, 420, "ent"},
};

KeyBoard stKeyboardNum[] = {
    {50, 270, "."},
    {110, 270, "1"},
    {150, 270, "2"},
    {190, 270, "3"},
    {250, 270, "esc"},

    {50, 320, "*"},
    {110, 320, "4"},
    {150, 320, "5"},
    {190, 320, "6"},
    {250, 320, "del"},

    {50, 370, "+"},
    {110, 370, "7"},
    {150, 370, "8"},
    {190, 370, "9"},
    {250, 370, "#"},

    {50, 420, "-"},
    {100, 420, "abc"},
    {150, 420, "0"},
    {190, 420, "space"},
    {250, 420, "ent"},

};
static int iKeyboardSelect = 0;
/*
 * 绘制当前选中的软键盘界面（字母/数字）。
 */
void KeyBoardDraw(void)
{
    static int numKeyBoardABCLen = 0;
    static int numKeyBoardNumLen = 0;
    int i = 0;

    numKeyBoardABCLen = sizeof(stKeyboardAbc) / sizeof(KeyBoard);
    numKeyBoardNumLen = sizeof(stKeyboardNum) / sizeof(KeyBoard);

    TfxScrRectangle(0, 240, TfxScrGetWidth(), TfxScrGetHeight() - 240, true, 0xffffff);

    if (iKeyboardSelect == 0)
    {
        for (i = 0; i < numKeyBoardABCLen; i++)
        {

            TfxScrDispText(stKeyboardAbc[i].x, stKeyboardAbc[i].y, stKeyboardAbc[i].pLabel, strlen(stKeyboardAbc[i].pLabel));
        }
    }
    else if (iKeyboardSelect == 1)
    {
        for (i = 0; i < numKeyBoardNumLen; i++)
        {

            TfxScrDispText(stKeyboardNum[i].x, stKeyboardNum[i].y, stKeyboardNum[i].pLabel, strlen(stKeyboardNum[i].pLabel));
        }
    }

    TfxScrUpdate();
}

/*
 * 处理一次触摸输入，更新输入缓冲区或返回按键事件。
 * 返回：APP_SUCC 表示继续输入；KEY_ENT/APP_QUIT 表示结束。
 */
int KeyBoardInput(char *pInputBuffer, const int numInputBufferSize, int *pInputLen)
{
    static int numKeyBoardABCLen = 0;
    static int numKeyBoardNumLen = 0;
    int iRet = 0;
    int i = 0;

    numKeyBoardABCLen = sizeof(stKeyboardAbc) / sizeof(KeyBoard);
    numKeyBoardNumLen = sizeof(stKeyboardNum) / sizeof(KeyBoard);

    TST_PADDATA stPadData;
    memset(&stPadData, 0, sizeof(TST_PADDATA));

    iRet = TfxReadPad(&stPadData, 1000);
    if (iRet != 0)
    {
        return APP_SUCC;
    }
    else
    {
        KeyBoardDraw();
        if (stPadData.temPadEvent == PADEVENT_TOUCH_UP)
        {
            if (iKeyboardSelect == 0)
            {
                for (i = 0; i < numKeyBoardABCLen; i++)
                {
                    if (stPadData.uiX >= stKeyboardAbc[i].x - 15 && stPadData.uiX <= stKeyboardAbc[i].x + 15 && stPadData.uiY >= stKeyboardAbc[i].y - 25 && stPadData.uiY <= stKeyboardAbc[i].y + 25)
                    {
                        if (strcmp(stKeyboardAbc[i].pLabel, "123") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            iKeyboardSelect = 1;
                            KeyBoardDraw();
                        }
                        else if (strcmp(stKeyboardAbc[i].pLabel, "space") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            if (*pInputLen < numInputBufferSize - 1)
                            {
                                pInputBuffer[*pInputLen] = ' ';
                                (*pInputLen)++;
                                pInputBuffer[*pInputLen] = '\0';
                            }
                        }
                        else if (strcmp(stKeyboardAbc[i].pLabel, "ent") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            return KEY_ENT;
                        }
                        else if (strcmp(stKeyboardAbc[i].pLabel, "del") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            if (*pInputLen > 0)
                            {
                                (*pInputLen)--;
                                pInputBuffer[*pInputLen] = '\0';
                            }
                        }
                        else if (strcmp(stKeyboardAbc[i].pLabel, "esc") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            return APP_QUIT;
                        }
                        else
                        {
                            KeyBoardInputFeedback(i);
                            if (*pInputLen < numInputBufferSize - 1)
                            {
                                pInputBuffer[*pInputLen] = stKeyboardAbc[i].pLabel[0];
                                (*pInputLen)++;
                                pInputBuffer[*pInputLen] = '\0';
                            }
                        }
                        TfxScrRectangle(0, 220, TfxScrGetWidth(), 20, true, 0xffffff);
                        TfxScrDispText(10, 220, pInputBuffer, strlen(pInputBuffer));
                        TfxScrUpdate();
                        break;
                    }
                }
            }
            else if (iKeyboardSelect == 1)
            {
                for (i = 0; i < numKeyBoardNumLen; i++)
                {
                    if (stPadData.uiX >= stKeyboardNum[i].x - 15 && stPadData.uiX <= stKeyboardNum[i].x + 15 && stPadData.uiY >= stKeyboardNum[i].y - 25 && stPadData.uiY <= stKeyboardNum[i].y + 25)
                    {
                        if (strcmp(stKeyboardNum[i].pLabel, "abc") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            iKeyboardSelect = 0;
                            KeyBoardDraw();
                        }
                        else if (strcmp(stKeyboardNum[i].pLabel, "space") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            if (*pInputLen < numInputBufferSize - 1)
                            {
                                pInputBuffer[*pInputLen] = ' ';
                                (*pInputLen)++;
                                pInputBuffer[*pInputLen] = '\0';
                            }
                        }
                        else if (strcmp(stKeyboardNum[i].pLabel, "ent") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            return KEY_ENT;
                        }
                        else if (strcmp(stKeyboardNum[i].pLabel, "del") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            if (*pInputLen > 0)
                            {
                                (*pInputLen)--;
                                pInputBuffer[*pInputLen] = '\0';
                            }
                        }
                        else if (strcmp(stKeyboardNum[i].pLabel, "esc") == 0)
                        {
                            KeyBoardInputFeedback(i);
                            return APP_QUIT;
                        }
                        else
                        {
                            KeyBoardInputFeedback(i);
                            if (*pInputLen < numInputBufferSize - 1)
                            {
                                pInputBuffer[*pInputLen] = stKeyboardNum[i].pLabel[0];
                                (*pInputLen)++;
                                pInputBuffer[*pInputLen] = '\0';
                            }
                        }
                        TfxScrRectangle(0, 220, TfxScrGetWidth(), 20, true, 0xffffff);
                        TfxScrDispText(10, 220, pInputBuffer, strlen(pInputBuffer));
                        TfxScrUpdate();
                        break;
                    }
                }
            }
        }
    }
    return APP_SUCC;
}

/*
 * 对当前按下的按键框选反馈。
 */
void KeyBoardInputFeedback(int numCurrentKeyIndex)
{
    if (iKeyboardSelect == 0)
    {
        TfxScrRectangle(stKeyboardAbc[numCurrentKeyIndex].x - 15, stKeyboardAbc[numCurrentKeyIndex].y - 25, 30, 50, false, 0x000000);
        TfxScrUpdate();
    }
    else if (iKeyboardSelect == 1)
    {
        TfxScrRectangle(stKeyboardNum[numCurrentKeyIndex].x - 15, stKeyboardNum[numCurrentKeyIndex].y - 25, 30, 50, false, 0x000000);
        TfxScrUpdate();
    }
}

/*
 * 获取输入并处理按键，直到返回结束事件。
 */
int KeyBoardGetCode(char *pInputBuffer, const int numInputBufferSize, int *pInputLen)
{
    int iRet = 0;
    iKeyboardSelect = 0;
    KeyBoardDraw();
    while (1)
    {
        iRet = KeyBoardInput(pInputBuffer, numInputBufferSize, pInputLen);
        if (iRet != APP_SUCC)
        {
            return iRet;
        }
    }
    return APP_SUCC;
}

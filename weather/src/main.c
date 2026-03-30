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
#include "cJSON.h"
#include "curl/curl.h"
#include "mosquitto.h"
#include "weather.h"
#include "mqtt.h"

#include <math.h>
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
static void AppFree(void)
{
	TfxKeyboardClose();
	TfxScrDestroy();
	TfxDestroy();
}
static int AppInit(void)
{
	int iRet = 0;
	// struct tm stPosTime;

	// init sys
	TfxInitialize();

	// set debug mode
	TfxSetDebugMode(1); // set debug mode 1  -》print debug in serial port

	// Init screen
	iRet = TfxScrInit();
	if (iRet != 0)
	{
		return iRet;
	}

	// set background
	TfxScrClearAll();
	TfxScrSetBackgroundPic("/paylab/etc/Greem.bmp");
	// set font
	TfxScrInitFont(ZN_TTF);
	TfxScrSetFontColor(0xFF);
	TfxScrSetFontSize(16);
	TfxScrSetLineGapSize(3);

	// set keyboard
	TfxInitKeyboard();
	TfxKeyboardOpen();
	TfxKeyboardEvent(true);

	return 0;
}

int main(void)
{
	int iRet = 0;
	iRet = AppInit(); // initialize
	if (iRet != 0)
	{
		AppFree();
		return EXIT_FAILURE;
	}
	// WiFi连接 分配IP
	WiFiMenuFlow();
    
	iRet = MQTT_Init();
	if(iRet != 0)
	{
		AppFree();
		MQTT_Free();
		return EXIT_FAILURE;
	}
	
	Weather_Interface();

	AppFree();
	MQTT_Free();
	return APP_SUCC;
}

/* End of this file c*/

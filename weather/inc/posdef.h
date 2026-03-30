#ifndef _POSDEF_H_
#define _POSDEF_H_

#define BURN_TEST_SPEED 500

#define FT_BURNIN	    	"burn_in"
#define FT_PRINTER	        "printer"

#define FT_WIRELESS 		"wireless"
#define FT_ETH   		    "eth"
#define FT_WIFI   		    "wifi"
#define FT_MODEM   		    "modem"
#define FT_BT   		    "bt"
#define FT_RAM   		    "ram"
#define FT_TAMPER           "tamper"
#define FT_TEMPHUM  		"temp_hum"


#define FT_USB   		    "usb"
#define FT_COM0             "com0"
#define FT_COM1             "com1"

#define FT_MSR		        "msr"
#define FT_RFID		        "rfid"
#define FT_ICC		        "icc"
#define FT_SAM1		        "sam1"
#define FT_SAM2		        "sam2"

#define FT_LCD		        "lcd"
#define FT_TOUCH   		    "touch"
#define FT_KBD		        "keyboard"
#define FT_BACKLIGHT		"backlight"
#define FT_LED		        "led"
#define FT_FLASH		    "flash"
#define FT_BEEPER	        "buzzer"

#define FT_POWER	        "power"
#define FT_COINBATTERY	    "coinbattery"
#define FT_CURRENT	    	"machinecurrent"

#define FT_MCU   		    "mcu"

#define FT_SCAN				"scan"



#define FT_OK 		"Y"
#define FT_FAIL		"N"
#define FT_NA 		"?"
#define FT_UNSUPPOTED    "U"


typedef enum {
	LANG_EN = 0,
	LANG_CN,
	LANG_NONE = 99
}LANG;

#define FT_IMAGE_PATH "/sysappfs/desktop/ftimage"
#define FT_IMAGE_SUCC "/sysappfs/desktop/ftimage/succ.png"
#define FT_IMAGE_FAIL "/sysappfs/desktop/ftimage/fail.png"	
#define FT_IMAGE_IC "/sysappfs/desktop/ftimage/ic.png"	
#define FT_IMAGE_MSR "/sysappfs/desktop/ftimage/msr.png"	
#define FT_IMAGE_BT "/sysappfs/desktop/ftimage/bt.png"	
#define FT_IMAGE_WIFI "/sysappfs/desktop/ftimage/wifi.png"	
#define FT_IMAGE_WL "/sysappfs/desktop/ftimage/wl.png"	
#define FT_IMAGE_NFC "/sysappfs/desktop/ftimage/nfc.png"	
#define FT_IMAGE_ETH "/sysappfs/desktop/ftimage/eth.png"	
#define FT_IMAGE_SCAN "/sysappfs/desktop/ftimage/scan.png"	


#define FT_IMAGE_X 100
#define FT_IMAGE_Y 30


#define ZN_TTF "/sysappfs/desktop/simfang.ttf"

#define CONFIG_FACTORY_TEST              "/sysappfs/desktop/factorytest.cfg"

#define FILE_PLLOGLV  "/paylab/etc/plloglv"
#define FILE_PLLOGLMD "/paylab/etc/pllogmd"

#define CHECK_LIST_SN "SN"
#define CHECK_LIST_PN "PN"
#define CHECK_LIST_OS "OS_Version"
#define CHECK_LIST_OS_DATE "OS_Date"

#define CHECK_LIST_KERNEL "Kernel_Version"
#define CHECK_LIST_UBOOT "UBoot_Version"
#define CHECK_LIST_SDK "Libsdk_Version"
#define CHECK_LIST_HWID "HW_ID"
#define CHECK_LIST_CA "CA_Name"

#define CHECK_LIST_IMEI "IMEI"
#define CHECK_LIST_CCID "CCID"
#define CHECK_LIST_IMSI "IMSI"
#define CHECK_LIST_APN "APN"
#define CHECK_LIST_CID "CID"
#define CHECK_LIST_ATD "ATD"
#define CHECK_LIST_WL_PIN "WL_PIN"
#define CHECK_LIST_WL_PASSWORD "WL_Password"
#define CHECK_LIST_WL_USERNAME "WL_UserName"
#define CHECK_LIST_WL_SIGNAL "WL_Signal"
#define CHECK_LIST_WL_SCANMODE "WL_ScanMode"
#define CHECK_LIST_WL_Band "WL_Band"


#define CHECK_LIST_WIFI_SIGNAL "WiFi_Signal_Strength"
#define CHECK_LIST_WIFI_MAC "WiFi_MAC_address"
#define CHECK_LIST_BT_NAME "BT_name"
#define CHECK_LIST_BT_MAC "BT_MAC_address"
#define CHECK_LIST_ETH_MAC "ETH_Mac_address"
#define CHECK_LIST_FLASH "Bad_Flash"
#define CHECK_LIST_TIME "Test_date_and_time"
#define CHECK_LIST_DISK_ROOTFS "Disk_Rootfs"
#define CHECK_LIST_DISK_APPFS "Disk_Appfs"

#define CHECK_LIST_TEMP "Temperature"
#define CHECK_LIST_HUMI "Humidity"

#define CHECK_LIST_TAMPER "Tamper"


#define CHECK_LIST_FT "Ft_Version"
#define CHECK_LIST_APP "APP_Version"


//PC Tool Command
#define CMD_REQUEST_CLIC        0x19
#define CMD_SET_DATETIME        0x18
#define CMD_SETSN              0x01


#define CMD_CHECK_TIME 0x10
#define CMD_PUSH_DB 0x11
#define CMD_CHECK_DB 0x12
#define CMD_CHECK_SURFACESN 0x13


//Thread State
#define THREAD_RUNNING -6
#define THREAD_CREATE_ERR -7
#define THREAD_UNINIT -8
#define THREAD_JOIN_ERR -9
#define THREAD_UNKNOWN -10


#endif


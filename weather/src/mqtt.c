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
#include "mosquitto.h"
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
struct mosquitto *mosq = NULL;
// MQTT初始化
int MQTT_Init(void)
{
    int ret=0;
    // 1. 初始化mosquitto库
    ret = mosquitto_lib_init();
    if (ret != MOSQ_ERR_SUCCESS) {
        DebugPrintf("初始化mosquitto库失败: %d\n", ret);
        return EXIT_FAILURE;
    }

    // 2. 创建MQTT客户端实例 
    mosq = mosquitto_new(MQTT_CLIENT_ID, CLEAN_SESSION, NULL);
    if (mosq == NULL) {
        DebugPrintf("创建MQTT客户端实例失败\n");
        mosquitto_lib_cleanup();
        return EXIT_FAILURE;
    }

    // 3. 设置回调函数（可选，但建议设置）
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
	mosquitto_message_callback_set(mosq, on_message);
    //mosquitto_error_callback_set(mosq, on_error);

    // 4. 连接MQTT服务器
    ret = mosquitto_connect(mosq, MQTT_BROKER, MQTT_PORT, MQTT_KEEPALIVE);
    if (ret != MOSQ_ERR_SUCCESS) {
        DebugPrintf("连接服务器失败: %d\n", ret);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return EXIT_FAILURE;
    }
	mosquitto_subscribe(mosq, NULL, "device/cmd", 0);
	mosquitto_subscribe(mosq, NULL, "device/test", 0);
    return EXIT_SUCCESS;
}
// MQTT释放内存
void MQTT_Free(void)
{
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}
// 连接回调函数（当客户端连接/断开时触发）
void on_connect(struct mosquitto *mosq, void *obj, int rc) {
    (void)mosq;
    (void)obj;
    if (rc == MOSQ_ERR_SUCCESS) {
        DebugPrintf("成功连接到MQTT服务器！\n");
    } else {
        DebugPrintf("连接MQTT服务器失败，错误码: %d\n", rc);
        mosquitto_disconnect(mosq);
    }
}

// 断开连接回调函数
void on_disconnect(struct mosquitto *mosq, void *obj, int rc) {
    (void)mosq;
    (void)obj;
    if (rc != 0) {
        DebugPrintf("意外断开连接，错误码: %d\n", rc);
    } else {
        DebugPrintf("正常断开与MQTT服务器的连接\n");
    }
}

// 错误回调函数
void on_error(struct mosquitto *mosq, void *obj, int rc) {
    (void)mosq;
    (void)obj;
    DebugPrintf("MQTT客户端错误: %d\n", rc);
}
char Receive_Data_Buffer[20]={0};
char Receive_Topic_Buffer[20]={0};
void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg)
{
    (void)mosq;
    (void)userdata;
    // 收到消息就会跑到这里！
    
    DebugPrintf("收到主题：%s\n", msg->topic);
    DebugPrintf("收到内容：%s L: %d\n", (char*)msg->payload, msg->payloadlen);

    memset(Receive_Data_Buffer,0,20);memset(Receive_Topic_Buffer,0,20);
    strncpy(Receive_Data_Buffer, (msg->payload)+11, (strlen(msg->payload))-11-2);
    strcpy(Receive_Topic_Buffer,(char*)msg->topic);

    // 你可以在这里做逻辑：开灯、开门、刷新天气等
}

#ifndef MQTT_H
#define MQTT_H

extern struct mosquitto *mosq;
extern char Receive_Data_Buffer[20];
extern char Receive_Topic_Buffer[20];
// MQTT 服务器配置
#define MQTT_BROKER "test.mosquitto.org"  // 服务器地址
#define MQTT_PORT 1883           // 服务器端口
#define MQTT_KEEPALIVE 60        // 心跳间隔（秒）
#define MQTT_CLIENT_ID "test_client"  // 客户端ID（NULL则自动生成）
#define CLEAN_SESSION true       // 清理会话标识

int MQTT_Init(void);
void MQTT_Free(void);
void on_connect(struct mosquitto *mosq, void *obj, int rc);
void on_disconnect(struct mosquitto *mosq, void *obj, int rc);
void on_error(struct mosquitto *mosq, void *obj, int rc);
void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg);
#endif

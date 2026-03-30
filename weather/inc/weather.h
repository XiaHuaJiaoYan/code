#ifndef WEATHER_H
#define WEATHER_H

// 免费天气接口（高德天气，无需注册，替换为你的城市adcode，如北京110100）
#define WEATHER_API "https://restapi.amap.com/v3/weather/weatherInfo?city=350100&key=84099c8b09be05c8ffaaa7de5923aacc"


#define SUN_CX     60    // 太阳圆心X
#define SUN_CY     40    // 太阳圆心Y
#define SUN_RADIUS 15    // 太阳半径（点击检测范围放大为2倍，提高容错）
#define SUN_CLICK_RANGE (SUN_RADIUS * 2)
// 在 (cx, cy) 位置画太阳，半径 r
int Weather_Interface(void);
#endif

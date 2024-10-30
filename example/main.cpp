#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "../lib/qweather/qweather.h"

const char *ssid = "YOUR_SSID";      // 你的网络名称
const char *password = "YOUR_PASSWORD"; // 你的网络密码
const int location = 101300903;    // 你的城市ID,如 101300903:北流
const String apiKey = "YOUR_API_KEY";   // 你的和风天气API密钥
const String apiVersion = "devapi";  // 你的和风天气API类型，devapi:免费版，api:付费版

void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected!");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // 调用qweather函数获取天气信息
  DynamicJsonDocument jsonDoc = static_cast<DynamicJsonDocument>(qweather(location, apiKey, apiVersion));
  // 使用ArduinoJson在串口监视器中打印JSON文档
  serializeJsonPretty(jsonDoc, Serial);
}

void loop()
{
}
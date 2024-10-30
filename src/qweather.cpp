#include "qweather.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ArduinoUZlib.h>

const String URL_ADDRESS = ".qweather.com/v7/weather/3d";

/**
 * 获取指定地点的天气信息
 *
 * @param location 地点，格式为城市ID，如 "101010100"
 * @param apiKey 和风天气API密钥
 * @param apiVersion 和风天气API类型，免费用"devapi", 付费版"api"
 * @return 包含天气信息的JSON文档，如果请求失败则返回空文档
 */
DynamicJsonDocument qweather(const int &location, const String &apiKey, const String &apiVersion)
{
    // get方法请求
    HTTPClient http;

    String url = "https://" + apiVersion + URL_ADDRESS + "?location=" + location + "&key=" + apiKey;
    http.begin(url);
    http.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36");
    http.addHeader("Accept", "application/json");

    // 发起请求
    int httpCode = http.GET();

    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK)
        {
            // 由于和风天气默认使用了 GZIP 压缩，因此需要使用 ArduinoUZlib 库解压响应数据
            WiFiClient *stream = http.getStreamPtr();
            size_t size = stream->available();
            uint8_t inbuff[size];
            stream->readBytes(inbuff, size);
            uint8_t *outbuf = NULL;
            uint32_t outsize = 0;

            int result = ArduinoUZlib::decompress(inbuff, size, outbuf, outsize);

            DynamicJsonDocument  jsonDoc(2048);
            DeserializationError error = deserializeJson(jsonDoc, outbuf, outsize);
            http.end();
            return jsonDoc;
        }
        else
        {
            Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode));
            http.end();
            return DynamicJsonDocument(0); // 返回一个空的JSON文档
        }
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        http.end();
        return DynamicJsonDocument(0); // 返回一个空的JSON文档
    }
}
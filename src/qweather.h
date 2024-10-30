#ifndef GETWEATHER_H
#define GETWEATHER_H

#include <ArduinoJson.h>
#include <String.h>

DynamicJsonDocument qweather(const int& location, const String& apiKey, const String& apiVersion);

#endif // GETWEATHER_H
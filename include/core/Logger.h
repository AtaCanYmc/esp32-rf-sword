/**
 * @file Logger.h
 * @brief Thread-safe colorized logging engine for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#ifndef HOST_SIMULATION
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#else
#include <iostream>
#include <string>
#endif

enum class LogLevel : uint8_t {
    LOG_NONE = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_VERBOSE
};

class Logger {
public:
    static void init(LogLevel level = LogLevel::LOG_INFO);
    static void setLevel(LogLevel level);
    static LogLevel getLevel();
    
    static void error(const char* tag, const char* format, ...);
    static void warn(const char* tag, const char* format, ...);
    static void info(const char* tag, const char* format, ...);
    static void debug(const char* tag, const char* format, ...);
    static void success(const char* tag, const char* format, ...);
    static void raw(const char* format, ...);
    
    typedef void (*LogCallback)(LogLevel level, const char* tag, const char* message);
    static void setCallback(LogCallback cb);

private:
    static LogLevel currentLevel;
    static void logFormatted(LogLevel level, const char* color, const char* prefix, const char* tag, const char* format, va_list args);
    static LogCallback externalCallback;
#ifndef HOST_SIMULATION
    static SemaphoreHandle_t logMutex;
#endif
};

#define SWORD_LOGE(tag, ...) Logger::error(tag, __VA_ARGS__)
#define SWORD_LOGW(tag, ...) Logger::warn(tag, __VA_ARGS__)
#define SWORD_LOGI(tag, ...) Logger::info(tag, __VA_ARGS__)
#define SWORD_LOGD(tag, ...) Logger::debug(tag, __VA_ARGS__)
#define SWORD_LOGS(tag, ...) Logger::success(tag, __VA_ARGS__)
#define SWORD_RAW(...)       Logger::raw(__VA_ARGS__)

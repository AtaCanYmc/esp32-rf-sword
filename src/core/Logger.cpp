/**
 * @file Logger.cpp
 * @brief Thread-safe colorized logging implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "core/Logger.h"

// ANSI Color Codes
#define ANSI_RESET   "\033[0m"
#define ANSI_RED     "\033[1;31m"
#define ANSI_GREEN   "\033[1;32m"
#define ANSI_YELLOW  "\033[1;33m"
#define ANSI_BLUE    "\033[1;34m"
#define ANSI_MAGENTA "\033[1;35m"
#define ANSI_CYAN    "\033[1;36m"
#define ANSI_WHITE   "\033[1;37m"

LogLevel Logger::currentLevel = LogLevel::LOG_INFO;
Logger::LogCallback Logger::externalCallback = nullptr;

#ifndef HOST_SIMULATION
SemaphoreHandle_t Logger::logMutex = nullptr;
#endif

void Logger::init(LogLevel level) {
    currentLevel = level;
#ifndef HOST_SIMULATION
    if (logMutex == nullptr) {
        logMutex = xSemaphoreCreateMutex();
    }
#endif
}

void Logger::setLevel(LogLevel level) {
    currentLevel = level;
}

LogLevel Logger::getLevel() {
    return currentLevel;
}

void Logger::setCallback(LogCallback cb) {
    externalCallback = cb;
}

void Logger::logFormatted(LogLevel level, const char* color, const char* prefix, const char* tag, const char* format, va_list args) {
    if (level > currentLevel) return;

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);

#ifndef HOST_SIMULATION
    if (logMutex != nullptr) {
        xSemaphoreTake(logMutex, portMAX_DELAY);
    }
    
    Serial.printf("%s[%s][%s]%s %s\r\n", color, prefix, tag, ANSI_RESET, buffer);

    if (logMutex != nullptr) {
        xSemaphoreGive(logMutex);
    }
#else
    printf("%s[%s][%s]%s %s\n", color, prefix, tag, ANSI_RESET, buffer);
#endif

    if (externalCallback != nullptr) {
        externalCallback(level, tag, buffer);
    }
}

void Logger::error(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    logFormatted(LogLevel::LOG_ERROR, ANSI_RED, "ERR", tag, format, args);
    va_end(args);
}

void Logger::warn(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    logFormatted(LogLevel::LOG_WARN, ANSI_YELLOW, "WRN", tag, format, args);
    va_end(args);
}

void Logger::info(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    logFormatted(LogLevel::LOG_INFO, ANSI_CYAN, "INF", tag, format, args);
    va_end(args);
}

void Logger::debug(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    logFormatted(LogLevel::LOG_DEBUG, ANSI_MAGENTA, "DBG", tag, format, args);
    va_end(args);
}

void Logger::success(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    logFormatted(LogLevel::LOG_INFO, ANSI_GREEN, "OK ", tag, format, args);
    va_end(args);
}

void Logger::raw(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

#ifndef HOST_SIMULATION
    if (logMutex != nullptr) {
        xSemaphoreTake(logMutex, portMAX_DELAY);
    }
    Serial.print(buffer);
    if (logMutex != nullptr) {
        xSemaphoreGive(logMutex);
    }
#else
    printf("%s", buffer);
#endif
}

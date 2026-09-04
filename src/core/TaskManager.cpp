/**
 * @file TaskManager.cpp
 * @brief Multi-Core FreeRTOS Task Management implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "core/TaskManager.h"
#include "core/SystemState.h"
#include "core/Logger.h"
#include "attack/AttackCoordinator.h"
#include "ui/WebServerManager.h"
#include "ui/DisplayManager.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#endif

TaskManager& TaskManager::instance() {
    static TaskManager s_instance;
    return s_instance;
}

TaskManager::TaskManager() {
}

bool TaskManager::startTasks() {
#ifndef HOST_SIMULATION
    if (tasksRunning) return true;

    // Detect core count
#if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6)
    // Single core RISC-V: allocate on Core 0
    BaseType_t resRF = xTaskCreatePinnedToCore(
        rfWorkerTask,
        "RFWorker",
        RF_WORKER_STACK_SIZE,
        nullptr,
        RF_WORKER_PRIORITY,
        nullptr,
        0
    );

    BaseType_t resSys = xTaskCreatePinnedToCore(
        systemManagerTask,
        "SysManager",
        SYSTEM_MGR_STACK_SIZE,
        nullptr,
        SYSTEM_MGR_PRIORITY,
        nullptr,
        0
    );
#else
    // Dual Core Xtensa LX6 / LX7 (ESP32 Classic / ESP32-S3):
    // Dedicated Core 1 for RF timing; Core 0 for Web & System
    BaseType_t resRF = xTaskCreatePinnedToCore(
        rfWorkerTask,
        "RFWorker",
        RF_WORKER_STACK_SIZE,
        nullptr,
        RF_WORKER_PRIORITY,
        nullptr,
        RF_WORKER_CORE
    );

    BaseType_t resSys = xTaskCreatePinnedToCore(
        systemManagerTask,
        "SysManager",
        SYSTEM_MGR_STACK_SIZE,
        nullptr,
        SYSTEM_MGR_PRIORITY,
        nullptr,
        SYSTEM_MGR_CORE
    );
#endif

    if (resRF == pdPASS && resSys == pdPASS) {
        tasksRunning = true;
        SWORD_LOGS("TASK", "FreeRTOS worker tasks spawned successfully.");
        return true;
    } else {
        SWORD_LOGE("TASK", "Failed to create FreeRTOS tasks (RF=%d, Sys=%d).", resRF, resSys);
        return false;
    }
#else
    tasksRunning = true;
    return true;
#endif
}

void TaskManager::stopTasks() {
    tasksRunning = false;
}

void TaskManager::rfWorkerTask(void* parameter) {
    SWORD_LOGI("RF_TASK", "RF Worker thread started.");
    while (true) {
        AttackCoordinator::instance().step();
#if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6)
        // On single core systems, yield slightly to prevent watchdog trigger
        taskYIELD();
#endif
    }
}

void TaskManager::systemManagerTask(void* parameter) {
    SWORD_LOGI("SYS_TASK", "System Manager thread started.");
    while (true) {
        WebServerManager::instance().handleDNS();
        WebServerManager::instance().broadcastTelemetry();
        DisplayManager::instance().updateHUD();
        SystemState::instance().updateSystemMetrics();

#ifndef HOST_SIMULATION
        vTaskDelay(pdMS_TO_TICKS(50)); // 20 Hz cycle
#endif
    }
}

/**
 * @file TaskManager.h
 * @brief Multi-Core FreeRTOS Task Management for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include "Config.h"

class TaskManager {
public:
    static TaskManager& instance();

    bool startTasks();
    void stopTasks();

private:
    TaskManager();
    ~TaskManager() = default;

    static void rfWorkerTask(void* parameter);
    static void systemManagerTask(void* parameter);

    bool tasksRunning = false;
};

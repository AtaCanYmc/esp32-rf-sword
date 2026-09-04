/**
 * @file WebServerManager.h
 * @brief Embedded Web Server, WebSocket Server & Captive Portal Manager
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>

#ifndef HOST_SIMULATION
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#endif

class WebServerManager {
public:
    static WebServerManager& instance();

    bool init(const DeviceConfig& config);
    void handleDNS();
    void broadcastTelemetry();
    void stop();

    bool isRunning() const;

private:
    WebServerManager();
    ~WebServerManager();

    bool running = false;
    uint32_t lastBroadcastTime = 0;

#ifndef HOST_SIMULATION
    AsyncWebServer* server = nullptr;
    AsyncWebSocket* ws = nullptr;
    DNSServer* dnsServer = nullptr;

    void setupRoutes();
    void onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);
    void handleClientCommand(AsyncWebSocketClient* client, const char* jsonPayload);
#endif
};

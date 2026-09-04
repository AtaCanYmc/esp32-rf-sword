/**
 * @file WebServerManager.cpp
 * @brief Embedded Web Server, WebSocket & Captive Portal implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "ui/WebServerManager.h"
#include "ui/WebAssets.h"
#include "core/SystemState.h"
#include "core/NVSManager.h"
#include "core/Logger.h"
#include "attack/AttackCoordinator.h"
#include "analyzer/SpectrumScanner.h"

#ifndef HOST_SIMULATION
#include <WiFi.h>
#include <ArduinoJson.h>
#endif

WebServerManager& WebServerManager::instance() {
    static WebServerManager s_instance;
    return s_instance;
}

WebServerManager::WebServerManager() {
}

WebServerManager::~WebServerManager() {
    stop();
}

bool WebServerManager::init(const DeviceConfig& config) {
    if (!config.wifiApEnabled) {
        SWORD_LOGI("WEB", "Wi-Fi AP mode disabled by configuration.");
        return false;
    }

#ifndef HOST_SIMULATION
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(config.apSsid, config.apPassword, config.apChannel);

    IPAddress IP = WiFi.softAPIP();
    SWORD_LOGS("WEB", "Wi-Fi Access Point active: SSID='%s', IP=%s", config.apSsid, IP.toString().c_str());

    // Setup DNS Server for Captive Portal (redirects all queries to SoftAP IP)
    dnsServer = new DNSServer();
    dnsServer->start(CAPTIVE_PORTAL_DNS_PORT, "*", IP);

    server = new AsyncWebServer(DEFAULT_HTTP_PORT);
    ws = new AsyncWebSocket("/ws");

    ws->onEvent([this](AsyncWebSocket* s, AsyncWebSocketClient* c, AwsEventType t, void* arg, uint8_t* d, size_t l) {
        this->onWebSocketEvent(s, c, t, arg, d, l);
    });

    server->addHandler(ws);
    setupRoutes();
    server->begin();

    running = true;
    SWORD_LOGS("WEB", "Async HTTP & WebSocket server listening on port %d", DEFAULT_HTTP_PORT);
    return true;
#else
    running = true;
    return true;
#endif
}

void WebServerManager::handleDNS() {
#ifndef HOST_SIMULATION
    if (dnsServer != nullptr) {
        dnsServer->processNextRequest();
    }
#endif
}

#ifndef HOST_SIMULATION
void WebServerManager::setupRoutes() {
    // Serve Web UI Single Page Application
    server->on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", WebAssets::getIndexHtml());
    });

    // Captive Portal Detection Endpoints
    server->on("/generate_204", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->redirect("/");
    });
    server->on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->redirect("/");
    });
    server->on("/canonical.html", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->redirect("/");
    });
    server->on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->redirect("/");
    });

    // REST API Endpoints
    server->on("/api/status", HTTP_GET, [](AsyncWebServerRequest* request) {
        TelemetryData t = SystemState::instance().getTelemetry();
        JsonDocument doc;
        doc["uptime"] = t.uptimeSeconds;
        doc["freeHeap"] = t.freeHeapBytes;
        doc["mode"] = SystemState::instance().getModeName(t.activeMode);
        doc["preset"] = SystemState::instance().getPresetName(t.activePreset);
        doc["hopsPerSec"] = t.totalHopsPerSecond;
        doc["packetsPerSec"] = t.totalPacketsPerSecond;
        doc["running"] = SystemState::instance().isRunning();

        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });
}

void WebServerManager::onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        SWORD_LOGI("WS", "Client #%u connected from %s", client->id(), client->remoteIP().toString().c_str());
    } else if (type == WS_EVT_DISCONNECT) {
        SWORD_LOGI("WS", "Client #%u disconnected", client->id());
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo* info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0;
            handleClientCommand(client, (char*)data);
        }
    }
}

void WebServerManager::handleClientCommand(AsyncWebSocketClient* client, const char* jsonPayload) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, jsonPayload);
    if (err) return;

    const char* action = doc["action"];
    if (action == nullptr) return;

    if (strcmp(action, "toggle_run") == 0) {
        bool run = doc["run"] | false;
        if (run) {
            SystemState::instance().start();
            SWORD_LOGI("WEB", "Attack started by web client.");
        } else {
            AttackCoordinator::instance().stop();
            SWORD_LOGI("WEB", "Attack stopped by web client.");
        }
    } else if (strcmp(action, "set_mode") == 0) {
        const char* modeStr = doc["mode"];
        if (modeStr) {
            if (strcmp(modeStr, "SWEEP_COPRIME") == 0) AttackCoordinator::instance().setMode(AttackMode::SWEEP_COPRIME);
            else if (strcmp(modeStr, "SWEEP_LINEAR") == 0) AttackCoordinator::instance().setMode(AttackMode::SWEEP_LINEAR);
            else if (strcmp(modeStr, "SWEEP_RANDOM") == 0) AttackCoordinator::instance().setMode(AttackMode::SWEEP_RANDOM);
            else if (strcmp(modeStr, "TARGETED_BLE") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_BLE);
            else if (strcmp(modeStr, "TARGETED_WIFI") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_WIFI);
            else if (strcmp(modeStr, "TARGETED_ZIGBEE") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_ZIGBEE);
            else if (strcmp(modeStr, "TARGETED_DRONE") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_DRONE);
            else if (strcmp(modeStr, "TARGETED_SUBGHZ") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_SUBGHZ);
            else if (strcmp(modeStr, "NOISE_BURST") == 0) AttackCoordinator::instance().setMode(AttackMode::NOISE_BURST);
            else if (strcmp(modeStr, "SPECTRUM_ANALYZER") == 0) AttackCoordinator::instance().setMode(AttackMode::SPECTRUM_ANALYZER);
            else if (strcmp(modeStr, "WIFI_BEACON_SPAM") == 0) AttackCoordinator::instance().setMode(AttackMode::WIFI_BEACON_SPAM);
            else if (strcmp(modeStr, "WIFI_DEAUTH_STORM") == 0) AttackCoordinator::instance().setMode(AttackMode::WIFI_DEAUTH_STORM);
            else if (strcmp(modeStr, "BLE_POPUP_SPAM") == 0) AttackCoordinator::instance().setMode(AttackMode::BLE_POPUP_SPAM);
            else if (strcmp(modeStr, "TOTAL_BLITZ") == 0) AttackCoordinator::instance().setMode(AttackMode::TOTAL_BLITZ);
        }
    } else if (strcmp(action, "set_preset") == 0) {
        const char* presetStr = doc["preset"];
        if (presetStr) {
            TargetPreset preset = Presets::getPresetByName(presetStr);
            AttackCoordinator::instance().applyPreset(preset);
        }
    } else if (strcmp(action, "set_channels") == 0) {
        uint8_t minCh = doc["min"] | 2;
        uint8_t maxCh = doc["max"] | 80;
        SystemState::instance().setChannels(minCh, maxCh);
    } else if (strcmp(action, "set_dwell") == 0) {
        uint16_t minDwell = doc["min"] | 120;
        uint16_t maxDwell = doc["max"] | 180;
        SystemState::instance().setDwellRangeUs(minDwell, maxDwell);
    } else if (strcmp(action, "set_power") == 0) {
        const char* pwr = doc["power"];
        if (pwr) {
            if (strcmp(pwr, "MIN") == 0) SystemState::instance().setPowerLevel(RadioPowerLevel::POWER_MIN);
            else if (strcmp(pwr, "LOW") == 0) SystemState::instance().setPowerLevel(RadioPowerLevel::POWER_LOW);
            else if (strcmp(pwr, "HIGH") == 0) SystemState::instance().setPowerLevel(RadioPowerLevel::POWER_HIGH);
            else if (strcmp(pwr, "MAX") == 0) SystemState::instance().setPowerLevel(RadioPowerLevel::POWER_MAX);
        }
    } else if (strcmp(action, "save_config") == 0) {
        DeviceConfig cfg = SystemState::instance().getConfig();
        NVSManager::saveConfig(cfg);
    } else if (strcmp(action, "reboot") == 0) {
        ESP.restart();
    }
}
#endif

void WebServerManager::broadcastTelemetry() {
#ifndef HOST_SIMULATION
    if (ws == nullptr || ws->count() == 0) return;

    uint32_t now = millis();
    if (now - lastBroadcastTime < 100) return; // 10 Hz broadcast
    lastBroadcastTime = now;

    TelemetryData t = SystemState::instance().getTelemetry();
    JsonDocument doc;
    doc["type"] = "telemetry";

    JsonObject tel = doc["telemetry"].to<JsonObject>();
    tel["uptime"] = t.uptimeSeconds;
    tel["freeHeap"] = t.freeHeapBytes;
    tel["tempC"] = t.coreTemperatureC;
    tel["hopsPerSec"] = t.totalHopsPerSecond;
    tel["packetsPerSec"] = t.totalPacketsPerSecond;
    tel["mode"] = SystemState::instance().getModeName(t.activeMode);
    tel["preset"] = SystemState::instance().getPresetName(t.activePreset);
    tel["power"] = SystemState::instance().getPowerLevelName(t.powerLevel);
    tel["radioCount"] = t.activeRadioCount;
    tel["running"] = SystemState::instance().isRunning();

    JsonArray radios = doc["radios"].to<JsonArray>();
    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
        RadioStatus st = SystemState::instance().getRadioStatus(i);
        JsonObject r = radios.add<JsonObject>();
        r["present"] = st.present;
        r["active"] = st.active;
        r["channel"] = st.currentChannel;
        r["freq"] = st.currentFrequencyMHz;
    }

    // Include 128-channel spectrum data
    uint8_t scan[128];
    SpectrumScanner::instance().getLatestScan(scan);
    JsonArray spec = doc["spectrum"].to<JsonArray>();
    for (int i = 0; i < 128; i++) {
        spec.add(scan[i]);
    }

    String output;
    serializeJson(doc, output);
    ws->textAll(output);
#endif
}

void WebServerManager::stop() {
#ifndef HOST_SIMULATION
    if (server != nullptr) {
        server->end();
        delete server;
        server = nullptr;
    }
    if (ws != nullptr) {
        delete ws;
        ws = nullptr;
    }
    if (dnsServer != nullptr) {
        dnsServer->stop();
        delete dnsServer;
        dnsServer = nullptr;
    }
#endif
    running = false;
}

bool WebServerManager::isRunning() const {
    return running;
}

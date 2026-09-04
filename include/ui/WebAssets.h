/**
 * @file WebAssets.h
 * @brief Auto-generated standalone Web UI bundle for ESP32-RF-SWORD
 * @note Generated automatically by scripts/bundle_web.py
 */

#pragma once

#include <stddef.h>

namespace WebAssets {

const char INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32-RF-SWORD // RF Research & Security Dashboard</title>
    <style>
/* ==============================================================================
   ESP32-RF-SWORD: Modern Cyberpunk Dashboard Theme
   ============================================================================== */

:root {
    --bg-dark: #070a0f;
    --bg-card: #0d121c;
    --bg-card-hover: #131b2a;
    --border-color: #1e293b;
    --border-accent: #00f0ff33;
    
    --accent-cyan: #00f0ff;
    --accent-green: #00ff88;
    --accent-red: #ff3366;
    --accent-amber: #ffb800;
    --accent-purple: #9d4edd;
    
    --text-main: #e2e8f0;
    --text-muted: #94a3b8;
    --text-dim: #64748b;
    
    --font-mono: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, "Liberation Mono", "Courier New", monospace;
    --font-sans: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen, Ubuntu, Cantarell, "Helvetica Neue", sans-serif;
}

* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
}

body {
    background-color: var(--bg-dark);
    color: var(--text-main);
    font-family: var(--font-sans);
    min-height: 100vh;
    display: flex;
    justify-content: center;
    padding: 16px;
}

.hud-container {
    width: 100%;
    max-width: 1200px;
    display: flex;
    flex-direction: column;
    gap: 16px;
}

/* Header */
.hud-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    background: var(--bg-card);
    border: 1px solid var(--border-color);
    padding: 14px 20px;
    border-radius: 12px;
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
}

.brand {
    display: flex;
    align-items: center;
    gap: 12px;
}

.brand-icon {
    font-size: 26px;
    color: var(--accent-cyan);
    filter: drop-shadow(0 0 8px var(--accent-cyan));
}

.brand-text h1 {
    font-size: 20px;
    font-weight: 800;
    letter-spacing: 1px;
    color: #fff;
    font-family: var(--font-mono);
}

.badge {
    font-size: 10px;
    background: var(--accent-cyan);
    color: #000;
    font-weight: bold;
    padding: 2px 6px;
    border-radius: 4px;
    font-family: var(--font-mono);
}

.status-indicator {
    display: flex;
    align-items: center;
    gap: 8px;
    font-family: var(--font-mono);
    font-size: 12px;
}

.status-dot {
    width: 10px;
    height: 10px;
    border-radius: 50%;
    display: inline-block;
}

.status-dot.connected {
    background: var(--accent-green);
    box-shadow: 0 0 10px var(--accent-green);
}

.status-dot.disconnected {
    background: var(--accent-red);
    box-shadow: 0 0 10px var(--accent-red);
}

.uptime {
    margin-left: 12px;
    color: var(--text-dim);
}

/* Metrics Grid */
.metrics-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(170px, 1fr));
    gap: 12px;
}

.metric-card {
    background: var(--bg-card);
    border: 1px solid var(--border-color);
    padding: 12px 16px;
    border-radius: 8px;
    display: flex;
    flex-direction: column;
    gap: 4px;
}

.metric-label {
    font-size: 11px;
    font-family: var(--font-mono);
    color: var(--text-dim);
    letter-spacing: 0.5px;
}

.metric-value {
    font-size: 16px;
    font-weight: 700;
    font-family: var(--font-mono);
    color: #fff;
}

.metric-value.accent {
    color: var(--accent-cyan);
    text-shadow: 0 0 10px rgba(0, 240, 255, 0.4);
}

.metric-value small {
    font-size: 11px;
    color: var(--text-muted);
}

/* Dashboard Body Layout */
.dashboard-body {
    display: grid;
    grid-template-columns: 1fr 380px;
    gap: 16px;
}

@media (max-width: 900px) {
    .dashboard-body {
        grid-template-columns: 1fr;
    }
}

.main-panel, .control-panel, .log-panel {
    background: var(--bg-card);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    padding: 18px;
    display: flex;
    flex-direction: column;
    gap: 16px;
}

.panel-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.panel-header h2 {
    font-size: 14px;
    font-family: var(--font-mono);
    letter-spacing: 0.5px;
    color: var(--accent-cyan);
}

/* Canvas & Spectrum */
.canvas-container {
    background: #04060a;
    border: 1px solid var(--border-color);
    border-radius: 8px;
    overflow: hidden;
    display: flex;
    justify-content: center;
}

#waterfallCanvas {
    width: 100%;
    height: 240px;
    display: block;
}

.spectrum-labels {
    display: flex;
    justify-content: space-between;
    font-family: var(--font-mono);
    font-size: 10px;
    color: var(--text-dim);
    padding: 0 4px;
}

/* Radio Gauges */
.radio-gauges-container {
    display: grid;
    grid-template-columns: repeat(2, 1fr);
    gap: 10px;
}

.radio-gauge {
    background: #090e17;
    border: 1px solid var(--border-color);
    padding: 10px;
    border-radius: 6px;
    display: flex;
    flex-direction: column;
    gap: 6px;
}

.gauge-title {
    font-size: 11px;
    font-family: var(--font-mono);
    color: var(--text-muted);
}

.gauge-bar-bg {
    background: #141c2b;
    height: 8px;
    border-radius: 4px;
    overflow: hidden;
}

.gauge-bar-fill {
    background: linear-gradient(90deg, var(--accent-cyan), var(--accent-green));
    height: 100%;
    transition: width 0.1s ease;
}

.gauge-info {
    font-size: 11px;
    font-family: var(--font-mono);
    color: #fff;
}

/* Controls */
.master-switch-box {
    width: 100%;
}

.btn {
    border: none;
    border-radius: 6px;
    padding: 10px 16px;
    font-weight: 700;
    font-family: var(--font-mono);
    cursor: pointer;
    transition: all 0.2s;
}

.btn-master {
    width: 100%;
    padding: 16px;
    font-size: 15px;
    letter-spacing: 1px;
}

.btn-primary {
    background: var(--accent-cyan);
    color: #000;
}

.btn-primary:hover {
    background: #5cf4ff;
    box-shadow: 0 0 15px rgba(0, 240, 255, 0.5);
}

.btn-master.active-stop {
    background: var(--accent-red);
    color: #fff;
    box-shadow: 0 0 15px rgba(255, 51, 102, 0.5);
}

.btn-secondary {
    background: #1e293b;
    color: var(--text-main);
}

.btn-secondary:hover {
    background: #334155;
}

.btn-danger {
    background: #3a101b;
    color: var(--accent-red);
    border: 1px solid var(--accent-red);
}

.btn-danger:hover {
    background: var(--accent-red);
    color: #fff;
}

.btn-sm {
    padding: 4px 10px;
    font-size: 11px;
}

.control-group {
    display: flex;
    flex-direction: column;
    gap: 8px;
}

.control-group label {
    font-size: 11px;
    font-family: var(--font-mono);
    color: var(--text-muted);
    font-weight: 600;
}

.form-select {
    background: #090e17;
    border: 1px solid var(--border-color);
    color: #fff;
    padding: 10px;
    border-radius: 6px;
    font-family: var(--font-mono);
    font-size: 12px;
}

.range-labels {
    display: flex;
    justify-content: space-between;
    font-size: 11px;
    font-family: var(--font-mono);
    color: var(--text-muted);
}

.dual-range {
    display: flex;
    flex-direction: column;
    gap: 6px;
}

input[type=range] {
    width: 100%;
    accent-color: var(--accent-cyan);
}

.btn-group-segmented {
    display: grid;
    grid-template-columns: repeat(4, 1fr);
    gap: 4px;
    background: #090e17;
    padding: 4px;
    border-radius: 6px;
    border: 1px solid var(--border-color);
}

.btn-seg {
    background: transparent;
    border: none;
    color: var(--text-muted);
    padding: 8px 4px;
    font-size: 11px;
    border-radius: 4px;
    font-family: var(--font-mono);
}

.btn-seg.active {
    background: var(--accent-cyan);
    color: #000;
    font-weight: bold;
}

.quick-actions {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 10px;
    margin-top: 8px;
}

/* Log Panel */
.log-panel {
    gap: 10px;
}

.log-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.log-header h3 {
    font-size: 13px;
    font-family: var(--font-mono);
    color: var(--text-muted);
}

.log-output {
    background: #04060a;
    border: 1px solid var(--border-color);
    border-radius: 8px;
    padding: 12px;
    height: 120px;
    overflow-y: auto;
    font-family: var(--font-mono);
    font-size: 11px;
    display: flex;
    flex-direction: column;
    gap: 4px;
}

.log-entry {
    word-break: break-all;
}

.log-info { color: var(--accent-cyan); }
.log-warn { color: var(--accent-amber); }
.log-err { color: var(--accent-red); }
.log-ok { color: var(--accent-green); }

</style>
</head>
<body>
    <div class="hud-container">
        <!-- Top Navigation Bar -->
        <header class="hud-header">
            <div class="brand">
                <div class="brand-icon">⚔</div>
                <div class="brand-text">
                    <h1>ESP32-RF-SWORD</h1>
                    <span class="badge">v0.1.0</span>
                </div>
            </div>
            <div class="status-indicator">
                <span id="ws-status" class="status-dot disconnected"></span>
                <span id="status-text">DISCONNECTED</span>
                <span class="uptime" id="uptime-display">UP: 00:00:00</span>
            </div>
        </header>

        <!-- Top Metrics Grid -->
        <div class="metrics-grid">
            <div class="metric-card">
                <div class="metric-label">ACTIVE MODE</div>
                <div class="metric-value accent" id="metric-mode">IDLE</div>
            </div>
            <div class="metric-card">
                <div class="metric-label">HOP RATE</div>
                <div class="metric-value" id="metric-hops">0 <small>hops/s</small></div>
            </div>
            <div class="metric-card">
                <div class="metric-label">PACKET RATE</div>
                <div class="metric-value" id="metric-packets">0 <small>pkt/s</small></div>
            </div>
            <div class="metric-card">
                <div class="metric-label">RADIO POWER</div>
                <div class="metric-value" id="metric-power">MAX (0dBm)</div>
            </div>
            <div class="metric-card">
                <div class="metric-label">ACTIVE RADIOS</div>
                <div class="metric-value" id="metric-radios">2x nRF24</div>
            </div>
            <div class="metric-card">
                <div class="metric-label">CORE TEMP / HEAP</div>
                <div class="metric-value" id="metric-health">--°C / -- KB</div>
            </div>
        </div>

        <!-- Main Dashboard Content -->
        <div class="dashboard-body">
            <!-- Left Column: Spectrum & Visualizer -->
            <div class="main-panel">
                <div class="panel-header">
                    <h2>2.4 GHz REAL-TIME SPECTRUM WATERFALL</h2>
                    <div class="panel-actions">
                        <button class="btn btn-sm" id="btn-toggle-view">VIEW: 2D SPECTRUM</button>
                    </div>
                </div>
                <div class="canvas-container">
                    <canvas id="waterfallCanvas" width="800" height="260"></canvas>
                </div>
                <div class="spectrum-labels">
                    <span>2400 MHz (Ch 0)</span>
                    <span>2420 MHz (Ch 20)</span>
                    <span>2440 MHz (Ch 40)</span>
                    <span>2460 MHz (Ch 60)</span>
                    <span>2480 MHz (Ch 80)</span>
                    <span>2525 MHz (Ch 125)</span>
                </div>

                <!-- Radio Channel Indicators -->
                <div class="radio-gauges-container">
                    <div class="radio-gauge" id="gauge-radio-0">
                        <span class="gauge-title">RADIO #1 (A)</span>
                        <div class="gauge-bar-bg"><div class="gauge-bar-fill" style="width: 0%"></div></div>
                        <span class="gauge-info">Ch -- (---- MHz)</span>
                    </div>
                    <div class="radio-gauge" id="gauge-radio-1">
                        <span class="gauge-title">RADIO #2 (B)</span>
                        <div class="gauge-bar-bg"><div class="gauge-bar-fill" style="width: 0%"></div></div>
                        <span class="gauge-info">Ch -- (---- MHz)</span>
                    </div>
                    <div class="radio-gauge" id="gauge-radio-2">
                        <span class="gauge-title">RADIO #3 (C)</span>
                        <div class="gauge-bar-bg"><div class="gauge-bar-fill" style="width: 0%"></div></div>
                        <span class="gauge-info">OFFLINE</span>
                    </div>
                    <div class="radio-gauge" id="gauge-radio-3">
                        <span class="gauge-title">RADIO #4 (D)</span>
                        <div class="gauge-bar-bg"><div class="gauge-bar-fill" style="width: 0%"></div></div>
                        <span class="gauge-info">OFFLINE</span>
                    </div>
                </div>
            </div>

            <!-- Right Column: Control Deck -->
            <div class="control-panel">
                <!-- Master On/Off Switch -->
                <div class="master-switch-box">
                    <button id="btn-master-toggle" class="btn btn-primary btn-master">START ATTACK / SWEEP</button>
                </div>

                <!-- Mode Selection -->
                <div class="control-group">
                    <label>OPERATION / ATTACK MODE</label>
                    <select id="select-mode" class="form-select">
                        <option value="SWEEP_COPRIME">2.4GHz Coprime Interleaved Sweeper (RF-KILL V2+)</option>
                        <option value="SWEEP_LINEAR">2.4GHz High-Speed Linear Sweep</option>
                        <option value="SWEEP_RANDOM">2.4GHz Pseudo-Random Jitter Sweep</option>
                        <option value="TARGETED_BLE">Focused BLE Jammer (Adv 37, 38, 39)</option>
                        <option value="TARGETED_WIFI">Focused Wi-Fi Blocker (Ch 1/6/11)</option>
                        <option value="TARGETED_ZIGBEE">Focused ZigBee / 802.15.4 Jammer</option>
                        <option value="TARGETED_DRONE">Focused Drone RC Hopping Jammer</option>
                        <option value="TARGETED_SUBGHZ">Sub-GHz Jammer (315/433/868/915 MHz)</option>
                        <option value="NOISE_BURST">High-Power Corrupted Packet Blaster</option>
                        <option value="SPECTRUM_ANALYZER">2.4 GHz Spectrum Sniffer & Waterfall</option>
                        <option value="WIFI_BEACON_SPAM">Native 802.11 Beacon SSID Flooder</option>
                        <option value="WIFI_DEAUTH_STORM">Native 802.11 Deauth & Disassoc Storm</option>
                        <option value="BLE_POPUP_SPAM">Native BLE Apple/Android/Win Popup Storm</option>
                        <option value="TOTAL_BLITZ">TOTAL BLITZ (All Radios & Bands Synchronized)</option>
                    </select>
                </div>

                <!-- Target Presets -->
                <div class="control-group">
                    <label>TARGET PRESET</label>
                    <select id="select-preset" class="form-select">
                        <option value="full">Full ISM Band (2402 - 2480 MHz)</option>
                        <option value="ble-adv">Bluetooth Adv (Ch 37, 38, 39)</option>
                        <option value="ble-all">Bluetooth All 40 Channels</option>
                        <option value="wifi-1">Wi-Fi Channel 1 (2412 MHz)</option>
                        <option value="wifi-6">Wi-Fi Channel 6 (2437 MHz)</option>
                        <option value="wifi-11">Wi-Fi Channel 11 (2462 MHz)</option>
                        <option value="wifi-all">Wi-Fi All Channels (1-13)</option>
                        <option value="zigbee">ZigBee / Thread (Ch 11-26)</option>
                        <option value="flysky">FlySky AFHDS RC Drones</option>
                        <option value="frsky">FrSky ACCST RC Drones</option>
                        <option value="elrs">ExpressLRS 2.4GHz</option>
                        <option value="sub-315">Sub-GHz 315.00 MHz</option>
                        <option value="sub-433">Sub-GHz 433.92 MHz</option>
                        <option value="sub-868">Sub-GHz 868.35 MHz</option>
                        <option value="sub-915">Sub-GHz 915.00 MHz</option>
                        <option value="custom">Custom Range...</option>
                    </select>
                </div>

                <!-- Channel Range Slider -->
                <div class="control-group">
                    <div class="range-labels">
                        <label>RF CHANNEL SPAN</label>
                        <span id="channel-span-text">Ch 2 (2402MHz) -> Ch 80 (2480MHz)</span>
                    </div>
                    <div class="dual-range">
                        <input type="range" id="range-min-ch" min="0" max="125" value="2">
                        <input type="range" id="range-max-ch" min="0" max="125" value="80">
                    </div>
                </div>

                <!-- Dwell Timing -->
                <div class="control-group">
                    <div class="range-labels">
                        <label>DWELL TIME JITTER (µs)</label>
                        <span id="dwell-text">120 - 180 µs</span>
                    </div>
                    <div class="dual-range">
                        <input type="range" id="range-min-dwell" min="10" max="2000" step="10" value="120">
                        <input type="range" id="range-max-dwell" min="10" max="2000" step="10" value="180">
                    </div>
                </div>

                <!-- Power Level Selection -->
                <div class="control-group">
                    <label>TX OUTPUT POWER</label>
                    <div class="btn-group-segmented">
                        <button class="btn btn-seg" data-power="MIN">MIN</button>
                        <button class="btn btn-seg" data-power="LOW">LOW</button>
                        <button class="btn btn-seg" data-power="HIGH">HIGH</button>
                        <button class="btn btn-seg active" data-power="MAX">MAX (PA)</button>
                    </div>
                </div>

                <!-- Quick Action Buttons -->
                <div class="quick-actions">
                    <button class="btn btn-secondary" id="btn-save-nvs">SAVE TO FLASH</button>
                    <button class="btn btn-danger" id="btn-reboot">REBOOT ESP32</button>
                </div>
            </div>
        </div>

        <!-- Live Event Log Console -->
        <div class="log-panel">
            <div class="log-header">
                <h3>SYSTEM AUDIT LOG</h3>
                <button class="btn btn-sm" id="btn-clear-log">CLEAR</button>
            </div>
            <div class="log-output" id="log-output">
                <div class="log-entry log-info">[INF][SYS] ESP32-RF-SWORD Web Console Loaded. Connecting WebSocket...</div>
            </div>
        </div>
    </div>

    <script>
/**
 * @file app.js
 * @brief Front-end controller & waterfall visualizer for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 */

let ws = null;
let isRunning = false;
let viewMode2D = false;
const canvas = document.getElementById('waterfallCanvas');
const ctx = canvas.getContext('2d');

// Waterfall image data buffer
const WATERFALL_WIDTH = 128;
const WATERFALL_HEIGHT = 100;
let waterfallHistory = [];

// DOM Elements
const wsStatusDot = document.getElementById('ws-status');
const wsStatusText = document.getElementById('status-text');
const uptimeDisplay = document.getElementById('uptime-display');
const btnMasterToggle = document.getElementById('btn-master-toggle');
const selectMode = document.getElementById('select-mode');
const selectPreset = document.getElementById('select-preset');
const rangeMinCh = document.getElementById('range-min-ch');
const rangeMaxCh = document.getElementById('range-max-ch');
const channelSpanText = document.getElementById('channel-span-text');
const rangeMinDwell = document.getElementById('range-min-dwell');
const rangeMaxDwell = document.getElementById('range-max-dwell');
const dwellText = document.getElementById('dwell-text');
const logOutput = document.getElementById('log-output');

// Metric DOMs
const metricMode = document.getElementById('metric-mode');
const metricHops = document.getElementById('metric-hops');
const metricPackets = document.getElementById('metric-packets');
const metricPower = document.getElementById('metric-power');
const metricRadios = document.getElementById('metric-radios');
const metricHealth = document.getElementById('metric-health');

// Connect WebSocket
function connectWebSocket() {
    const protocol = location.protocol === 'https:' ? 'wss:' : 'ws:';
    const host = location.host || '192.168.4.1';
    ws = new WebSocket(`${protocol}//${host}/ws`);

    ws.onopen = () => {
        wsStatusDot.className = 'status-dot connected';
        wsStatusText.textContent = 'CONNECTED';
        addLog('WebSocket session established.', 'ok');
    };

    ws.onclose = () => {
        wsStatusDot.className = 'status-dot disconnected';
        wsStatusText.textContent = 'DISCONNECTED';
        setTimeout(connectWebSocket, 2000);
    };

    ws.onerror = () => {
        wsStatusDot.className = 'status-dot disconnected';
        wsStatusText.textContent = 'ERROR';
    };

    ws.onmessage = (event) => {
        try {
            const msg = JSON.parse(event.data);
            handleTelemetry(msg);
        } catch (e) {
            console.error('WS Parse Error:', e);
        }
    };
}

function handleTelemetry(data) {
    if (data.type === 'telemetry') {
        const t = data.telemetry;
        isRunning = t.running;
        
        // Update master button
        if (isRunning) {
            btnMasterToggle.textContent = 'STOP ATTACK';
            btnMasterToggle.className = 'btn btn-master active-stop';
        } else {
            btnMasterToggle.textContent = 'START ATTACK / SWEEP';
            btnMasterToggle.className = 'btn btn-primary btn-master';
        }

        // Metrics
        metricMode.textContent = t.mode;
        metricHops.innerHTML = `${t.hopsPerSec.toLocaleString()} <small>hops/s</small>`;
        metricPackets.innerHTML = `${t.packetsPerSec.toLocaleString()} <small>pkt/s</small>`;
        metricPower.textContent = t.power;
        metricRadios.textContent = `${t.radioCount}x Radio(s)`;
        metricHealth.textContent = `${t.tempC.toFixed(1)}°C / ${Math.round(t.freeHeap / 1024)} KB`;

        // Uptime
        const hrs = String(Math.floor(t.uptime / 3600)).padStart(2, '0');
        const mins = String(Math.floor((t.uptime % 3600) / 60)).padStart(2, '0');
        const secs = String(t.uptime % 60).padStart(2, '0');
        uptimeDisplay.textContent = `UP: ${hrs}:${mins}:${secs}`;

        // Update Radios
        if (data.radios) {
            data.radios.forEach((r, idx) => {
                const gauge = document.getElementById(`gauge-radio-${idx}`);
                if (gauge) {
                    const fill = gauge.querySelector('.gauge-bar-fill');
                    const info = gauge.querySelector('.gauge-info');
                    if (r.active) {
                        const pct = (r.channel / 125) * 100;
                        fill.style.width = `${pct}%`;
                        info.textContent = `Ch ${r.channel} (${(2400 + r.channel)} MHz)`;
                    } else if (r.present) {
                        fill.style.width = `0%`;
                        info.textContent = `STANDBY`;
                    } else {
                        fill.style.width = `0%`;
                        info.textContent = `OFFLINE`;
                    }
                }
            });
        }

        // Waterfall / Spectrum data
        if (data.spectrum) {
            drawWaterfallRow(data.spectrum);
        }
    } else if (data.type === 'log') {
        addLog(data.msg, data.level);
    }
}

function drawWaterfallRow(spectrumArray) {
    waterfallHistory.unshift(spectrumArray);
    if (waterfallHistory.length > WATERFALL_HEIGHT) {
        waterfallHistory.pop();
    }
    renderCanvas();
}

function renderCanvas() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    if (viewMode2D) {
        // 2D Spectrum Bar Graph
        if (waterfallHistory.length === 0) return;
        const latest = waterfallHistory[0];
        const barWidth = canvas.width / latest.length;

        for (let i = 0; i < latest.length; i++) {
            const val = latest[i];
            const h = (val / 100) * (canvas.height - 20);
            ctx.fillStyle = val > 50 ? '#ff3366' : '#00f0ff';
            ctx.fillRect(i * barWidth, canvas.height - h, barWidth - 1, h);
        }
    } else {
        // Waterfall Heatmap
        const rowHeight = canvas.height / WATERFALL_HEIGHT;
        const colWidth = canvas.width / WATERFALL_WIDTH;

        for (let y = 0; y < waterfallHistory.length; y++) {
            const row = waterfallHistory[y];
            for (let x = 0; x < row.length; x++) {
                const val = row[x];
                if (val > 0) {
                    ctx.fillStyle = getHeatmapColor(val);
                    ctx.fillRect(x * colWidth, y * rowHeight, colWidth + 1, rowHeight + 1);
                }
            }
        }
    }
}

function getHeatmapColor(val) {
    if (val > 80) return '#ff3366';
    if (val > 50) return '#ffb800';
    if (val > 20) return '#00ff88';
    return '#00f0ff';
}

function addLog(msg, level = 'info') {
    const entry = document.createElement('div');
    entry.className = `log-entry log-${level}`;
    const time = new Date().toLocaleTimeString();
    entry.textContent = `[${time}] ${msg}`;
    logOutput.appendChild(entry);
    logOutput.scrollTop = logOutput.scrollHeight;
}

// Event Listeners
btnMasterToggle.addEventListener('click', () => {
    sendAction('toggle_run', { run: !isRunning });
});

selectMode.addEventListener('change', () => {
    sendAction('set_mode', { mode: selectMode.value });
});

selectPreset.addEventListener('change', () => {
    sendAction('set_preset', { preset: selectPreset.value });
});

rangeMinCh.addEventListener('input', updateChannels);
rangeMaxCh.addEventListener('input', updateChannels);
function updateChannels() {
    let min = parseInt(rangeMinCh.value);
    let max = parseInt(rangeMaxCh.value);
    if (min > max) { min = max; rangeMinCh.value = min; }
    channelSpanText.textContent = `Ch ${min} (${2400 + min}MHz) -> Ch ${max} (${2400 + max}MHz)`;
    sendAction('set_channels', { min, max });
}

rangeMinDwell.addEventListener('input', updateDwell);
rangeMaxDwell.addEventListener('input', updateDwell);
function updateDwell() {
    let min = parseInt(rangeMinDwell.value);
    let max = parseInt(rangeMaxDwell.value);
    if (min > max) { min = max; rangeMinDwell.value = min; }
    dwellText.textContent = `${min} - ${max} µs`;
    sendAction('set_dwell', { min, max });
}

document.querySelectorAll('.btn-seg').forEach(btn => {
    btn.addEventListener('click', () => {
        document.querySelectorAll('.btn-seg').forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        sendAction('set_power', { power: btn.dataset.power });
    });
});

document.getElementById('btn-toggle-view').addEventListener('click', () => {
    viewMode2D = !viewMode2D;
    document.getElementById('btn-toggle-view').textContent = viewMode2D ? 'VIEW: WATERFALL' : 'VIEW: 2D SPECTRUM';
});

document.getElementById('btn-clear-log').addEventListener('click', () => {
    logOutput.innerHTML = '';
});

document.getElementById('btn-save-nvs').addEventListener('click', () => {
    sendAction('save_config', {});
    addLog('Configuration saved to Non-Volatile Flash Memory.', 'ok');
});

document.getElementById('btn-reboot').addEventListener('click', () => {
    if (confirm('Reboot ESP32-RF-SWORD now?')) {
        sendAction('reboot', {});
        addLog('Reboot command sent...', 'warn');
    }
});

function sendAction(action, payload) {
    if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ action, ...payload }));
    }
}

// Initialise
window.addEventListener('load', () => {
    connectWebSocket();
});

</script>
</body>
</html>

)rawliteral";

inline const char* getIndexHtml() {
    return INDEX_HTML;
}

} // namespace WebAssets

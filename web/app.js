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

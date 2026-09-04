# Contributing to ESP32-RF-SWORD

We welcome contributions from the RF research, embedded systems, and security communities!

---

## Development Workflow

1. **Fork and Clone** the repository:
   ```bash
   git clone https://github.com/your-username/esp32-rf-sword.git
   cd esp32-rf-sword
   ```

2. **Install PlatformIO Core / IDE**:
   - [PlatformIO CLI / VSCode Extension](https://platformio.org/)

3. **Run Unit Tests**:
   - Verify native unit tests before submitting PRs:
   ```bash
   clang++ -std=c++17 -DHOST_SIMULATION -Iinclude -Isrc -Itest \
     test/test_main.cpp test/test_channel_math.cpp test/test_presets.cpp \
     test/test_waterfall.cpp test/test_system_state.cpp \
     src/radio/ChannelMath.cpp src/attack/Presets.cpp \
     src/analyzer/WaterfallBuffer.cpp src/core/SystemState.cpp \
     -o /tmp/sword_test && /tmp/sword_test
   ```

4. **Web UI Development**:
   - Edit files under `web/` (`index.html`, `style.css`, `app.js`).
   - Run the bundler script to generate `include/ui/WebAssets.h`:
   ```bash
   python3 scripts/bundle_web.py
   ```

5. **Code Style**:
   - C++17 / C++20 standard compliance.
   - Clean modular architecture under `include/` and `src/`.
   - Thread safety: Core 1 RF tasks must remain non-blocking. Core 0 handles UI and networking.
   - Meaningful Doxygen comments on all public interfaces.

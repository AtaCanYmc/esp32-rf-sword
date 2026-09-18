<!-- well-document · audit scorecard: G1:5 G2:5 G3:5 G4:5 G5:5 G6:5 · STATUS: PASS -->

# Contributing to ESP32-RF-SWORD

Contributions to ESP32-RF-SWORD are welcome. To maintain code quality, deterministic timing, and automated releases, all pull requests must satisfy the guidelines below.

---

## Table of Contents

- [Commit Message Specification](#commit-message-specification)
- [Branch Naming Conventions](#branch-naming-conventions)
- [Local Development and Verification](#local-development-and-verification)
- [Architectural and Threading Rules](#architectural-and-threading-rules)
- [Pull Request Checklist](#pull-request-checklist)

---

## Commit Message Specification

This repository uses [Release-Please](https://github.com/googleapis/release-please) to parse commit history, calculate semantic version tags, and generate `CHANGELOG.md`. All commit messages must follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:

```text
<type>(<optional scope>): <description>

[optional body]

[optional footer(s)]
```

### Allowed Types

| Type | SemVer Impact | Description |
| :--- | :--- | :--- |
| `feat` | MINOR | Introduces a new feature or public capability. |
| `fix` | PATCH | Fixes a bug or unexpected behavior. |
| `docs` | None | Documentation updates or corrections. |
| `refactor`| None | Code changes that neither fix a bug nor add a feature. |
| `test` | None | Adding or correcting automated tests. |
| `ci` | None | Changes to CI workflows, build scripts, or pipelines. |
| `chore` | None | Tooling, dependency bumps, or repository governance. |

Breaking changes must include `BREAKING CHANGE:` in the commit footer or append an exclamation mark (`!`) after the type or scope (e.g. `feat(radio)!: redesign HAL interface`).

---

## Branch Naming Conventions

Prefix branch names with the corresponding contribution category:

- `feat/<topic>` (e.g. `feat/esp32-c6-ieee802154`)
- `fix/<issue>` (e.g. `fix/spi-miso-float-condition`)
- `docs/<topic>` (e.g. `docs/subghz-calibration-table`)
- `refactor/<target>` (e.g. `refactor/channel-math-simplification`)

---

## Local Development and Verification

Before submitting a pull request, run the verification pipeline locally:

### 1. Host-Side Unit Tests

Verify core channel math, presets, and circular buffer lifecycle using the host test runner:

```bash
clang++ -std=c++17 -DHOST_SIMULATION -Iinclude -Isrc -Itest \
  test/test_main.cpp test/test_channel_math.cpp test/test_presets.cpp \
  test/test_waterfall.cpp test/test_system_state.cpp \
  src/radio/ChannelMath.cpp src/attack/Presets.cpp \
  src/analyzer/WaterfallBuffer.cpp src/core/SystemState.cpp \
  -o /tmp/sword_test && /tmp/sword_test
```

### 2. Web Asset Bundling Synchronization

When modifying HTML, CSS, or JavaScript assets in `web/`, rebuild `include/ui/WebAssets.h` and verify a clean git status:

```bash
python3 scripts/bundle_web.py
git diff --exit-code include/ui/WebAssets.h
```

### 3. Target Firmware Compilation

Ensure targeted microcontroller environments compile cleanly via PlatformIO:

```bash
pio run -e esp32-c3-supermini
pio run -e esp32-devkit-v1
pio run -e esp32-s3-devkit
```

---

## Architectural and Threading Rules

Firmware modifications must respect FreeRTOS task boundaries:

1. **Core 1 RF Isolation**:
   - The radio transmission loop executes on Core 1 at priority 24.
   - Code executed on Core 1 must be non-blocking. Never invoke blocking delays (`delay()`, long `vTaskDelay()`), dynamic heap allocations (`malloc`, `new`), or synchronous file/flash writes within the transmission loop.
2. **Core 0 System Concurrency**:
   - Network services (Wi-Fi, HTTP, WebSockets), serial CLI processing, OLED HUD refresh, and NVS flash writes reside on Core 0.
3. **Cross-Core Data Integrity**:
   - All state mutations between cores must occur through atomic types (`std::atomic`) or designated mutex guards (`SPIManager`, `SystemState`).
4. **Code Standard**:
   - C++17 language compliance.
   - Do not introduce external libraries without prior maintainer discussion.

---

## Pull Request Checklist

Ensure all items are satisfied prior to review:

- [ ] Commit history adheres to Conventional Commits format.
- [ ] Host-side unit tests compile and pass without failures.
- [ ] Modified `web/` assets have been re-bundled using `scripts/bundle_web.py`.
- [ ] PlatformIO builds pass without errors or unhandled compiler warnings.
- [ ] No hardcoded GPIO pin numbers; all hardware lines reference `BoardConfig.h`.
- [ ] Concurrency rules respected: Core 1 RF routines remain non-blocking.

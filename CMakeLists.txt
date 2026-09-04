cmake_minimum_required(VERSION 3.16)
project(esp32-rf-sword C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    include
    src
    test
)

# Host test executable target
add_executable(sword_unit_tests
    test/test_main.cpp
    test/test_channel_math.cpp
    test/test_presets.cpp
    test/test_waterfall.cpp
    test/test_system_state.cpp
    src/radio/ChannelMath.cpp
    src/attack/Presets.cpp
    src/analyzer/WaterfallBuffer.cpp
    src/core/SystemState.cpp
)

target_compile_definitions(sword_unit_tests PRIVATE HOST_SIMULATION=1)

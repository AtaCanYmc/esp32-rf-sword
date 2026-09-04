/**
 * @file unity.h
 * @brief Self-contained lightweight test assertions for ESP32-RF-SWORD
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

static int g_tests_run = 0;
static int g_tests_failed = 0;

inline void UNITY_BEGIN() {
    g_tests_run = 0;
    g_tests_failed = 0;
    printf("\n======================= TEST RUNNER =======================\n");
}

inline int UNITY_END() {
    printf("-----------------------------------------------------------\n");
    printf("RESULTS: %d Ran, %d Failed, %d Passed\n", g_tests_run, g_tests_failed, g_tests_run - g_tests_failed);
    printf("===========================================================\n\n");
    return g_tests_failed > 0 ? 1 : 0;
}

#define RUN_TEST(func) do { \
    g_tests_run++; \
    printf("[ RUN      ] " #func "\n"); \
    int prev_failed = g_tests_failed; \
    func(); \
    if (g_tests_failed == prev_failed) { \
        printf("[       OK ] " #func "\n"); \
    } else { \
        printf("[  FAILED  ] " #func "\n"); \
    } \
} while(0)

#define TEST_ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("  FAIL: %s:%d: Condition (" #cond ") is FALSE\n", __FILE__, __LINE__); \
        g_tests_failed++; \
    } \
} while(0)

#define TEST_ASSERT_FALSE(cond) do { \
    if ((cond)) { \
        printf("  FAIL: %s:%d: Condition (" #cond ") is TRUE\n", __FILE__, __LINE__); \
        g_tests_failed++; \
    } \
} while(0)

#define TEST_ASSERT_EQUAL_UINT32(expected, actual) do { \
    uint32_t e = (expected); \
    uint32_t a = (actual); \
    if (e != a) { \
        printf("  FAIL: %s:%d: Expected %u, got %u\n", __FILE__, __LINE__, e, a); \
        g_tests_failed++; \
    } \
} while(0)

#define TEST_ASSERT_EQUAL_UINT8(expected, actual) do { \
    uint8_t e = (expected); \
    uint8_t a = (actual); \
    if (e != a) { \
        printf("  FAIL: %s:%d: Expected %u, got %u\n", __FILE__, __LINE__, e, a); \
        g_tests_failed++; \
    } \
} while(0)

#define TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual) do { \
    float diff = fabsf((float)(expected) - (float)(actual)); \
    if (diff > (delta)) { \
        printf("  FAIL: %s:%d: Expected %f, got %f (diff %f > %f)\n", __FILE__, __LINE__, (float)(expected), (float)(actual), diff, (float)(delta)); \
        g_tests_failed++; \
    } \
} while(0)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) do { \
    const char* e = (expected); \
    const char* a = (actual); \
    if (strcmp(e, a) != 0) { \
        printf("  FAIL: %s:%d: Expected '%s', got '%s'\n", __FILE__, __LINE__, e, a); \
        g_tests_failed++; \
    } \
} while(0)

#ifdef __cplusplus
}
#endif

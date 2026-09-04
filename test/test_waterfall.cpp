/**
 * @file test_waterfall.cpp
 * @brief Unit tests for Waterfall circular buffer
 */

#include "unity.h"
#include "analyzer/WaterfallBuffer.h"

void test_waterfall_buffer_lifecycle() {
    WaterfallBuffer wb;
    TEST_ASSERT_EQUAL_UINT32(0, wb.getRowCount());

    uint8_t row1[128];
    memset(row1, 55, sizeof(row1));
    wb.addScanRow(row1, sizeof(row1));

    TEST_ASSERT_EQUAL_UINT32(1, wb.getRowCount());
    const uint8_t* r = wb.getRow(0);
    TEST_ASSERT_TRUE(r != nullptr);
    TEST_ASSERT_EQUAL_UINT8(55, r[0]);
    TEST_ASSERT_EQUAL_UINT8(55, r[127]);

    uint8_t latest[128];
    wb.getLatestRow(latest);
    TEST_ASSERT_EQUAL_UINT8(55, latest[0]);
}

void test_waterfall_overflow() {
    WaterfallBuffer wb;
    uint8_t row[128];

    // Push 40 rows (capacity is 32)
    for (int i = 0; i < 40; i++) {
        memset(row, (uint8_t)i, sizeof(row));
        wb.addScanRow(row, sizeof(row));
    }

    TEST_ASSERT_EQUAL_UINT32(32, wb.getRowCount());
    
    // Latest row should be 39
    uint8_t latest[128];
    wb.getLatestRow(latest);
    TEST_ASSERT_EQUAL_UINT8(39, latest[0]);
}

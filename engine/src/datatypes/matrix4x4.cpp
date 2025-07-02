#include "datatypes/matrix4x4.h"
#include "datatypes/float3.h"

#include "utility/defines.h"

#include "utility/managed_memory_block.h"

using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 50;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.7f;
    using memblock_t = managed_sequential_memory_block<float, matrix4x4::CELL_COUNT, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>;
    static memblock_t gMemoryBlock;
}

matrix4x4::id_t matrix4x4::create(MATRIX4X4_DECLARE_PARAMS(v)) {
    return gMemoryBlock.push({ MATRIX4X4_FORWARD_PARAMS(v) });
}
matrix4x4::id_t matrix4x4::create(const values& vals) {
    return gMemoryBlock.push({ MATRIX4X4_UNPACK(vals) });
}

bool matrix4x4::release(id_t id) {
    return gMemoryBlock.try_remove(id);
}

bool matrix4x4::is_valid(id_t id) {
    return gMemoryBlock.is_valid(id);
}

bool matrix4x4::try_get_values(id_t id, matrix4x4::values& vals) {
    if (is_valid(id)) {
        vals = {
            gMemoryBlock.get(id, R1C1), gMemoryBlock.get(id, R1C2), gMemoryBlock.get(id, R1C3), gMemoryBlock.get(id, R1C4),
            gMemoryBlock.get(id, R2C1), gMemoryBlock.get(id, R2C2), gMemoryBlock.get(id, R2C3), gMemoryBlock.get(id, R2C4),
            gMemoryBlock.get(id, R3C1), gMemoryBlock.get(id, R3C2), gMemoryBlock.get(id, R3C3), gMemoryBlock.get(id, R3C4),
            gMemoryBlock.get(id, R4C1), gMemoryBlock.get(id, R4C2), gMemoryBlock.get(id, R4C3), gMemoryBlock.get(id, R4C4)
        };
        return true;
    }
    return false;
}

float& matrix4x4::cell(id_t id, int index) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Element offset is out of range.");
    return gMemoryBlock.get(id, index);
}
float& matrix4x4::cell(id_t id, int row, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return gMemoryBlock.get(id, row * COLUMN_COUNT + column);
}

float4::values matrix4x4::row(id_t id, int row) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        gMemoryBlock.get(id, row * COLUMN_COUNT + 0),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 1),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 2),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 3)
    };
}

float4::values matrix4x4::column(id_t id, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return {
        gMemoryBlock.get(id, 0 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 1 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 2 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 3 * COLUMN_COUNT + column)
    };
}

const float& matrix4x4::const_cell(id_t id, int index) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Element offset is out of range.");
    return gMemoryBlock.get(id, index);
}
const float& matrix4x4::const_cell(id_t id, int row, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return gMemoryBlock.get(id, row * COLUMN_COUNT + column);
}

const float4::values matrix4x4::const_row(id_t id, int row) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        gMemoryBlock.get(id, row * COLUMN_COUNT + 0),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 1),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 2),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 3)
    };
}

const float4::values matrix4x4::const_column(id_t id, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return {
        gMemoryBlock.get(id, 0 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 1 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 2 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 3 * COLUMN_COUNT + column)
    };
}


///////////////////////////////////////////////////////
// Values container

float4::values matrix4x4::values::row(int row) {
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        mCells[row * COLUMN_COUNT + 0],
        mCells[row * COLUMN_COUNT + 1],
        mCells[row * COLUMN_COUNT + 2],
        mCells[row * COLUMN_COUNT + 3]
    };
}

float4::values matrix4x4::values::column(int column) {
    ELOO_ASSERT(column >= 0 && column < 2, "Column index is out of range.");
    return {
        mCells[0 * COLUMN_COUNT + column],
        mCells[1 * COLUMN_COUNT + column],
        mCells[2 * COLUMN_COUNT + column],
        mCells[3 * COLUMN_COUNT + column]
    };
}

const float4::values matrix4x4::values::row(int row) const {
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        mCells[row * COLUMN_COUNT + 0],
        mCells[row * COLUMN_COUNT + 1],
        mCells[row * COLUMN_COUNT + 2],
        mCells[row * COLUMN_COUNT + 3]
    };
}

const float4::values matrix4x4::values::column(int column) const {
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return {
        mCells[0 * COLUMN_COUNT + column],
        mCells[1 * COLUMN_COUNT + column],
        mCells[2 * COLUMN_COUNT + column],
        mCells[3 * COLUMN_COUNT + column]
    };
}

float& matrix4x4::values::cell(int index) {
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Index is out of range.");
    return mCells[index];
}

const float& matrix4x4::values::cell(int index) const {
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Index is out of range.");
    return mCells[index];
}

bool operator != (const matrix4x4::values& lhs, const matrix4x4::values& rhs) {
    return
        lhs[matrix4x4::R1C1] != rhs[matrix4x4::R1C1] || lhs[matrix4x4::R1C2] != rhs[matrix4x4::R1C2] || lhs[matrix4x4::R1C3] != rhs[matrix4x4::R1C3] || lhs[matrix4x4::R1C4] != rhs[matrix4x4::R1C4] ||
        lhs[matrix4x4::R2C1] != rhs[matrix4x4::R2C1] || lhs[matrix4x4::R2C2] != rhs[matrix4x4::R2C2] || lhs[matrix4x4::R2C3] != rhs[matrix4x4::R2C3] || lhs[matrix4x4::R2C4] != rhs[matrix4x4::R2C4] ||
        lhs[matrix4x4::R3C1] != rhs[matrix4x4::R3C1] || lhs[matrix4x4::R3C2] != rhs[matrix4x4::R3C2] || lhs[matrix4x4::R3C3] != rhs[matrix4x4::R3C3] || lhs[matrix4x4::R3C4] != rhs[matrix4x4::R3C4] ||
        lhs[matrix4x4::R4C1] != rhs[matrix4x4::R4C1] || lhs[matrix4x4::R4C2] != rhs[matrix4x4::R4C2] || lhs[matrix4x4::R4C3] != rhs[matrix4x4::R4C3] || lhs[matrix4x4::R4C4] != rhs[matrix4x4::R4C4];
}
bool operator != (const matrix4x4::values& lhs, float rhs) {
    return
        lhs[matrix4x4::R1C1] != rhs || lhs[matrix4x4::R1C2] != rhs || lhs[matrix4x4::R1C3] != rhs || lhs[matrix4x4::R1C4] != rhs ||
        lhs[matrix4x4::R2C1] != rhs || lhs[matrix4x4::R2C2] != rhs || lhs[matrix4x4::R2C3] != rhs || lhs[matrix4x4::R2C4] != rhs ||
        lhs[matrix4x4::R3C1] != rhs || lhs[matrix4x4::R3C2] != rhs || lhs[matrix4x4::R3C3] != rhs || lhs[matrix4x4::R3C4] != rhs ||
        lhs[matrix4x4::R4C1] != rhs || lhs[matrix4x4::R4C2] != rhs || lhs[matrix4x4::R4C3] != rhs || lhs[matrix4x4::R4C4] != rhs;
}

bool operator == (const matrix4x4::values& lhs, const matrix4x4::values& rhs) {
    return
        lhs[matrix4x4::R1C1] == rhs[matrix4x4::R1C1] && lhs[matrix4x4::R1C2] == rhs[matrix4x4::R1C2] && lhs[matrix4x4::R1C3] == rhs[matrix4x4::R1C3] && lhs[matrix4x4::R1C4] == rhs[matrix4x4::R1C4] &&
        lhs[matrix4x4::R2C1] == rhs[matrix4x4::R2C1] && lhs[matrix4x4::R2C2] == rhs[matrix4x4::R2C2] && lhs[matrix4x4::R2C3] == rhs[matrix4x4::R2C3] && lhs[matrix4x4::R2C4] == rhs[matrix4x4::R2C4] &&
        lhs[matrix4x4::R3C1] == rhs[matrix4x4::R3C1] && lhs[matrix4x4::R3C2] == rhs[matrix4x4::R3C2] && lhs[matrix4x4::R3C3] == rhs[matrix4x4::R3C3] && lhs[matrix4x4::R3C4] == rhs[matrix4x4::R3C4] &&
        lhs[matrix4x4::R4C1] == rhs[matrix4x4::R4C1] && lhs[matrix4x4::R4C2] == rhs[matrix4x4::R4C2] && lhs[matrix4x4::R4C3] == rhs[matrix4x4::R4C3] && lhs[matrix4x4::R4C4] == rhs[matrix4x4::R4C4];
}
bool operator == (const matrix4x4::values& lhs, float rhs) {
    return
        lhs[matrix4x4::R1C1] == rhs && lhs[matrix4x4::R1C2] == rhs && lhs[matrix4x4::R1C3] == rhs && lhs[matrix4x4::R1C4] == rhs &&
        lhs[matrix4x4::R2C1] == rhs && lhs[matrix4x4::R2C2] == rhs && lhs[matrix4x4::R2C3] == rhs && lhs[matrix4x4::R2C4] == rhs &&
        lhs[matrix4x4::R3C1] == rhs && lhs[matrix4x4::R3C2] == rhs && lhs[matrix4x4::R3C3] == rhs && lhs[matrix4x4::R3C4] == rhs &&
        lhs[matrix4x4::R4C1] == rhs && lhs[matrix4x4::R4C2] == rhs && lhs[matrix4x4::R4C3] == rhs && lhs[matrix4x4::R4C4] == rhs;
}

matrix4x4::values operator + (const matrix4x4::values& lhs) {
    return lhs;
}

matrix4x4::values operator - (const matrix4x4::values& lhs) {
    return {
        -lhs[matrix4x4::R1C1], -lhs[matrix4x4::R1C2], -lhs[matrix4x4::R1C3], -lhs[matrix4x4::R1C4],
        -lhs[matrix4x4::R2C1], -lhs[matrix4x4::R2C2], -lhs[matrix4x4::R2C3], -lhs[matrix4x4::R2C4],
        -lhs[matrix4x4::R3C1], -lhs[matrix4x4::R3C2], -lhs[matrix4x4::R3C3], -lhs[matrix4x4::R3C4],
        -lhs[matrix4x4::R4C1], -lhs[matrix4x4::R4C2], -lhs[matrix4x4::R4C3], -lhs[matrix4x4::R4C4]
    };
}

matrix4x4::values operator / (const matrix4x4::values& lhs, const matrix4x4::values& rhs) {
    return {
        lhs[matrix4x4::R1C1] / rhs[matrix4x4::R1C1], lhs[matrix4x4::R1C2] / rhs[matrix4x4::R1C2], lhs[matrix4x4::R1C3] / rhs[matrix4x4::R1C3], lhs[matrix4x4::R1C4] / rhs[matrix4x4::R1C4],
        lhs[matrix4x4::R2C1] / rhs[matrix4x4::R2C1], lhs[matrix4x4::R2C2] / rhs[matrix4x4::R2C2], lhs[matrix4x4::R2C3] / rhs[matrix4x4::R2C3], lhs[matrix4x4::R2C4] / rhs[matrix4x4::R2C4],
        lhs[matrix4x4::R3C1] / rhs[matrix4x4::R3C1], lhs[matrix4x4::R3C2] / rhs[matrix4x4::R3C2], lhs[matrix4x4::R3C3] / rhs[matrix4x4::R3C3], lhs[matrix4x4::R3C4] / rhs[matrix4x4::R3C4],
        lhs[matrix4x4::R4C1] / rhs[matrix4x4::R4C1], lhs[matrix4x4::R4C2] / rhs[matrix4x4::R4C2], lhs[matrix4x4::R4C3] / rhs[matrix4x4::R4C3], lhs[matrix4x4::R4C4] / rhs[matrix4x4::R4C4]
    };
}
matrix4x4::values operator / (const matrix4x4::values& lhs, float rhs) {
    return {
        lhs[matrix4x4::R1C1] / rhs, lhs[matrix4x4::R1C2] / rhs, lhs[matrix4x4::R1C3] / rhs, lhs[matrix4x4::R1C4] / rhs,
        lhs[matrix4x4::R2C1] / rhs, lhs[matrix4x4::R2C2] / rhs, lhs[matrix4x4::R2C3] / rhs, lhs[matrix4x4::R2C4] / rhs,
        lhs[matrix4x4::R3C1] / rhs, lhs[matrix4x4::R3C2] / rhs, lhs[matrix4x4::R3C3] / rhs, lhs[matrix4x4::R3C4] / rhs,
        lhs[matrix4x4::R4C1] / rhs, lhs[matrix4x4::R4C2] / rhs, lhs[matrix4x4::R4C3] / rhs, lhs[matrix4x4::R4C4] / rhs
    };
}

matrix4x4::values operator * (const matrix4x4::values& lhs, const matrix4x4::values& rhs) {
    return {
        // row 1
        lhs[matrix4x4::R1C1] * rhs[matrix4x4::R1C1] + lhs[matrix4x4::R1C2] * rhs[matrix4x4::R2C1] + lhs[matrix4x4::R1C3] * rhs[matrix4x4::R3C1] + lhs[matrix4x4::R1C4] * rhs[matrix4x4::R4C1],
        lhs[matrix4x4::R1C1] * rhs[matrix4x4::R1C2] + lhs[matrix4x4::R1C2] * rhs[matrix4x4::R2C2] + lhs[matrix4x4::R1C3] * rhs[matrix4x4::R3C2] + lhs[matrix4x4::R1C4] * rhs[matrix4x4::R4C2],
        lhs[matrix4x4::R1C1] * rhs[matrix4x4::R1C3] + lhs[matrix4x4::R1C2] * rhs[matrix4x4::R2C3] + lhs[matrix4x4::R1C3] * rhs[matrix4x4::R3C3] + lhs[matrix4x4::R1C4] * rhs[matrix4x4::R4C3],
        lhs[matrix4x4::R1C1] * rhs[matrix4x4::R1C4] + lhs[matrix4x4::R1C2] * rhs[matrix4x4::R2C4] + lhs[matrix4x4::R1C3] * rhs[matrix4x4::R3C4] + lhs[matrix4x4::R1C4] * rhs[matrix4x4::R4C4],
        // row 2
        lhs[matrix4x4::R2C1] * rhs[matrix4x4::R1C1] + lhs[matrix4x4::R2C2] * rhs[matrix4x4::R2C1] + lhs[matrix4x4::R2C3] * rhs[matrix4x4::R3C1] + lhs[matrix4x4::R2C4] * rhs[matrix4x4::R4C1],
        lhs[matrix4x4::R2C1] * rhs[matrix4x4::R1C2] + lhs[matrix4x4::R2C2] * rhs[matrix4x4::R2C2] + lhs[matrix4x4::R2C3] * rhs[matrix4x4::R3C2] + lhs[matrix4x4::R2C4] * rhs[matrix4x4::R4C2],
        lhs[matrix4x4::R2C1] * rhs[matrix4x4::R1C3] + lhs[matrix4x4::R2C2] * rhs[matrix4x4::R2C3] + lhs[matrix4x4::R2C3] * rhs[matrix4x4::R3C3] + lhs[matrix4x4::R2C4] * rhs[matrix4x4::R4C3],
        lhs[matrix4x4::R2C1] * rhs[matrix4x4::R1C4] + lhs[matrix4x4::R2C2] * rhs[matrix4x4::R2C4] + lhs[matrix4x4::R2C3] * rhs[matrix4x4::R3C4] + lhs[matrix4x4::R2C4] * rhs[matrix4x4::R4C4],
        // row 3
        lhs[matrix4x4::R3C1] * rhs[matrix4x4::R1C1] + lhs[matrix4x4::R3C2] * rhs[matrix4x4::R2C1] + lhs[matrix4x4::R3C3] * rhs[matrix4x4::R3C1] + lhs[matrix4x4::R3C4] * rhs[matrix4x4::R4C1],
        lhs[matrix4x4::R3C1] * rhs[matrix4x4::R1C2] + lhs[matrix4x4::R3C2] * rhs[matrix4x4::R2C2] + lhs[matrix4x4::R3C3] * rhs[matrix4x4::R3C2] + lhs[matrix4x4::R3C4] * rhs[matrix4x4::R4C2],
        lhs[matrix4x4::R3C1] * rhs[matrix4x4::R1C3] + lhs[matrix4x4::R3C2] * rhs[matrix4x4::R2C3] + lhs[matrix4x4::R3C3] * rhs[matrix4x4::R3C3] + lhs[matrix4x4::R3C4] * rhs[matrix4x4::R4C3],
        lhs[matrix4x4::R3C1] * rhs[matrix4x4::R1C4] + lhs[matrix4x4::R3C2] * rhs[matrix4x4::R2C4] + lhs[matrix4x4::R3C3] * rhs[matrix4x4::R3C4] + lhs[matrix4x4::R3C4] * rhs[matrix4x4::R4C4],
        // row 4
        lhs[matrix4x4::R4C1] * rhs[matrix4x4::R1C1] + lhs[matrix4x4::R4C2] * rhs[matrix4x4::R2C1] + lhs[matrix4x4::R4C3] * rhs[matrix4x4::R3C1] + lhs[matrix4x4::R4C4] * rhs[matrix4x4::R4C1],
        lhs[matrix4x4::R4C1] * rhs[matrix4x4::R1C2] + lhs[matrix4x4::R4C2] * rhs[matrix4x4::R2C2] + lhs[matrix4x4::R4C3] * rhs[matrix4x4::R3C2] + lhs[matrix4x4::R4C4] * rhs[matrix4x4::R4C2],
        lhs[matrix4x4::R4C1] * rhs[matrix4x4::R1C3] + lhs[matrix4x4::R4C2] * rhs[matrix4x4::R2C3] + lhs[matrix4x4::R4C3] * rhs[matrix4x4::R3C3] + lhs[matrix4x4::R4C4] * rhs[matrix4x4::R4C3],
        lhs[matrix4x4::R4C1] * rhs[matrix4x4::R1C4] + lhs[matrix4x4::R4C2] * rhs[matrix4x4::R2C4] + lhs[matrix4x4::R4C3] * rhs[matrix4x4::R3C4] + lhs[matrix4x4::R4C4] * rhs[matrix4x4::R4C4]
    };
}
matrix4x4::values operator * (const matrix4x4::values& lhs, float rhs) {
    return {
        lhs[matrix4x4::R1C1]* rhs, lhs[matrix4x4::R1C2]* rhs, lhs[matrix4x4::R1C3]* rhs, lhs[matrix4x4::R1C4]* rhs,
        lhs[matrix4x4::R2C1]* rhs, lhs[matrix4x4::R2C2]* rhs, lhs[matrix4x4::R2C3]* rhs, lhs[matrix4x4::R2C4]* rhs,
        lhs[matrix4x4::R3C1]* rhs, lhs[matrix4x4::R3C2]* rhs, lhs[matrix4x4::R3C3]* rhs, lhs[matrix4x4::R3C4]* rhs,
        lhs[matrix4x4::R4C1]* rhs, lhs[matrix4x4::R4C2]* rhs, lhs[matrix4x4::R4C3]* rhs, lhs[matrix4x4::R4C4]* rhs
    };
}
float3::values operator * (const matrix4x4::values& lhs, const float3::values& rhs) {
    return {
        lhs[matrix4x4::R1C1] * rhs.x() + lhs[matrix4x4::R1C2] * rhs.y() + lhs[matrix4x4::R1C3] * rhs.z() + lhs[matrix4x4::R1C4],
        lhs[matrix4x4::R2C1] * rhs.x() + lhs[matrix4x4::R2C2] * rhs.y() + lhs[matrix4x4::R2C3] * rhs.z() + lhs[matrix4x4::R2C4],
        lhs[matrix4x4::R3C1] * rhs.x() + lhs[matrix4x4::R3C2] * rhs.y() + lhs[matrix4x4::R3C3] * rhs.z() + lhs[matrix4x4::R3C4]
    };
}
float4::values operator * (const matrix4x4::values& lhs, const float4::values& rhs) {
    return {
        lhs[matrix4x4::R1C1]* rhs.x() + lhs[matrix4x4::R1C2] * rhs.y() + lhs[matrix4x4::R1C3] * rhs.z() + lhs[matrix4x4::R1C4] * rhs.w(),
        lhs[matrix4x4::R2C1]* rhs.x() + lhs[matrix4x4::R2C2] * rhs.y() + lhs[matrix4x4::R2C3] * rhs.z() + lhs[matrix4x4::R2C4] * rhs.w(),
        lhs[matrix4x4::R3C1]* rhs.x() + lhs[matrix4x4::R3C2] * rhs.y() + lhs[matrix4x4::R3C3] * rhs.z() + lhs[matrix4x4::R3C4] * rhs.w(),
        lhs[matrix4x4::R4C1]* rhs.x() + lhs[matrix4x4::R4C2] * rhs.y() + lhs[matrix4x4::R4C3] * rhs.z() + lhs[matrix4x4::R4C4] * rhs.w()
    };
}

matrix4x4::values operator + (const matrix4x4::values& lhs, const matrix4x4::values& rhs) {
    return {
        lhs[matrix4x4::R1C1] + rhs[matrix4x4::R1C1], lhs[matrix4x4::R1C2] + rhs[matrix4x4::R1C2], lhs[matrix4x4::R1C3] + rhs[matrix4x4::R1C3], lhs[matrix4x4::R1C4] + rhs[matrix4x4::R1C4],
        lhs[matrix4x4::R2C1] + rhs[matrix4x4::R2C1], lhs[matrix4x4::R2C2] + rhs[matrix4x4::R2C2], lhs[matrix4x4::R2C3] + rhs[matrix4x4::R2C3], lhs[matrix4x4::R2C4] + rhs[matrix4x4::R2C4],
        lhs[matrix4x4::R3C1] + rhs[matrix4x4::R3C1], lhs[matrix4x4::R3C2] + rhs[matrix4x4::R3C2], lhs[matrix4x4::R3C3] + rhs[matrix4x4::R3C3], lhs[matrix4x4::R3C4] + rhs[matrix4x4::R3C4],
        lhs[matrix4x4::R4C1] + rhs[matrix4x4::R4C1], lhs[matrix4x4::R4C2] + rhs[matrix4x4::R4C2], lhs[matrix4x4::R4C3] + rhs[matrix4x4::R4C3], lhs[matrix4x4::R4C4] + rhs[matrix4x4::R4C4]
    };
}
matrix4x4::values operator + (const matrix4x4::values& lhs, float rhs) {
    return {
        lhs[matrix4x4::R1C1] + rhs, lhs[matrix4x4::R1C2] + rhs, lhs[matrix4x4::R1C3] + rhs, lhs[matrix4x4::R1C4] + rhs,
        lhs[matrix4x4::R2C1] + rhs, lhs[matrix4x4::R2C2] + rhs, lhs[matrix4x4::R2C3] + rhs, lhs[matrix4x4::R2C4] + rhs,
        lhs[matrix4x4::R3C1] + rhs, lhs[matrix4x4::R3C2] + rhs, lhs[matrix4x4::R3C3] + rhs, lhs[matrix4x4::R3C4] + rhs,
        lhs[matrix4x4::R4C1] + rhs, lhs[matrix4x4::R4C2] + rhs, lhs[matrix4x4::R4C3] + rhs, lhs[matrix4x4::R4C4] + rhs
    };
}

matrix4x4::values operator - (const matrix4x4::values& lhs, const matrix4x4::values& rhs) {
    return {
        lhs[matrix4x4::R1C1] - rhs[matrix4x4::R1C1], lhs[matrix4x4::R1C2] - rhs[matrix4x4::R1C2], lhs[matrix4x4::R1C3] - rhs[matrix4x4::R1C3], lhs[matrix4x4::R1C4] - rhs[matrix4x4::R1C4],
        lhs[matrix4x4::R2C1] - rhs[matrix4x4::R2C1], lhs[matrix4x4::R2C2] - rhs[matrix4x4::R2C2], lhs[matrix4x4::R2C3] - rhs[matrix4x4::R2C3], lhs[matrix4x4::R2C4] - rhs[matrix4x4::R2C4],
        lhs[matrix4x4::R3C1] - rhs[matrix4x4::R3C1], lhs[matrix4x4::R3C2] - rhs[matrix4x4::R3C2], lhs[matrix4x4::R3C3] - rhs[matrix4x4::R3C3], lhs[matrix4x4::R3C4] - rhs[matrix4x4::R3C4],
        lhs[matrix4x4::R4C1] - rhs[matrix4x4::R4C1], lhs[matrix4x4::R4C2] - rhs[matrix4x4::R4C2], lhs[matrix4x4::R4C3] - rhs[matrix4x4::R4C3], lhs[matrix4x4::R4C4] - rhs[matrix4x4::R4C4]
    };
}
matrix4x4::values operator - (const matrix4x4::values& lhs, float rhs) {
    return {
        lhs[matrix4x4::R1C1] - rhs, lhs[matrix4x4::R1C2] - rhs, lhs[matrix4x4::R1C3] - rhs, lhs[matrix4x4::R1C4] - rhs,
        lhs[matrix4x4::R2C1] - rhs, lhs[matrix4x4::R2C2] - rhs, lhs[matrix4x4::R2C3] - rhs, lhs[matrix4x4::R2C4] - rhs,
        lhs[matrix4x4::R3C1] - rhs, lhs[matrix4x4::R3C2] - rhs, lhs[matrix4x4::R3C3] - rhs, lhs[matrix4x4::R3C4] - rhs,
        lhs[matrix4x4::R4C1] - rhs, lhs[matrix4x4::R4C2] - rhs, lhs[matrix4x4::R4C3] - rhs, lhs[matrix4x4::R4C4] - rhs
    };
}


///////////////////////////////////////////////////////
// Assignment and manipulation operators

matrix4x4::values& matrix4x4::values::operator = (const values& other) {
    eastl::copy(other.mCells.begin(), other.mCells.end(), mCells.begin());
    return *this;
}

matrix4x4::values& matrix4x4::values::operator + () {
    return *this;
}
matrix4x4::values& matrix4x4::values::operator - () {
    mCells[R1C1] = -mCells[R1C1];  mCells[R1C2] = -mCells[R1C2];  mCells[R1C3] = -mCells[R1C3];  mCells[R1C4] = -mCells[R1C4];
    mCells[R2C1] = -mCells[R2C1];  mCells[R2C2] = -mCells[R2C2];  mCells[R2C3] = -mCells[R2C3];  mCells[R2C4] = -mCells[R2C4];
    mCells[R3C1] = -mCells[R3C1];  mCells[R3C2] = -mCells[R3C2];  mCells[R3C3] = -mCells[R3C3];  mCells[R3C4] = -mCells[R3C4];
    mCells[R4C1] = -mCells[R4C1];  mCells[R4C2] = -mCells[R4C2];  mCells[R4C3] = -mCells[R4C3];  mCells[R4C4] = -mCells[R4C4];
    return *this;
}

matrix4x4::values& matrix4x4::values::operator /= (const values& other) {
    mCells[R1C1] /= other[R1C1];  mCells[R1C2] /= other[R1C2];  mCells[R1C3] /= other[R1C3];  mCells[R1C4] /= other[R1C4];
    mCells[R2C1] /= other[R2C1];  mCells[R2C2] /= other[R2C2];  mCells[R2C3] /= other[R2C3];  mCells[R2C4] /= other[R2C4];
    mCells[R3C1] /= other[R3C1];  mCells[R3C2] /= other[R3C2];  mCells[R3C3] /= other[R3C3];  mCells[R3C4] /= other[R3C4];
    mCells[R4C1] /= other[R4C1];  mCells[R4C2] /= other[R4C2];  mCells[R4C3] /= other[R4C3];  mCells[R4C4] /= other[R4C4];
    return *this;
}
matrix4x4::values& matrix4x4::values::operator /= (float val) {
    mCells[R1C1] /= val;  mCells[R1C2] /= val;  mCells[R1C3] /= val;  mCells[R1C4] /= val;
    mCells[R2C1] /= val;  mCells[R2C2] /= val;  mCells[R2C3] /= val;  mCells[R2C4] /= val;
    mCells[R3C1] /= val;  mCells[R3C2] /= val;  mCells[R3C3] /= val;  mCells[R3C4] /= val;
    mCells[R4C1] /= val;  mCells[R4C2] /= val;  mCells[R4C3] /= val;  mCells[R4C4] /= val;
    return *this;
}

matrix4x4::values& matrix4x4::values::operator *= (const values& other) {
    const values temp = *this;
    // row 1
    mCells[R1C1] = temp[R1C1] * other[R1C1] + temp[R1C2] * other[R2C1] + temp[R1C3] * other[R3C1] + temp[R1C4] * other[R4C1];
    mCells[R1C2] = temp[R1C1] * other[R1C2] + temp[R1C2] * other[R2C2] + temp[R1C3] * other[R3C2] + temp[R1C4] * other[R4C2];
    mCells[R1C3] = temp[R1C1] * other[R1C3] + temp[R1C2] * other[R2C3] + temp[R1C3] * other[R3C3] + temp[R1C4] * other[R4C3];
    mCells[R1C4] = temp[R1C1] * other[R1C4] + temp[R1C2] * other[R2C4] + temp[R1C3] * other[R3C4] + temp[R1C4] * other[R4C4];
    // row 2
    mCells[R2C1] = temp[R2C1] * other[R1C1] + temp[R2C2] * other[R2C1] + temp[R2C3] * other[R3C1] + temp[R2C4] * other[R4C1];
    mCells[R2C2] = temp[R2C1] * other[R1C2] + temp[R2C2] * other[R2C2] + temp[R2C3] * other[R3C2] + temp[R2C4] * other[R4C2];
    mCells[R2C3] = temp[R2C1] * other[R1C3] + temp[R2C2] * other[R2C3] + temp[R2C3] * other[R3C3] + temp[R2C4] * other[R4C3];
    mCells[R2C4] = temp[R2C1] * other[R1C4] + temp[R2C2] * other[R2C4] + temp[R2C3] * other[R3C4] + temp[R2C4] * other[R4C4];
    // row 3
    mCells[R3C1] = temp[R3C1] * other[R1C1] + temp[R3C2] * other[R2C1] + temp[R3C3] * other[R3C1] + temp[R3C4] * other[R4C1];
    mCells[R3C2] = temp[R3C1] * other[R1C2] + temp[R3C2] * other[R2C2] + temp[R3C3] * other[R3C2] + temp[R3C4] * other[R4C2];
    mCells[R3C3] = temp[R3C1] * other[R1C3] + temp[R3C2] * other[R2C3] + temp[R3C3] * other[R3C3] + temp[R3C4] * other[R4C3];
    mCells[R3C4] = temp[R3C1] * other[R1C4] + temp[R3C2] * other[R2C4] + temp[R3C3] * other[R3C4] + temp[R3C4] * other[R4C4];
    // row 4
    mCells[R4C1] = temp[R4C1] * other[R1C1] + temp[R4C2] * other[R2C1] + temp[R4C3] * other[R3C1] + temp[R4C4] * other[R4C1];
    mCells[R4C2] = temp[R4C1] * other[R1C2] + temp[R4C2] * other[R2C2] + temp[R4C3] * other[R3C2] + temp[R4C4] * other[R4C2];
    mCells[R4C3] = temp[R4C1] * other[R1C3] + temp[R4C2] * other[R2C3] + temp[R4C3] * other[R3C3] + temp[R4C4] * other[R4C3];
    mCells[R4C4] = temp[R4C1] * other[R1C4] + temp[R4C2] * other[R2C4] + temp[R4C3] * other[R3C4] + temp[R4C4] * other[R4C4];
    return *this;
}
matrix4x4::values& matrix4x4::values::operator *= (float val) {
    mCells[R1C1] *= val;  mCells[R1C2] *= val;  mCells[R1C3] *= val;  mCells[R1C4] *= val;
    mCells[R2C1] *= val;  mCells[R2C2] *= val;  mCells[R2C3] *= val;  mCells[R2C4] *= val;
    mCells[R3C1] *= val;  mCells[R3C2] *= val;  mCells[R3C3] *= val;  mCells[R3C4] *= val;
    mCells[R4C1] *= val;  mCells[R4C2] *= val;  mCells[R4C3] *= val;  mCells[R4C4] *= val;
    return *this;
}

matrix4x4::values& matrix4x4::values::operator += (const values& other) {
    mCells[R1C1] += other[R1C1];  mCells[R1C2] += other[R1C2];  mCells[R1C3] += other[R1C3];  mCells[R1C4] += other[R1C4];
    mCells[R2C1] += other[R2C1];  mCells[R2C2] += other[R2C2];  mCells[R2C3] += other[R2C3];  mCells[R2C4] += other[R2C4];
    mCells[R3C1] += other[R3C1];  mCells[R3C2] += other[R3C2];  mCells[R3C3] += other[R3C3];  mCells[R3C4] += other[R3C4];
    mCells[R4C1] += other[R4C1];  mCells[R4C2] += other[R4C2];  mCells[R4C3] += other[R4C3];  mCells[R4C4] += other[R4C4];
    return *this;
}
matrix4x4::values& matrix4x4::values::operator += (float val) {
    mCells[R1C1] += val;  mCells[R1C2] += val;  mCells[R1C3] += val;  mCells[R1C4] += val;
    mCells[R2C1] += val;  mCells[R2C2] += val;  mCells[R2C3] += val;  mCells[R2C4] += val;
    mCells[R3C1] += val;  mCells[R3C2] += val;  mCells[R3C3] += val;  mCells[R3C4] += val;
    mCells[R4C1] += val;  mCells[R4C2] += val;  mCells[R4C3] += val;  mCells[R4C4] += val;
    return *this;
}

matrix4x4::values& matrix4x4::values::operator -= (const values& other) {
    mCells[R1C1] -= other[R1C1];  mCells[R1C2] -= other[R1C2];  mCells[R1C3] -= other[R1C3];  mCells[R1C4] -= other[R1C4];
    mCells[R2C1] -= other[R2C1];  mCells[R2C2] -= other[R2C2];  mCells[R2C3] -= other[R2C3];  mCells[R2C4] -= other[R2C4];
    mCells[R3C1] -= other[R3C1];  mCells[R3C2] -= other[R3C2];  mCells[R3C3] -= other[R3C3];  mCells[R3C4] -= other[R3C4];
    mCells[R4C1] -= other[R4C1];  mCells[R4C2] -= other[R4C2];  mCells[R4C3] -= other[R4C3];  mCells[R4C4] -= other[R4C4];
    return *this;
}
matrix4x4::values& matrix4x4::values::operator -= (float val) {
    mCells[R1C1] -= val;  mCells[R1C2] -= val;  mCells[R1C3] -= val;  mCells[R1C4] -= val;
    mCells[R2C1] -= val;  mCells[R2C2] -= val;  mCells[R2C3] -= val;  mCells[R2C4] -= val;
    mCells[R3C1] -= val;  mCells[R3C2] -= val;  mCells[R3C3] -= val;  mCells[R3C4] -= val;
    mCells[R4C1] -= val;  mCells[R4C2] -= val;  mCells[R4C3] -= val;  mCells[R4C4] -= val;
    return *this;
}
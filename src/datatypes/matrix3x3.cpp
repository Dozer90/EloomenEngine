#include "datatype/matrix3x3.h"

#include "utility/defines.h"

#include "utility/managed_memory_block.h"

using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 50;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.7f;
    using memblock_t = managed_sequential_memory_block<float, matrix3x3::CELL_COUNT, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>;
    static memblock_t gMemoryBlock;
}

matrix3x3::id_t matrix3x3::create(MATRIX3X3_DECLARE_PARAMS(v), bool useIDPool) {
    return gMemoryBlock.push({ MATRIX3X3_FORWARD_PARAMS(v) }, useIDPool);
}
matrix3x3::id_t matrix3x3::create(const values& vals, bool useIDPool) {
    return gMemoryBlock.push({ MATRIX3X3_UNPACK(vals) }, useIDPool);
}

bool matrix3x3::try_release(id_t id) {
    return gMemoryBlock.try_remove(id);
}

bool matrix3x3::is_valid(id_t id) {
    return gMemoryBlock.is_valid(id);
}

bool matrix3x3::try_get_values(id_t id, matrix3x3::values& vals) {
    if (is_valid(id)) {
        vals = {
            gMemoryBlock.get(id, R1C1), gMemoryBlock.get(id, R1C2), gMemoryBlock.get(id, R1C3),
            gMemoryBlock.get(id, R2C1), gMemoryBlock.get(id, R2C2), gMemoryBlock.get(id, R2C3),
            gMemoryBlock.get(id, R3C1), gMemoryBlock.get(id, R3C2), gMemoryBlock.get(id, R3C3)
        };
        return true;
    }
    return false;
}

float& matrix3x3::cell(id_t id, int index) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Element offset is out of range.");
    return gMemoryBlock.get(id, index);
}
float& matrix3x3::cell(id_t id, int row, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return gMemoryBlock.get(id, row * COLUMN_COUNT + column);
}

float3::values matrix3x3::row(id_t id, int row) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        gMemoryBlock.get(id, row * COLUMN_COUNT + 0),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 1),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 2)
    };
}

float3::values matrix3x3::column(id_t id, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return {
        gMemoryBlock.get(id, 0 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 1 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 2 * COLUMN_COUNT + column)
    };
}

const float& matrix3x3::const_cell(id_t id, int index) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Element offset is out of range.");
    return gMemoryBlock.get(id, index);
}
const float& matrix3x3::const_cell(id_t id, int row, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return gMemoryBlock.get(id, row * COLUMN_COUNT + column);
}

const float3::values matrix3x3::const_row(id_t id, int row) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        gMemoryBlock.get(id, row * COLUMN_COUNT + 0),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 1),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 2)
    };
}

const float3::values matrix3x3::const_column(id_t id, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return {
        gMemoryBlock.get(id, 0 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 1 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 2 * COLUMN_COUNT + column)
    };
}


///////////////////////////////////////////////////////
// Values container

float3::values matrix3x3::values::row(int row) {
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        mCells[row * COLUMN_COUNT + 0],
        mCells[row * COLUMN_COUNT + 1],
        mCells[row * COLUMN_COUNT + 2]
    };
}

float3::values matrix3x3::values::column(int column) {
    ELOO_ASSERT(column >= 0 && column < 2, "Column index is out of range.");
    return {
        mCells[0 * COLUMN_COUNT + column],
        mCells[1 * COLUMN_COUNT + column],
        mCells[2 * COLUMN_COUNT + column]
    };
}

const float3::values matrix3x3::values::row(int row) const {
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        mCells[row * COLUMN_COUNT + 0],
        mCells[row * COLUMN_COUNT + 1],
        mCells[row * COLUMN_COUNT + 2]
    };
}

const float3::values matrix3x3::values::column(int column) const {
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return {
        mCells[0 * COLUMN_COUNT + column],
        mCells[1 * COLUMN_COUNT + column],
        mCells[2 * COLUMN_COUNT + column]
    };
}

float& matrix3x3::values::cell(int index) {
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Index is out of range.");
    return mCells[index];
}

const float& matrix3x3::values::cell(int index) const {
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Index is out of range.");
    return mCells[index];
}

bool operator != (const matrix3x3::values& lhs, const matrix3x3::values& rhs) {
    return
        lhs[matrix3x3::R1C1] != rhs[matrix3x3::R1C1] || lhs[matrix3x3::R1C2] != rhs[matrix3x3::R1C2] || lhs[matrix3x3::R1C3] != rhs[matrix3x3::R1C3] ||
        lhs[matrix3x3::R2C1] != rhs[matrix3x3::R2C1] || lhs[matrix3x3::R2C2] != rhs[matrix3x3::R2C2] || lhs[matrix3x3::R2C3] != rhs[matrix3x3::R2C3] ||
        lhs[matrix3x3::R3C1] != rhs[matrix3x3::R3C1] || lhs[matrix3x3::R3C2] != rhs[matrix3x3::R3C2] || lhs[matrix3x3::R3C3] != rhs[matrix3x3::R3C3];
}
bool operator != (const matrix3x3::values& lhs, float rhs) {
    return
        lhs[matrix3x3::R1C1] != rhs || lhs[matrix3x3::R1C2] != rhs || lhs[matrix3x3::R1C3] != rhs ||
        lhs[matrix3x3::R2C1] != rhs || lhs[matrix3x3::R2C2] != rhs || lhs[matrix3x3::R2C3] != rhs ||
        lhs[matrix3x3::R3C1] != rhs || lhs[matrix3x3::R3C2] != rhs || lhs[matrix3x3::R3C3] != rhs;
}

bool operator == (const matrix3x3::values& lhs, const matrix3x3::values& rhs) {
    return
        lhs[matrix3x3::R1C1] == rhs[matrix3x3::R1C1] && lhs[matrix3x3::R1C2] == rhs[matrix3x3::R1C2] && lhs[matrix3x3::R1C3] == rhs[matrix3x3::R1C3] &&
        lhs[matrix3x3::R2C1] == rhs[matrix3x3::R2C1] && lhs[matrix3x3::R2C2] == rhs[matrix3x3::R2C2] && lhs[matrix3x3::R2C3] == rhs[matrix3x3::R2C3] &&
        lhs[matrix3x3::R3C1] == rhs[matrix3x3::R3C1] && lhs[matrix3x3::R3C2] == rhs[matrix3x3::R3C2] && lhs[matrix3x3::R3C3] == rhs[matrix3x3::R3C3];
}
bool operator == (const matrix3x3::values& lhs, float rhs) {
    return
        lhs[matrix3x3::R1C1] == rhs && lhs[matrix3x3::R1C2] == rhs && lhs[matrix3x3::R1C3] == rhs &&
        lhs[matrix3x3::R2C1] == rhs && lhs[matrix3x3::R2C2] == rhs && lhs[matrix3x3::R2C3] == rhs &&
        lhs[matrix3x3::R3C1] == rhs && lhs[matrix3x3::R3C2] == rhs && lhs[matrix3x3::R3C3] == rhs;
}

matrix3x3::values operator + (const matrix3x3::values& lhs) {
    return lhs;
}

matrix3x3::values operator - (const matrix3x3::values& lhs) {
    return {
        -lhs[matrix3x3::R1C1], -lhs[matrix3x3::R1C2], -lhs[matrix3x3::R1C3],
        -lhs[matrix3x3::R2C1], -lhs[matrix3x3::R2C2], -lhs[matrix3x3::R2C3],
        -lhs[matrix3x3::R3C1], -lhs[matrix3x3::R3C2], -lhs[matrix3x3::R3C3]
    };
}

matrix3x3::values operator / (const matrix3x3::values& lhs, const matrix3x3::values& rhs) {
    return {
        lhs[matrix3x3::R1C1] / rhs[matrix3x3::R1C1], lhs[matrix3x3::R1C2] / rhs[matrix3x3::R1C2], lhs[matrix3x3::R1C3] / rhs[matrix3x3::R1C3],
        lhs[matrix3x3::R2C1] / rhs[matrix3x3::R2C1], lhs[matrix3x3::R2C2] / rhs[matrix3x3::R2C2], lhs[matrix3x3::R2C3] / rhs[matrix3x3::R2C3],
        lhs[matrix3x3::R3C1] / rhs[matrix3x3::R3C1], lhs[matrix3x3::R3C2] / rhs[matrix3x3::R3C2], lhs[matrix3x3::R3C3] / rhs[matrix3x3::R3C3]
    };
}
matrix3x3::values operator / (const matrix3x3::values& lhs, float rhs) {
    return {
        lhs[matrix3x3::R1C1] / rhs, lhs[matrix3x3::R1C2] / rhs, lhs[matrix3x3::R1C3] / rhs,
        lhs[matrix3x3::R2C1] / rhs, lhs[matrix3x3::R2C2] / rhs, lhs[matrix3x3::R2C3] / rhs,
        lhs[matrix3x3::R3C1] / rhs, lhs[matrix3x3::R3C2] / rhs, lhs[matrix3x3::R3C3] / rhs
    };
}

matrix3x3::values operator * (const matrix3x3::values& lhs, const matrix3x3::values& rhs) {
    return {
        // row 1
        lhs[matrix3x3::R1C1] * rhs[matrix3x3::R1C1] + lhs[matrix3x3::R1C2] * rhs[matrix3x3::R2C1] + lhs[matrix3x3::R1C3] * rhs[matrix3x3::R3C1],
        lhs[matrix3x3::R1C1] * rhs[matrix3x3::R1C2] + lhs[matrix3x3::R1C2] * rhs[matrix3x3::R2C2] + lhs[matrix3x3::R1C3] * rhs[matrix3x3::R3C2],
        lhs[matrix3x3::R1C1] * rhs[matrix3x3::R1C3] + lhs[matrix3x3::R1C2] * rhs[matrix3x3::R2C3] + lhs[matrix3x3::R1C3] * rhs[matrix3x3::R3C3],
        // row 2
        lhs[matrix3x3::R2C1] * rhs[matrix3x3::R1C1] + lhs[matrix3x3::R2C2] * rhs[matrix3x3::R2C1] + lhs[matrix3x3::R2C3] * rhs[matrix3x3::R3C1],
        lhs[matrix3x3::R2C1] * rhs[matrix3x3::R1C2] + lhs[matrix3x3::R2C2] * rhs[matrix3x3::R2C2] + lhs[matrix3x3::R2C3] * rhs[matrix3x3::R3C2],
        lhs[matrix3x3::R2C1] * rhs[matrix3x3::R1C3] + lhs[matrix3x3::R2C2] * rhs[matrix3x3::R2C3] + lhs[matrix3x3::R2C3] * rhs[matrix3x3::R3C3],
        // row 3
        lhs[matrix3x3::R3C1] * rhs[matrix3x3::R1C1] + lhs[matrix3x3::R3C2] * rhs[matrix3x3::R2C1] + lhs[matrix3x3::R3C3] * rhs[matrix3x3::R3C1],
        lhs[matrix3x3::R3C1] * rhs[matrix3x3::R1C2] + lhs[matrix3x3::R3C2] * rhs[matrix3x3::R2C2] + lhs[matrix3x3::R3C3] * rhs[matrix3x3::R3C2],
        lhs[matrix3x3::R3C1] * rhs[matrix3x3::R1C3] + lhs[matrix3x3::R3C2] * rhs[matrix3x3::R2C3] + lhs[matrix3x3::R3C3] * rhs[matrix3x3::R3C3]
    };
}
matrix3x3::values operator * (const matrix3x3::values& lhs, float rhs) {
    return {
        lhs[matrix3x3::R1C1] * rhs, lhs[matrix3x3::R1C2] * rhs, lhs[matrix3x3::R1C3] * rhs,
        lhs[matrix3x3::R2C1] * rhs, lhs[matrix3x3::R2C2] * rhs, lhs[matrix3x3::R2C3] * rhs,
        lhs[matrix3x3::R3C1] * rhs, lhs[matrix3x3::R3C2] * rhs, lhs[matrix3x3::R3C3] * rhs
    };
}
float3::values operator * (const matrix3x3::values& lhs, const float3::values& rhs) {
    return {
        lhs[matrix3x3::R1C1] * rhs.x() + lhs[matrix3x3::R1C2] * rhs.y() + lhs[matrix3x3::R1C3] * rhs.z(),
        lhs[matrix3x3::R2C1] * rhs.x() + lhs[matrix3x3::R2C2] * rhs.y() + lhs[matrix3x3::R2C3] * rhs.z(),
        lhs[matrix3x3::R3C1] * rhs.x() + lhs[matrix3x3::R3C2] * rhs.y() + lhs[matrix3x3::R3C3] * rhs.z()
    };
}

matrix3x3::values operator + (const matrix3x3::values& lhs, const matrix3x3::values& rhs) {
    return {
        lhs[matrix3x3::R1C1] + rhs[matrix3x3::R1C1], lhs[matrix3x3::R1C2] + rhs[matrix3x3::R1C2], lhs[matrix3x3::R1C3] + rhs[matrix3x3::R1C3],
        lhs[matrix3x3::R2C1] + rhs[matrix3x3::R2C1], lhs[matrix3x3::R2C2] + rhs[matrix3x3::R2C2], lhs[matrix3x3::R2C3] + rhs[matrix3x3::R2C3],
        lhs[matrix3x3::R3C1] + rhs[matrix3x3::R3C1], lhs[matrix3x3::R3C2] + rhs[matrix3x3::R3C2], lhs[matrix3x3::R3C3] + rhs[matrix3x3::R3C3]
    };
}
matrix3x3::values operator + (const matrix3x3::values& lhs, float rhs) {
    return {
        lhs[matrix3x3::R1C1] + rhs, lhs[matrix3x3::R1C2] + rhs, lhs[matrix3x3::R1C3] + rhs,
        lhs[matrix3x3::R2C1] + rhs, lhs[matrix3x3::R2C2] + rhs, lhs[matrix3x3::R2C3] + rhs,
        lhs[matrix3x3::R3C1] + rhs, lhs[matrix3x3::R3C2] + rhs, lhs[matrix3x3::R3C3] + rhs
    };
}

matrix3x3::values operator - (const matrix3x3::values& lhs, const matrix3x3::values& rhs) {
    return {
        lhs[matrix3x3::R1C1] - rhs[matrix3x3::R1C1], lhs[matrix3x3::R1C2] - rhs[matrix3x3::R1C2], lhs[matrix3x3::R1C3] - rhs[matrix3x3::R1C3],
        lhs[matrix3x3::R2C1] - rhs[matrix3x3::R2C1], lhs[matrix3x3::R2C2] - rhs[matrix3x3::R2C2], lhs[matrix3x3::R2C3] - rhs[matrix3x3::R2C3],
        lhs[matrix3x3::R3C1] - rhs[matrix3x3::R3C1], lhs[matrix3x3::R3C2] - rhs[matrix3x3::R3C2], lhs[matrix3x3::R3C3] - rhs[matrix3x3::R3C3]
    };
}
matrix3x3::values operator - (const matrix3x3::values& lhs, float rhs) {
    return {
        lhs[matrix3x3::R1C1] - rhs, lhs[matrix3x3::R1C2] - rhs, lhs[matrix3x3::R1C3] - rhs,
        lhs[matrix3x3::R2C1] - rhs, lhs[matrix3x3::R2C2] - rhs, lhs[matrix3x3::R2C3] - rhs,
        lhs[matrix3x3::R3C1] - rhs, lhs[matrix3x3::R3C2] - rhs, lhs[matrix3x3::R3C3] - rhs
    };
}


///////////////////////////////////////////////////////
// Assignment and manipulation operators

matrix3x3::values& matrix3x3::values::operator = (const values& other) {
    eastl::copy(other.mCells.begin(), other.mCells.end(), mCells.begin());
    return *this;
}

matrix3x3::values& matrix3x3::values::operator + () {
    return *this;
}
matrix3x3::values& matrix3x3::values::operator - () {
    mCells[R1C1] = -mCells[R1C1];  mCells[R1C2] = -mCells[R1C2];  mCells[R1C3] = -mCells[R1C3];
    mCells[R2C1] = -mCells[R2C1];  mCells[R2C2] = -mCells[R2C2];  mCells[R2C3] = -mCells[R2C3];
    mCells[R3C1] = -mCells[R3C1];  mCells[R3C2] = -mCells[R3C2];  mCells[R3C3] = -mCells[R3C3];
    return *this;
}

matrix3x3::values& matrix3x3::values::operator /= (const values& other) {
    mCells[R1C1] /= other[R1C1];  mCells[R1C2] /= other[R1C2];  mCells[R1C3] /= other[R1C3];
    mCells[R2C1] /= other[R2C1];  mCells[R2C2] /= other[R2C2];  mCells[R2C3] /= other[R2C3];
    mCells[R3C1] /= other[R3C1];  mCells[R3C2] /= other[R3C2];  mCells[R3C3] /= other[R3C3];
    return *this;
}
matrix3x3::values& matrix3x3::values::operator /= (float val) {
    mCells[R1C1] /= val;  mCells[R1C2] /= val;  mCells[R1C3] /= val;
    mCells[R2C1] /= val;  mCells[R2C2] /= val;  mCells[R2C3] /= val;
    mCells[R3C1] /= val;  mCells[R3C2] /= val;  mCells[R3C3] /= val;
    return *this;
}

matrix3x3::values& matrix3x3::values::operator *= (const values& other) {
    const values temp = *this;
    // row 1
    mCells[R1C1] = temp[R1C1] * other[R1C1] + temp[R1C2] * other[R2C1] + temp[R1C3] * other[R3C1];
    mCells[R1C2] = temp[R1C1] * other[R1C2] + temp[R1C2] * other[R2C2] + temp[R1C3] * other[R3C2];
    mCells[R1C3] = temp[R1C1] * other[R1C3] + temp[R1C2] * other[R2C3] + temp[R1C3] * other[R3C3];
    // row 2
    mCells[R2C1] = temp[R2C1] * other[R1C1] + temp[R2C2] * other[R2C1] + temp[R2C3] * other[R3C1];
    mCells[R2C2] = temp[R2C1] * other[R1C2] + temp[R2C2] * other[R2C2] + temp[R2C3] * other[R3C2];
    mCells[R2C3] = temp[R2C1] * other[R1C3] + temp[R2C2] * other[R2C3] + temp[R2C3] * other[R3C3];
    // row 3
    mCells[R3C1] = temp[R3C1] * other[R1C1] + temp[R3C2] * other[R2C1] + temp[R3C3] * other[R3C1];
    mCells[R3C2] = temp[R3C1] * other[R1C2] + temp[R3C2] * other[R2C2] + temp[R3C3] * other[R3C2];
    mCells[R3C3] = temp[R3C1] * other[R1C3] + temp[R3C2] * other[R2C3] + temp[R3C3] * other[R3C3];
    return *this;
}
matrix3x3::values& matrix3x3::values::operator *= (float val) {
    mCells[R1C1] *= val;  mCells[R1C2] *= val;  mCells[R1C3] *= val;
    mCells[R2C1] *= val;  mCells[R2C2] *= val;  mCells[R2C3] *= val;
    mCells[R3C1] *= val;  mCells[R3C2] *= val;  mCells[R3C3] *= val;
    return *this;
}

matrix3x3::values& matrix3x3::values::operator += (const values& other) {
    mCells[R1C1] += other[R1C1];  mCells[R1C2] += other[R1C2];  mCells[R1C3] += other[R1C3];
    mCells[R2C1] += other[R2C1];  mCells[R2C2] += other[R2C2];  mCells[R2C3] += other[R2C3];
    mCells[R3C1] += other[R3C1];  mCells[R3C2] += other[R3C2];  mCells[R3C3] += other[R3C3];
    return *this;
}
matrix3x3::values& matrix3x3::values::operator += (float val) {
    mCells[R1C1] += val;  mCells[R1C2] += val;  mCells[R1C3] += val;
    mCells[R2C1] += val;  mCells[R2C2] += val;  mCells[R2C3] += val;
    mCells[R3C1] += val;  mCells[R3C2] += val;  mCells[R3C3] += val;
    return *this;
}

matrix3x3::values& matrix3x3::values::operator -= (const values& other) {
    mCells[R1C1] -= other[R1C1];  mCells[R1C2] -= other[R1C2];  mCells[R1C3] -= other[R1C3];
    mCells[R2C1] -= other[R2C1];  mCells[R2C2] -= other[R2C2];  mCells[R2C3] -= other[R2C3];
    mCells[R3C1] -= other[R3C1];  mCells[R3C2] -= other[R3C2];  mCells[R3C3] -= other[R3C3];
    return *this;
}
matrix3x3::values& matrix3x3::values::operator -= (float val) {
    mCells[R1C1] -= val;  mCells[R1C2] -= val;  mCells[R1C3] -= val;
    mCells[R2C1] -= val;  mCells[R2C2] -= val;  mCells[R2C3] -= val;
    mCells[R3C1] -= val;  mCells[R3C2] -= val;  mCells[R3C3] -= val;
    return *this;
}
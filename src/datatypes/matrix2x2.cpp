#include <datatype/matrix2x2.h>

#include <utility/defines.h>

#include <utility/managed_memory_block.h>

using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 50;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.7f;
    using memblock_t = managed_sequential_memory_block<float, matrix2x2::CELL_COUNT, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>;
    static memblock_t gMemoryBlock;
}

matrix2x2::id_t matrix2x2::create(MATRIX2X2_DECLARE_PARAMS(v), bool useIDPool) {
    return gMemoryBlock.push({ MATRIX2X2_FORWARD_PARAMS(v) }, useIDPool);
}
matrix2x2::id_t matrix2x2::create(const values& vals, bool useIDPool) {
    return gMemoryBlock.push({ MATRIX2X2_UNPACK(vals) }, useIDPool);
}

bool matrix2x2::try_release(id_t id) {
    return gMemoryBlock.try_remove(id);
}

bool matrix2x2::is_valid(id_t id) {
    return gMemoryBlock.is_valid(id);
}

bool matrix2x2::try_get_values(id_t id, matrix2x2::values& vals) {
    if (is_valid(id)) {
        vals = {
            gMemoryBlock.get(id, R1C1), gMemoryBlock.get(id, R1C2),
            gMemoryBlock.get(id, R2C1), gMemoryBlock.get(id, R2C2)
        };
        return true;
    }
    return false;
}

float& matrix2x2::cell(id_t id, int index) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Element offset is out of range.");
    return gMemoryBlock.get(id, index);
}
float& matrix2x2::cell(id_t id, int row, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return gMemoryBlock.get(id, row * COLUMN_COUNT + column);
}

float2::values matrix2x2::row(id_t id, int row) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        gMemoryBlock.get(id, row * COLUMN_COUNT + 0),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 1)
    };
}

float2::values matrix2x2::column(id_t id, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return {
        gMemoryBlock.get(id, 0 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 1 * COLUMN_COUNT + column)
    };
}

const float& matrix2x2::const_cell(id_t id, int index) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Element offset is out of range.");
    return gMemoryBlock.get(id, index);
}
const float& matrix2x2::const_cell(id_t id, int row, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return gMemoryBlock.get(id, row * COLUMN_COUNT + column);
}

const float2::values matrix2x2::const_row(id_t id, int row) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        gMemoryBlock.get(id, row * COLUMN_COUNT + 0),
        gMemoryBlock.get(id, row * COLUMN_COUNT + 1)
    };
}

const float2::values matrix2x2::const_column(id_t id, int column) {
    ELOO_ASSERT(is_valid(id), "ID is invalid.");
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return {
        gMemoryBlock.get(id, 0 * COLUMN_COUNT + column),
        gMemoryBlock.get(id, 1 * COLUMN_COUNT + column)
    };
}


///////////////////////////////////////////////////////
// Values container

float2::values matrix2x2::values::row(int row) {
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        mCells[row * COLUMN_COUNT + 0],
        mCells[row * COLUMN_COUNT + 1]
    };
}

float2::values matrix2x2::values::column(int column) {
    ELOO_ASSERT(column >= 0 && column < 2, "Column index is out of range.");
    return {
        mCells[0 * COLUMN_COUNT + column],
        mCells[1 * COLUMN_COUNT + column]
    };
}

const float2::values matrix2x2::values::row(int row) const {
    ELOO_ASSERT(row >= 0 && row < ROW_COUNT, "Row index is out of range.");
    return {
        mCells[row * COLUMN_COUNT + 0],
        mCells[row * COLUMN_COUNT + 1]
    };
}

const float2::values matrix2x2::values::column(int column) const {
    ELOO_ASSERT(column >= 0 && column < COLUMN_COUNT, "Column index is out of range.");
    return {
        mCells[0 * COLUMN_COUNT + column],
        mCells[1 * COLUMN_COUNT + column]
    };
}

float& matrix2x2::values::cell(int index) {
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Index is out of range.");
    return mCells[index];
}

const float& matrix2x2::values::cell(int index) const {
    ELOO_ASSERT(index >= 0 && index < CELL_COUNT, "Index is out of range.");
    return mCells[index];
}

bool operator != (const matrix2x2::values& lhs, const matrix2x2::values& rhs) {
    return
        lhs[matrix2x2::R1C1] != rhs[matrix2x2::R1C1] || lhs[matrix2x2::R1C2] != rhs[matrix2x2::R1C2] ||
        lhs[matrix2x2::R2C1] != rhs[matrix2x2::R2C1] || lhs[matrix2x2::R2C2] != rhs[matrix2x2::R2C2];
}
bool operator != (const matrix2x2::values& lhs, float rhs) {
    return
        lhs[matrix2x2::R1C1] != rhs || lhs[matrix2x2::R1C2] != rhs ||
        lhs[matrix2x2::R2C1] != rhs || lhs[matrix2x2::R2C2] != rhs;
}

bool operator == (const matrix2x2::values& lhs, const matrix2x2::values& rhs) {
    return
        lhs[matrix2x2::R1C1] == rhs[matrix2x2::R1C1] && lhs[matrix2x2::R1C2] == rhs[matrix2x2::R1C2] &&
        lhs[matrix2x2::R2C1] == rhs[matrix2x2::R2C1] && lhs[matrix2x2::R2C2] == rhs[matrix2x2::R2C2];
}
bool operator == (const matrix2x2::values& lhs, float rhs) {
    return
        lhs[matrix2x2::R1C1] == rhs && lhs[matrix2x2::R1C2] == rhs &&
        lhs[matrix2x2::R2C1] == rhs && lhs[matrix2x2::R2C2] == rhs;
}

matrix2x2::values operator + (const matrix2x2::values& lhs) {
    return lhs;
}

matrix2x2::values operator - (const matrix2x2::values& lhs) {
    return {
        -lhs[matrix2x2::R1C1], -lhs[matrix2x2::R1C2],
        -lhs[matrix2x2::R2C1], -lhs[matrix2x2::R2C2]
    };
}

matrix2x2::values operator / (const matrix2x2::values& lhs, const matrix2x2::values& rhs) {
    return {
        lhs[matrix2x2::R1C1] / rhs[matrix2x2::R1C1], lhs[matrix2x2::R1C2] / rhs[matrix2x2::R1C2],
        lhs[matrix2x2::R2C1] / rhs[matrix2x2::R2C1], lhs[matrix2x2::R2C2] / rhs[matrix2x2::R2C2]
    };
}
matrix2x2::values operator / (const matrix2x2::values& lhs, float rhs) {
    return {
        lhs[matrix2x2::R1C1] / rhs, lhs[matrix2x2::R1C2] / rhs,
        lhs[matrix2x2::R2C1] / rhs, lhs[matrix2x2::R2C2] / rhs
    };
}

matrix2x2::values operator * (const matrix2x2::values& lhs, const matrix2x2::values& rhs) {
    return {
        // row 1
        lhs[matrix2x2::R1C1] * rhs[matrix2x2::R1C1] + lhs[matrix2x2::R1C2] * rhs[matrix2x2::R2C1],
        lhs[matrix2x2::R1C1] * rhs[matrix2x2::R1C2] + lhs[matrix2x2::R1C2] * rhs[matrix2x2::R2C2],
        // row 2
        lhs[matrix2x2::R2C1] * rhs[matrix2x2::R1C1] + lhs[matrix2x2::R2C2] * rhs[matrix2x2::R2C1],
        lhs[matrix2x2::R2C1] * rhs[matrix2x2::R1C2] + lhs[matrix2x2::R2C2] * rhs[matrix2x2::R2C2]
    };
}
matrix2x2::values operator * (const matrix2x2::values& lhs, float rhs) {
    return {
        lhs[matrix2x2::R1C1] * rhs, lhs[matrix2x2::R1C2] * rhs,
        lhs[matrix2x2::R2C1] * rhs, lhs[matrix2x2::R2C2] * rhs
    };
}
float2::values operator * (const matrix2x2::values& lhs, const float2::values& rhs) {
    return {
        lhs[matrix2x2::R1C1] * rhs.x() + lhs[matrix2x2::R1C2] * rhs.y(),
        lhs[matrix2x2::R2C1] * rhs.x() + lhs[matrix2x2::R2C2] * rhs.y()
    };
}

matrix2x2::values operator + (const matrix2x2::values& lhs, const matrix2x2::values& rhs) {
    return {
        lhs[matrix2x2::R1C1] + rhs[matrix2x2::R1C1], lhs[matrix2x2::R1C2] + rhs[matrix2x2::R1C2],
        lhs[matrix2x2::R2C1] + rhs[matrix2x2::R2C1], lhs[matrix2x2::R2C2] + rhs[matrix2x2::R2C2]
    };
}
matrix2x2::values operator + (const matrix2x2::values& lhs, float rhs) {
    return {
        lhs[matrix2x2::R1C1] + rhs, lhs[matrix2x2::R1C2] + rhs,
        lhs[matrix2x2::R2C1] + rhs, lhs[matrix2x2::R2C2] + rhs
    };
}

matrix2x2::values operator - (const matrix2x2::values& lhs, const matrix2x2::values& rhs) {
    return {
        lhs[matrix2x2::R1C1] - rhs[matrix2x2::R1C1], lhs[matrix2x2::R1C2] - rhs[matrix2x2::R1C2],
        lhs[matrix2x2::R2C1] - rhs[matrix2x2::R2C1], lhs[matrix2x2::R2C2] - rhs[matrix2x2::R2C2]
    };
}
matrix2x2::values operator - (const matrix2x2::values& lhs, float rhs) {
    return {
        lhs[matrix2x2::R1C1] - rhs, lhs[matrix2x2::R1C2] - rhs,
        lhs[matrix2x2::R2C1] - rhs, lhs[matrix2x2::R2C2] - rhs
    };
}


///////////////////////////////////////////////////////
// Assignment and manipulation operators

matrix2x2::values& matrix2x2::values::operator = (const values& other) {
    eastl::copy(other.mCells.begin(), other.mCells.end(), mCells.begin());
    return *this;
}

matrix2x2::values& matrix2x2::values::operator + () {
    return *this;
}
matrix2x2::values& matrix2x2::values::operator - () {
    mCells[R1C1] = -mCells[R1C1];  mCells[R1C2] = -mCells[R1C2];
    mCells[R2C1] = -mCells[R2C1];  mCells[R2C2] = -mCells[R2C2];
    return *this;
}

matrix2x2::values& matrix2x2::values::operator /= (const values& other) {
    mCells[R1C1] /= other[R1C1];  mCells[R1C2] /= other[R1C2];
    mCells[R2C1] /= other[R2C1];  mCells[R2C2] /= other[R2C2];
    return *this;
}
matrix2x2::values& matrix2x2::values::operator /= (float val) {
    mCells[R1C1] /= val;  mCells[R1C2] /= val;
    mCells[R2C1] /= val;  mCells[R2C2] /= val;
    return *this;
}

matrix2x2::values& matrix2x2::values::operator *= (const values& other) {
    const values temp = *this;
    // row 1
    mCells[R1C1] = temp[R1C1] * other[R1C1] + temp[R1C2] * other[R2C1];
    mCells[R1C2] = temp[R1C1] * other[R1C2] + temp[R1C2] * other[R2C2];
    // row 2
    mCells[R2C1] = temp[R2C1] * other[R1C1] + temp[R2C2] * other[R2C1];
    mCells[R2C2] = temp[R2C1] * other[R1C2] + temp[R2C2] * other[R2C2];
    return *this;
}
matrix2x2::values& matrix2x2::values::operator *= (float val) {
    mCells[R1C1] *= val;  mCells[R1C2] *= val;
    mCells[R2C1] *= val;  mCells[R2C2] *= val;
    return *this;
}

matrix2x2::values& matrix2x2::values::operator += (const values& other) {
    mCells[R1C1] += other[R1C1];  mCells[R1C2] += other[R1C2];
    mCells[R2C1] += other[R2C1];  mCells[R2C2] += other[R2C2];
    return *this;
}
matrix2x2::values& matrix2x2::values::operator += (float val) {
    mCells[R1C1] += val;  mCells[R1C2] += val;
    mCells[R2C1] += val;  mCells[R2C2] += val;
    return *this;
}

matrix2x2::values& matrix2x2::values::operator -= (const values& other) {
    mCells[R1C1] -= other[R1C1];  mCells[R1C2] -= other[R1C2];
    mCells[R2C1] -= other[R2C1];  mCells[R2C2] -= other[R2C2];
    return *this;
}
matrix2x2::values& matrix2x2::values::operator -= (float val) {
    mCells[R1C1] -= val;  mCells[R1C2] -= val;
    mCells[R2C1] -= val;  mCells[R2C2] -= val;
    return *this;
}
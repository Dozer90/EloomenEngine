#include <maths/matrix2x2.h>

#include <utility/managed_memory_block.h>

using namespace eloo;

namespace {
    constexpr int gMemoryBlockInitialSize = 50;
    constexpr float gMemoryBlockExpansionScalar = 0.7f;
    using memblock_t = managed_sequential_memory_block<float, 2 * 2, gMemoryBlockInitialSize, gMemoryBlockExpansionScalar>;
    static memblock_t gMemoryBlock;
}

matrix2x2::id_t matrix2x2::create(float a11, float a12, float a21, float a22, bool useIDPool) {
    return gMemoryBlock.push({ a11, a12, a21, a22 }, useIDPool);
}

matrix2x2::id_t matrix2x2::create(const float2::value& row1, const float2::value& row2, bool useIDPool) {
    return gMemoryBlock.push({ row1.x(), row1.y(), row2.x(), row2.y() }, useIDPool);
}

matrix2x2::id_t matrix2x2::create(const value& vals, bool useIDPool) {
    return gMemoryBlock.push({ vals.a11(), vals.a12(), vals.a21(), vals.a22()}, useIDPool);
}

bool matrix2x2::try_release(id_t id) {
    return gMemoryBlock.try_remove(id);
}

matrix2x2::view matrix2x2::get_view(id_t id) {
    return {
        gMemoryBlock.get(id, 0), gMemoryBlock.get(id, 1),
        gMemoryBlock.get(id, 2), gMemoryBlock.get(id, 3)
    };
}

float& matrix2x2::value(id_t id, int elementOffset) {
    return gMemoryBlock.get(id, elementOffset);
}

float& matrix2x2::value(id_t id, int row, int col) {
    return gMemoryBlock.get(id, row * 2 + col);
}

const float& matrix2x2::const_value(id_t id, int elementOffset) {
    return gMemoryBlock.get(id, elementOffset);
}

const float& matrix2x2::const_value(id_t id, int row, int col) {
    return gMemoryBlock.get(id, row * 2 + col);
}


///////////////////////////////////////////////////////////////////
// view

bool operator != (const matrix2x2::view& lhs, const matrix2x2::view& rhs) {
    return
        lhs.a11() != rhs.a11() ||
        lhs.a12() != rhs.a12() ||
        lhs.a21() != rhs.a21() ||
        lhs.a22() != rhs.a22();
}

bool operator != (const matrix2x2::view& lhs, const matrix2x2::value& rhs) {
    return
        lhs.a11() != rhs.a11() ||
        lhs.a12() != rhs.a12() ||
        lhs.a21() != rhs.a21() ||
        lhs.a22() != rhs.a22();
}

bool operator == (const matrix2x2::view& lhs, const matrix2x2::view& rhs) {
    return
        lhs.a11() == rhs.a11() &&
        lhs.a12() == rhs.a12() &&
        lhs.a21() == rhs.a21() &&
        lhs.a22() == rhs.a22();
}

bool operator == (const matrix2x2::view& lhs, const matrix2x2::value& rhs) {
    return
        lhs.a11() == rhs.a11() &&
        lhs.a12() == rhs.a12() &&
        lhs.a21() == rhs.a21() &&
        lhs.a22() == rhs.a22();
}

matrix2x2::view& matrix2x2::view::operator = (const view& other) {
    mID = other.mID;
    return *this;
}

matrix2x2::view& matrix2x2::view::operator = (const value& other) {
    mID = other.mID;
    return *this;
}
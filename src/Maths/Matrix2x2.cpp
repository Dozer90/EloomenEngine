#include <Maths/Matrix2x2.h>

#include <Maths/Matrix2x2.h>

using namespace eloo::Math;

namespace {
static constexpr Matrix2x2 gZero({ 0.0f }, { 0.0f });
static constexpr Matrix2x2 gOne({ 1.0f }, { 1.0f });
static constexpr Matrix2x2 gIdentity;
}

Matrix2x2::Matrix2x2(const Matrix3x3& m, uint8_t row, uint8_t col) {
    uint8_t i = 0;
    for (uint8_t r = 0; r < 3; ++r) {
        if (r == row) continue;
        for (uint8_t c = 0; c < 3; ++c) {
            if (c == col) continue;
            data[i++] = m[r][c];
        }
    }
}

const Matrix2x2& Matrix2x2::zero() {
    return gZero;
}

const Matrix2x2& Matrix2x2::one() {
    return gOne;
}

const Matrix2x2& Matrix2x2::identity() {
    return gIdentity;
}
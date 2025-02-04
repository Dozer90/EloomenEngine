#include <Maths/Matrix2x2.h>

#include <Maths/Matrix3x3.h>

using namespace eloo::Math;

Matrix2x2::Matrix2x2(const Matrix3x3& m, uint8_t row, uint8_t col) {
    uint8_t i = 0;
    for (uint8_t r = 0; r < 3; ++r) {
        if (r == row) continue;
        for (uint8_t c = 0; c < 3; ++c) {
            if (c == col) continue;
            mData[i++] = m[r][c];
        }
    }
}

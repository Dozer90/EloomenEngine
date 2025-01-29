#pragma once

#include <Generic/Utilities.h>

#include <EASTL/array.h>

namespace eloo::Math
{
template <typename TValueType>
struct MatrixRow {
    inline explicit MatrixRow(TValueType* firstRowValue, uint8_t length) : mValueStart(data), mLength(length) {}
    inline TValueType& operator[](uint8_t index) {
        ASSERT(index < mLength, "Trying to access non-existant column %i in a MatrixRow", index);
        return mDataRef[index];
    }
    inline TValueType& operator[](uint8_t index) const {
        ASSERT(index < mLength, "Trying to access non-existant column %i in a MatrixRow", index);
        return mDataRef[index];
    }
private:
    TValueType* const mValueStart;
    const uint8_t mLength;
};
} // eloo::Math
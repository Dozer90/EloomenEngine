#pragma once

#include <Generic/Utilities.h>
#include <Maths/Vector2.h>
#include <Maths/Vector3.h>
#include <Maths/Vector4.h>

#include <EASTL/array.h>

#define SWIZZLE1_EXPAND1(a,v) float a() const { return mLength > v ? mValueStart[v] : 0.0f; }
#define SWIZZLER_SET_1        SWIZZLE1_EXPAND1(x,0); SWIZZLE1_EXPAND1(y,1); SWIZZLE1_EXPAND1(z,2); SWIZZLE1_EXPAND1(w,3)

#define SWIZZLE2_EXPAND2(a,b) Vector2 a##b() const { return { a(), b() }; }
#define SWIZZLE2_EXPAND1(a)   SWIZZLE2_EXPAND2(a,x); SWIZZLE2_EXPAND2(a,y); SWIZZLE2_EXPAND2(a,z); SWIZZLE2_EXPAND2(a,w)
#define SWIZZLER_SET_2        SWIZZLE2_EXPAND1(x);   SWIZZLE2_EXPAND1(y);   SWIZZLE2_EXPAND1(z);   SWIZZLE2_EXPAND1(w)

#define SWIZZLE3_EXPAND3(a,b,c) Vector3 a##b##c() const { return { a(), b(), c() }; }
#define SWIZZLE3_EXPAND2(a,b)   SWIZZLE3_EXPAND3(a,b,x); SWIZZLE3_EXPAND3(a,b,y); SWIZZLE3_EXPAND3(a,b,z); SWIZZLE3_EXPAND3(a,b,w)
#define SWIZZLE3_EXPAND1(a)     SWIZZLE3_EXPAND2(a,x);   SWIZZLE3_EXPAND2(a,y);   SWIZZLE3_EXPAND2(a,z);   SWIZZLE3_EXPAND2(a,w)
#define SWIZZLER_SET_3          SWIZZLE3_EXPAND1(x);     SWIZZLE3_EXPAND1(y);     SWIZZLE3_EXPAND1(z);     SWIZZLE3_EXPAND1(w)

#define SWIZZLE4_EXPAND4(a,b,c,d) Vector4 a##b##c##d() const { return { a(), b(), c(), d() }; }
#define SWIZZLE4_EXPAND3(a,b,c)   SWIZZLE4_EXPAND4(a,b,c,x); SWIZZLE4_EXPAND4(a,b,c,y); SWIZZLE4_EXPAND4(a,b,c,z); SWIZZLE4_EXPAND4(a,b,c,w)
#define SWIZZLE4_EXPAND2(a,b)     SWIZZLE4_EXPAND3(a,b,x);   SWIZZLE4_EXPAND3(a,b,y);   SWIZZLE4_EXPAND3(a,b,z);   SWIZZLE4_EXPAND3(a,b,w)
#define SWIZZLE4_EXPAND1(a)       SWIZZLE4_EXPAND2(a,x);     SWIZZLE4_EXPAND2(a,y);     SWIZZLE4_EXPAND2(a,z);     SWIZZLE4_EXPAND2(a,w)
#define SWIZZLER_SET_4            SWIZZLE4_EXPAND1(x);       SWIZZLE4_EXPAND1(y);       SWIZZLE4_EXPAND1(z);       SWIZZLE4_EXPAND1(w)

namespace eloo::Math
{
struct MatrixRow {
    inline explicit MatrixRow(float* firstRowValue, uint8_t length) : mValueStart(firstRowValue), mLength(length) {}
    inline float& operator[](uint8_t index) {
        ASSERT(index < mLength, "Index out of range: Trying to access non-existant column %i in a MatrixRow", index);
        return mValueStart[index];
    }

    // Swizzles
    SWIZZLER_SET_1;
    SWIZZLER_SET_2;
    SWIZZLER_SET_3;
    SWIZZLER_SET_4;

private:
    float* const mValueStart;
    const uint8_t mLength;
};

struct ConstMatrixRow {
    inline explicit ConstMatrixRow(const float* firstRowValue, uint8_t length) : mValueStart(firstRowValue), mLength(length) {}
    inline const float& operator[](uint8_t index) const {
        ASSERT(index < mLength, "Index out of range: Trying to access non-existant column %i in a MatrixRow", index);
        return mValueStart[index];
    }

    // Swizzles
    SWIZZLER_SET_1;
    SWIZZLER_SET_2;
    SWIZZLER_SET_3;
    SWIZZLER_SET_4;

private:
    const float* const mValueStart;
    const uint8_t mLength;
};


struct MatrixColumn {
	inline explicit MatrixColumn(float* firstColumnValue, uint8_t length) : mValueStart(firstColumnValue), mLength(length) {}
	inline float& operator[](uint8_t index) {
		ASSERT(index < mLength, "Index out of range: Trying to access non-existant row %i in a MatrixColumn", index);
		return mValueStart[index * mLength];
	}

	// Swizzles
	SWIZZLER_SET_1;
	SWIZZLER_SET_2;
	SWIZZLER_SET_3;
	SWIZZLER_SET_4;

private:
	float* const mValueStart;
	const uint8_t mLength;
};

struct ConstMatrixColumn {
	inline explicit ConstMatrixColumn(const float* firstColumnValue, uint8_t length) : mValueStart(firstColumnValue), mLength(length) {}
	inline const float& operator[](uint8_t index) const {
		ASSERT(index < mLength, "Index out of range: Trying to access non-existant row %i in a MatrixColumn", index);
		return mValueStart[index * mLength];
	}

	// Swizzles
	SWIZZLER_SET_1;
	SWIZZLER_SET_2;
	SWIZZLER_SET_3;
	SWIZZLER_SET_4;

private:
	const float* const mValueStart;
	const uint8_t mLength;
};
} // eloo::Math
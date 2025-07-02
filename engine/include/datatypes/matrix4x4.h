#pragma once

#include "datatypes/float4.h"

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale


// Helper for letting functions take matrix4X4 elements individually
#define MATRIX4X4_DECLARE_PARAMS(var) \
    float var##11, float var##12, float var##13, float var##14, \
    float var##21, float var##22, float var##23, float var##24, \
    float var##31, float var##32, float var##33, float var##34, \
    float var##41, float var##42, float var##43, float var##44

// Same as above, but with default identity values
#define MATRIX4X4_DECLARE_PARAMS_IDENTITY(var) \
    float var##11 = 1.0f, float var##12 = 0.0f, float var##13 = 0.0f, float var##14 = 0.0f, \
    float var##21 = 0.0f, float var##22 = 1.0f, float var##23 = 0.0f, float var##24 = 0.0f, \
    float var##31 = 0.0f, float var##32 = 0.0f, float var##33 = 1.0f, float var##34 = 0.0f, \
    float var##41 = 0.0f, float var##42 = 0.0f, float var##43 = 0.0f, float var##44 = 1.0f

// Helper that collates the input exploded values from function input, allowing
// the values to be passed to other functions
#define MATRIX4X4_FORWARD_PARAMS(var) \
    var##11, var##12, var##13, var##14, \
    var##21, var##22, var##23, var##24, \
    var##31, var##32, var##33, var##34, \
    var##41, var##42, var##43, var##44

// Helper to extract the individual elements from a matrix4X4
#define MATRIX4X4_UNPACK(matVar) \
    matVar[matrix4x4::R1C1], matVar[matrix4x4::R1C2], matVar[matrix4x4::R1C3], matVar[matrix4x4::R1C4], \
    matVar[matrix4x4::R2C1], matVar[matrix4x4::R2C2], matVar[matrix4x4::R2C3], matVar[matrix4x4::R2C4], \
    matVar[matrix4x4::R3C1], matVar[matrix4x4::R3C2], matVar[matrix4x4::R3C3], matVar[matrix4x4::R3C4], \
    matVar[matrix4x4::R4C1], matVar[matrix4x4::R4C2], matVar[matrix4x4::R4C3], matVar[matrix4x4::R4C4]


namespace eloo::float3 {
    struct values;
}

namespace eloo::matrix4x4 {
    ELOO_DECLARE_ID_T;

    enum cell_index {
        R1C1, R1C2, R1C3, R1C4,
        R2C1, R2C2, R2C3, R2C4,
        R3C1, R3C2, R3C3, R3C4,
        R4C1, R4C2, R4C3, R4C4,
        ROW_COUNT = 4,
        COLUMN_COUNT = 4,
        CELL_COUNT = ROW_COUNT * COLUMN_COUNT
    };

    using element_array_t = eastl::array<float, CELL_COUNT>;

    struct values {
    private:
        element_array_t mCells;

    public:
        constexpr values(MATRIX4X4_DECLARE_PARAMS_IDENTITY(v)) :
            mCells({ MATRIX4X4_FORWARD_PARAMS(v)}) {}

        float4::values row(int row);
        inline float4::values row1() { return { mCells[R1C1], mCells[R1C2], mCells[R1C3], mCells[R1C4] }; }
        inline float4::values row2() { return { mCells[R2C1], mCells[R2C2], mCells[R2C3], mCells[R2C4] }; }
        inline float4::values row3() { return { mCells[R3C1], mCells[R3C2], mCells[R3C3], mCells[R3C4] }; }
        inline float4::values row4() { return { mCells[R4C1], mCells[R4C2], mCells[R4C3], mCells[R4C4] }; }

        const float4::values row(int row) const;
        inline const float4::values row1() const { return { mCells[R1C1], mCells[R1C2], mCells[R1C3], mCells[R1C4] }; }
        inline const float4::values row2() const { return { mCells[R2C1], mCells[R2C2], mCells[R2C3], mCells[R2C4] }; }
        inline const float4::values row3() const { return { mCells[R3C1], mCells[R3C2], mCells[R3C3], mCells[R3C4] }; }
        inline const float4::values row4() const { return { mCells[R4C1], mCells[R4C2], mCells[R4C3], mCells[R4C4] }; }

        float4::values column(int column);
        inline float4::values column1() { return { mCells[R1C1], mCells[R2C1], mCells[R3C1], mCells[R4C1] }; }
        inline float4::values column2() { return { mCells[R1C2], mCells[R2C2], mCells[R3C2], mCells[R4C2] }; }
        inline float4::values column3() { return { mCells[R1C3], mCells[R2C3], mCells[R3C3], mCells[R4C3] }; }
        inline float4::values column4() { return { mCells[R1C4], mCells[R2C4], mCells[R3C4], mCells[R4C4] }; }

        const float4::values column(int column) const;
        inline const float4::values column1() const { return { mCells[R1C1], mCells[R2C1], mCells[R3C1], mCells[R4C1] }; }
        inline const float4::values column2() const { return { mCells[R1C2], mCells[R2C2], mCells[R3C2], mCells[R4C2] }; }
        inline const float4::values column3() const { return { mCells[R1C3], mCells[R2C3], mCells[R3C3], mCells[R4C3] }; }
        inline const float4::values column4() const { return { mCells[R1C4], mCells[R2C4], mCells[R3C4], mCells[R4C4] }; }

        float& cell(int index);
        const float& cell(int index) const;

        inline element_array_t& as_array() { return mCells; }
        inline const element_array_t& as_array() const { return mCells; }

    public:
        inline float& operator [] (int index) { return cell(index); }
        inline const float& operator [] (int index) const { return cell(index); }

    public:
        constexpr friend bool operator != (const values& lhs, const values& rhs);
        constexpr friend bool operator != (const values& lhs, float rhs);

        friend bool operator == (const values& lhs, const values& rhs);
        friend bool operator == (const values& lhs, float rhs);

        friend values operator + (const values& lhs);
        friend values operator - (const values& lhs);

        friend values operator / (const values& lhs, const values& rhs);
        friend values operator / (const values& lhs, float rhs);

        friend values operator * (const values& lhs, const values& rhs);
        friend values operator * (const values& lhs, float rhs);
        friend constexpr float3::values operator * (const values& lhs, const float3::values& rhs);
        friend constexpr float4::values operator * (const values& lhs, const float4::values& rhs);

        friend values operator + (const values& lhs, const values& rhs);
        friend values operator + (const values& lhs, float rhs);

        friend values operator - (const values& lhs, const values& rhs);
        friend values operator - (const values& lhs, float rhs);

    public:
        values& operator = (const values& other);

        values& operator + ();
        values& operator - ();

        values& operator /= (const values& other);
        values& operator /= (float values);

        values& operator *= (const values& other);
        values& operator *= (float values);

        values& operator += (const values& other);
        values& operator += (float values);

        values& operator -= (const values& other);
        values& operator -= (float values);
    };

    inline static constexpr values IDENTITY;
    inline static constexpr values ZERO     { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    inline static constexpr values ONE      { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

    id_t create(MATRIX4X4_DECLARE_PARAMS_IDENTITY(v));
    id_t create(const values& vals);
    bool release(id_t id);

    bool is_valid(id_t id);

    bool try_get_values(id_t id, values& vals);

    float& cell(id_t id, int index);
    float& cell(id_t id, int row, int column);
    float4::values row(id_t id, int index);
    float4::values column(id_t id, int index);

    const float& const_cell(id_t id, int index);
    const float& const_cell(id_t id, int row, int column);
    const float4::values const_row(id_t id, int index);
    const float4::values const_column(id_t id, int index);
}

using matrix4x4_v = eloo::matrix4x4::values;
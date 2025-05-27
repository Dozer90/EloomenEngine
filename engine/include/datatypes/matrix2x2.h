#pragma once

#include "datatypes/float2.h"

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale


// Helper for letting functions take matrix2x2 elements individually
#define MATRIX2X2_DECLARE_PARAMS(var) \
    float var##11, float var##12, \
    float var##21, float var##22

// Same as above, but with default identity values
#define MATRIX2X2_DECLARE_PARAMS_IDENTITY(var) \
    float var##11 = 1.0f, float var##12 = 0.0f, \
    float var##21 = 0.0f, float var##22 = 1.0f

// Helper that collates the input exploded values from function input, allowing
// the values to be passed to other functions
#define MATRIX2X2_FORWARD_PARAMS(var) \
    var##11, var##12, \
    var##21, var##22

// Helper to extract the individual elements from a matrix2x2
#define MATRIX2X2_UNPACK(matVar) \
    matVar[matrix2x2::R1C1], matVar[matrix2x2::R1C2], \
    matVar[matrix2x2::R2C1], matVar[matrix2x2::R2C2]


namespace eloo::matrix2x2 {
    ELOO_DECLARE_ID_T;

    enum cell_index {
        R1C1, R1C2,
        R2C1, R2C2,
        ROW_COUNT = 2,
        COLUMN_COUNT = 2,
        CELL_COUNT = ROW_COUNT * COLUMN_COUNT
    };

    using element_array_t = eastl::array<float, CELL_COUNT>;

    struct values {
    private:
        element_array_t mCells;

    public:
        constexpr values(MATRIX2X2_DECLARE_PARAMS_IDENTITY(v)) :
            mCells({ MATRIX2X2_FORWARD_PARAMS(v) }) {}

        float2::values row(int row);
        inline float2::values row1() { return { mCells[R1C1], mCells[R1C2] }; }
        inline float2::values row2() { return { mCells[R2C1], mCells[R2C2] }; }

        const float2::values row(int row) const;
        inline const float2::values row1() const { return { mCells[R1C1], mCells[R1C2] }; }
        inline const float2::values row2() const { return { mCells[R2C1], mCells[R2C2] }; }

        float2::values column(int column);
        inline float2::values column1() { return { mCells[R1C1], mCells[R2C1] }; }
        inline float2::values column2() { return { mCells[R1C2], mCells[R2C2] }; }

        const float2::values column(int column) const;
        inline const float2::values column1() const { return { mCells[R1C1], mCells[R2C1] }; }
        inline const float2::values column2() const { return { mCells[R1C2], mCells[R2C2] }; }

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
        friend constexpr float2::values operator * (const values& lhs, const float2::values& rhs);

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
    inline static constexpr values ZERO     { 0.0f, 0.0f, 0.0f, 0.0f };
    inline static constexpr values ONE      { 1.0f, 1.0f, 1.0f, 1.0f };

    id_t create(MATRIX2X2_DECLARE_PARAMS_IDENTITY(v), bool useIDPool = true);
    id_t create(const values& vals, bool useIDPool = true);
    bool try_release(id_t id);

    bool is_valid(id_t id);

    bool try_get_values(id_t id, values& vals);

    float& cell(id_t id, int index);
    float& cell(id_t id, cell_index index);
    float& cell(id_t id, int row, int column);
    float2::values row(id_t id, int index);
    float2::values column(id_t id, int index);

    const float& const_cell(id_t id, int index);
    const float& const_cell(id_t id, cell_index index);
    const float& const_cell(id_t id, int row, int column);
    const float2::values const_row(id_t id, int index);
    const float2::values const_column(id_t id, int index);
}

using matrix2x2_v = eloo::matrix2x2::values;
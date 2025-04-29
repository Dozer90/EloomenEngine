#pragma once

#include <datatype/float3.h>

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale


// Helper for letting functions take matrix3X3 elements individually
#define MATRIX3X3_DECLARE_PARAMS(var) \
    float var##11, float var##12, float var##13, \
    float var##21, float var##22, float var##23, \
    float var##31, float var##32, float var##33

// Same as above, but with default identity values
#define MATRIX3X3_DECLARE_PARAMS_IDENTITY(var) \
    float var##11 = 1.0f, float var##12 = 0.0f, float var##13 = 0.0f, \
    float var##21 = 0.0f, float var##22 = 1.0f, float var##23 = 0.0f, \
    float var##31 = 0.0f, float var##32 = 0.0f, float var##33 = 1.0f

// Helper that collates the input exploded values from function input, allowing
// the values to be passed to other functions
#define MATRIX3X3_FORWARD_PARAMS(var) \
    var##11, var##12, var##13, \
    var##21, var##22, var##23, \
    var##31, var##32, var##33

// Helper to extract the individual elements from a matrix3X3
#define MATRIX3X3_UNPACK(matVar) \
    matVar[matrix3x3::R1C1], matVar[matrix3x3::R1C2], matVar[matrix3x3::R1C3], \
    matVar[matrix3x3::R2C1], matVar[matrix3x3::R2C2], matVar[matrix3x3::R2C3], \
    matVar[matrix3x3::R3C1], matVar[matrix3x3::R3C2], matVar[matrix3x3::R3C3]


namespace eloo::matrix3x3 {
    ELOO_DECLARE_ID_T;

    enum cell_index {
        R1C1, R1C2, R1C3,
        R2C1, R2C2, R2C3,
        R3C1, R3C2, R3C3,
        ROW_COUNT = 3,
        COLUMN_COUNT = 3,
        CELL_COUNT = ROW_COUNT * COLUMN_COUNT
    };

    using element_array_t = eastl::array<float, CELL_COUNT>;

    struct values {
    private:
        element_array_t mCells;

    public:
        constexpr values(MATRIX3X3_DECLARE_PARAMS_IDENTITY(v)) :
            mCells({ MATRIX3X3_FORWARD_PARAMS(v)}) {}

        float3::values row(int row);
        inline float3::values row1() { return { mCells[R1C1], mCells[R1C2], mCells[R1C3] }; }
        inline float3::values row2() { return { mCells[R2C1], mCells[R2C2], mCells[R2C3] }; }
        inline float3::values row3() { return { mCells[R3C1], mCells[R3C2], mCells[R3C3] }; }

        const float3::values row(int row) const;
        inline const float3::values row1() const { return { mCells[R1C1], mCells[R1C2], mCells[R1C3] }; }
        inline const float3::values row2() const { return { mCells[R2C1], mCells[R2C2], mCells[R2C3] }; }
        inline const float3::values row3() const { return { mCells[R3C1], mCells[R3C2], mCells[R3C3] }; }

        float3::values column(int column);
        inline float3::values column1() { return { mCells[R1C1], mCells[R2C1], mCells[R3C1] }; }
        inline float3::values column2() { return { mCells[R1C2], mCells[R2C2], mCells[R3C2] }; }
        inline float3::values column3() { return { mCells[R1C3], mCells[R2C3], mCells[R3C3] }; }

        const float3::values column(int column) const;
        inline const float3::values column1() const { return { mCells[R1C1], mCells[R2C1], mCells[R3C1] }; }
        inline const float3::values column2() const { return { mCells[R1C2], mCells[R2C2], mCells[R3C2] }; }
        inline const float3::values column3() const { return { mCells[R1C3], mCells[R2C3], mCells[R3C3] }; }

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
    inline static constexpr values ZERO     { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    inline static constexpr values ONE      { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

    id_t create(MATRIX3X3_DECLARE_PARAMS_IDENTITY(v), bool useIDPool = true);
    id_t create(const values& vals, bool useIDPool = true);
    bool try_release(id_t id);

    bool is_valid(id_t id);

    bool try_get_values(id_t id, values& vals);

    float& cell(id_t id, int index);
    float& cell(id_t id, cell_index index);
    float& cell(id_t id, int row, int column);
    float3::values row(id_t id, int index);
    float3::values column(id_t id, int index);

    const float& const_cell(id_t id, int index);
    const float& const_cell(id_t id, cell_index index);
    const float& const_cell(id_t id, int row, int column);
    const float3::values const_row(id_t id, int index);
    const float3::values const_column(id_t id, int index);
}
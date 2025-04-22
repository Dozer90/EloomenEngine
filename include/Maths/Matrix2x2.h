#pragma once

#include <datatype/float2.h>

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale

namespace eloo::matrix2x2 {
    using id_t = size_t;

    struct valuess {
    private:
        float m11, m12;
        float m21, m22;

    public:
        constexpr valuess(float v11, float v12, float v21, float v22) : m11(v11), m12(v12), m21(v21), m22(v22) {}

        float2::values row(int index);
        inline float2::values row1() { return float2::values(m11, m12); }
        inline float2::values row2() { return float2::values(m21, m22); }

        const float2::values row(int index) const;
        inline const float2::values row1() const { return float2::values(m11, m12); }
        inline const float2::values row2() const { return float2::values(m21, m22); }

        float2::values col(int index);
        inline float2::values col1() { return float2::values(m11, m21); }
        inline float2::values col2() { return float2::values(m12, m22); }

        const float2::values col(int index) const;
        inline const float2::values col1() const { return float2::values(m11, m21); }
        inline const float2::values col2() const { return float2::values(m12, m22); }

        inline float& a11() { return m11; }
        inline float& a12() { return m12; }
        inline float& a21() { return m21; }
        inline float& a22() { return m22; }

        inline const float& a11() const { return m11; }
        inline const float& a12() const { return m12; }
        inline const float& a21() const { return m21; }
        inline const float& a22() const { return m22; }

    public:
        float& operator [] (int index);
        const float& operator [] (int index) const;

    public:
        constexpr friend bool operator != (const valuess& lhs, const valuess& rhs);
        constexpr friend bool operator != (const valuess& lhs, float rhs);

        friend bool operator == (const valuess& lhs, const valuess& rhs);
        friend bool operator == (const valuess& lhs, float rhs);

        friend valuess operator - (const valuess& lhs);

        friend valuess operator / (const valuess& lhs, const valuess& rhs);
        friend valuess operator / (const valuess& lhs, float rhs);

        friend valuess operator * (const valuess& lhs, const valuess& rhs);
        friend valuess operator * (const valuess& lhs, float rhs);

        friend valuess operator + (const valuess& lhs, const valuess& rhs);
        friend valuess operator + (const valuess& lhs, float rhs);

        friend valuess operator - (const valuess& lhs, const valuess& rhs);
        friend valuess operator - (const valuess& lhs, float rhs);

    public:
        valuess& operator = (const valuess& other);

        valuess& operator - ();

        valuess& operator /= (const valuess& other);
        valuess& operator /= (float valuess);

        valuess& operator *= (const valuess& other);
        valuess& operator *= (float valuess);

        valuess& operator += (const valuess& other);
        valuess& operator += (float valuess);

        valuess& operator -= (const valuess& other);
        valuess& operator -= (float valuess);
    };

    inline static constexpr valuess IDENTITY { 1.0f, 0.0f, 0.0f, 1.0f };
    inline static constexpr valuess ZERO     { 0.0f, 0.0f, 0.0f, 0.0f };
    inline static constexpr valuess ONE      { 1.0f, 1.0f, 1.0f, 1.0f };
    inline static constexpr valuess NaN      { FLT_NAN, FLT_NAN, FLT_NAN, FLT_NAN };

    id_t create(float v11 = 1.0f, float v12 = 0.0f, float v21 = 0.0f, float v22 = 1.0f, bool useIDPool = true);
    id_t create(const float2::values& row1, const float2::values& row2, bool useIDPool = true);
    id_t create(const valuess& vals, bool useIDPool = true);
    bool try_release(id_t id);

    valuess get_valuess(id_t id);

    float& cell(id_t id, int elementOffset);
    float& cell(id_t id, int rowIndex, int columnIndex);
    valuess row(id_t id, int rowIndex);
    valuess column(id_t id, int columnIndex);

    const float& const_values(id_t id, int elementOffset);
    const float& const_values(id_t id, int rowIndex, int columnIndex);
    const valuess const_row(id_t id, int rowIndex);
    const valuess const_column(id_t id, int columnIndex);
}
#pragma once

#include "utility/defines.h"

#include "datatypes/float3.h"
#include "datatypes/float4.h"

namespace eloo::quaternion {
    ELOO_DECLARE_ID_T;

    // OOP style container for quaternion
    struct values {
    private:
        float mX, mY, mZ, mW;

    public:
        constexpr values(float x, float y, float z, float w) : mX(x), mY(y), mZ(z), mW(w) {}
        values(const float4::values& vals) : mX(vals.x()), mY(vals.y()), mZ(vals.z()), mW(vals.w()) {}

        inline float& x() { return mX; }
        inline float& y() { return mY; }
        inline float& z() { return mZ; }
        inline float& w() { return mW; }
        inline const float& x() const { return mX; }
        inline const float& y() const { return mY; }
        inline const float& z() const { return mZ; }
        inline const float& w() const { return mW; }

    public:
        friend bool operator != (const values& lhs, const values& rhs);
        friend bool operator != (const values& lhs, float rhs);

        friend bool operator == (const values& lhs, const values& rhs);
        friend bool operator == (const values& lhs, float rhs);

        friend values operator + (const values& lhs);
        friend values operator - (const values& lhs);

        friend values operator / (const values& lhs, const values& rhs);
        friend values operator / (const values& lhs, float rhs);

        friend values operator * (const values& lhs, const values& rhs);
        friend values operator * (const values& lhs, float rhs);
        friend float3::values operator * (const values& lhs, const float3::values& rhs);

        friend values operator + (const values& lhs, const values& rhs);
        friend values operator + (const values& lhs, float rhs);

        friend values operator - (const values& lhs, const values& rhs);
        friend values operator - (const values& lhs, float rhs);

    public:
        values& operator = (const values& other);
        values& operator = (float values);

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

    inline static constexpr values IDENTITY { 0.0f, 0.0f, 0.0f, 1.0f };
    inline static constexpr values ZERO     { 0.0f, 0.0f, 0.0f, 0.0f };

    id_t create(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f, bool useIDPool = true);
    id_t create(const float4::values& vals, bool useIDPool = true);
    id_t create(float eulerX = 0.0f, float eulerY = 0.0f, float eulerZ = 0.0f, bool useIDPool = true);
    id_t create(const float3::values& vals, bool useIDPool = true);
    id_t create(const values& vals, bool useIDPool = true);
    bool try_release(id_t id);

    bool is_valid(id_t id);

    bool try_get_values(id_t id, values& vals);

    float& x(id_t id);
    float& y(id_t id);
    float& z(id_t id);
    float& w(id_t id);

    const float& const_x(id_t id);
    const float& const_y(id_t id);
    const float& const_z(id_t id);
    const float& const_w(id_t id);
};

using quaternion_v = eloo::quaternion::values;
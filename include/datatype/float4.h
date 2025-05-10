#pragma once

#include "utility/defines.h"

#include "datatype/float2.h"
#include "datatype/float3.h"

#include <EASTL/numeric_limits.h>
#include <EASTL/type_traits.h>


// Helper for letting functions take float4 elements individually
#define FLOAT4_DECLARE_PARAMS(var) \
    float var##X, float var##Y, float var##Z, float var##W

// Helper that collates the input exploded values from function input, allowing
// the values to be passed to other functions
#define FLOAT4_FORWARD_PARAMS(var) \
    var##X, var##Y, var##Z, var##W

// Helper to extract the individual elements from a float4
#define FLOAT4_UNPACK(f4Var) \
    f4Var.x(), f4Var.y(), f4Var.z(), f4Var.w()


namespace eloo::float4 {
    ELOO_DECLARE_ID_T;

    // OOP style container for float2
    struct values {
    private:
        float mX, mY, mZ, mW;

    public:
        constexpr values(float x, float y, float z, float w) : mX(x), mY(y), mZ(z), mW(w) {}

        inline float& x() { return mX; }
        inline float& y() { return mY; }
        inline float& z() { return mZ; }
        inline float& w() { return mW; }
        inline const float& x() const { return mX; }
        inline const float& y() const { return mY; }
        inline const float& z() const { return mZ; }
        inline const float& w() const { return mW; }

        inline const float3::values xyz() const { return { mX, mY, mZ }; }
        inline const float2::values xy() const { return { mX, mY }; }

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

    template <typename T> concept storage_t = eastl::is_same_v<T, values>;

    inline static constexpr values ZERO     { 0.0f, 0.0f, 0.0f, 0.0f };
    inline static constexpr values ONE      { 1.0f, 1.0f, 1.0f, 1.0f };
    inline static constexpr values RIGHT    { 1.0f, 0.0f, 0.0f, 0.0f };
    inline static constexpr values UP       { 0.0f, 1.0f, 0.0f, 0.0f };
    inline static constexpr values FORWARD  { 0.0f, 0.0f, 1.0f, 0.0f };

    id_t create(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f, bool useIDPool = true);
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
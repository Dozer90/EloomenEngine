#pragma once

#include "utility/defines.h"

#include "datatypes/float2.h"

#include <EASTL/numeric_limits.h>
#include <EASTL/type_traits.h>


// Helper for letting functions take float3 elements individually
#define FLOAT3_DECLARE_PARAMS(var) \
    float var##X, float var##Y, float var##Z

// Helper that collates the input exploded values from function input, allowing
// the values to be passed to other functions
#define FLOAT3_FORWARD_PARAMS(var) \
    var##X, var##Y, var##Z

// Helper to extract the individual elements from a float3
#define FLOAT3_UNPACK(f3Var) \
    f3Var.x(), f3Var.y(), f3Var.z()


namespace eloo::float3 {
    ELOO_DECLARE_ID_T;

    // OOP style container for float2
    struct values {
    private:
        float mX, mY, mZ;

    public:
        constexpr values(float x, float y, float z) : mX(x), mY(y), mZ(z) {}

        inline float& x() { return mX; }
        inline float& y() { return mY; }
        inline float& z() { return mZ; }
        inline const float& x() const { return mX; }
        inline const float& y() const { return mY; }
        inline const float& z() const { return mZ; }

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

    inline static constexpr values ZERO     { 0.0f, 0.0f, 0.0f };
    inline static constexpr values ONE      { 1.0f, 1.0f, 1.0f };
    inline static constexpr values RIGHT    { 1.0f, 0.0f, 0.0f };
    inline static constexpr values UP       { 0.0f, 1.0f, 0.0f };
    inline static constexpr values FORWARD  { 0.0f, 0.0f, 1.0f };

    id_t create(float x = 0.0f, float y = 0.0f, float z = 0.0f, bool useIDPool = true);
    id_t create(const values& vals, bool useIDPool = true);
    bool try_release(id_t id);

    bool is_valid(id_t id);

    bool try_get_values(id_t id, values& vals);

    float& x(id_t id);
    float& y(id_t id);
    float& z(id_t id);

    const float& const_x(id_t id);
    const float& const_y(id_t id);
    const float& const_z(id_t id);
};

using float3_v = eloo::float3::values;
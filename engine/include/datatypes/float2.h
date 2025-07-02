#pragma once

#include "utility/defines.h"

#include <EASTL/numeric_limits.h>
#include <EASTL/type_traits.h>


// Helper for letting functions take float2 elements individually
#define FLOAT2_DECLARE_PARAMS(var) \
    float var##X, float var##Y

// Helper that collates the input exploded values from function input, allowing
// the values to be passed to other functions
#define FLOAT2_FORWARD_PARAMS(var) \
    var##X, var##Y

// Helper to extract the individual elements from a float2
#define FLOAT2_UNPACK(f2Var) \
    f2Var.x(), f2Var.y()


namespace eloo::float2 {
    ELOO_DECLARE_ID_T;

    // OOP style container for float2
    struct values {
    private:
        float mX, mY;

    public:
        constexpr values(float x, float y) : mX(x), mY(y) {}

        inline float& x() { return mX; }
        inline float& y() { return mY; }
        inline const float& x() const { return mX; }
        inline const float& y() const { return mY; }

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

    inline static constexpr values ZERO     { 0.0f, 0.0f };
    inline static constexpr values ONE      { 1.0f, 1.0f };
    inline static constexpr values RIGHT    { 1.0f, 0.0f };
    inline static constexpr values UP       { 0.0f, 1.0f };

    id_t create(float x = 0.0f, float y = 0.0f);
    id_t create(const values& vals);
    bool release(id_t id);

    bool is_valid(id_t id);

    bool try_get_values(id_t id, values& vals);

    float& x(id_t id);
    float& y(id_t id);

    const float& const_x(id_t id);
    const float& const_y(id_t id);
};

using float2_v = eloo::float2::values;
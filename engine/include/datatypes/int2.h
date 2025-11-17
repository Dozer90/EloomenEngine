#pragma once

#include "utility/defines.h"

#include <EASTL/numeric_limits.h>
#include <EASTL/type_traits.h>

namespace eloo::int2 {
    ELOO_DECLARE_ID_T;

    // OOP style container for int2
    struct values {
    private:
        int mX, mY;

    public:
        constexpr values(int x, int y) : mX(x), mY(y) {}

        inline int& x() { return mX; }
        inline int& y() { return mY; }
        inline const int& x() const { return mX; }
        inline const int& y() const { return mY; }

    public:
        friend bool operator != (const values& lhs, const values& rhs);
        friend bool operator != (const values& lhs, int rhs);

        friend bool operator == (const values& lhs, const values& rhs);
        friend bool operator == (const values& lhs, int rhs);

        friend values operator + (const values& lhs);
        friend values operator - (const values& lhs);

        friend values operator / (const values& lhs, const values& rhs);
        friend values operator / (const values& lhs, int rhs);
        friend values operator / (const values& lhs, float rhs);

        friend values operator * (const values& lhs, const values& rhs);
        friend values operator * (const values& lhs, int rhs);
        friend values operator * (const values& lhs, float rhs);

        friend values operator + (const values& lhs, const values& rhs);
        friend values operator + (const values& lhs, int rhs);
        friend values operator + (const values& lhs, float rhs);

        friend values operator - (const values& lhs, const values& rhs);
        friend values operator - (const values& lhs, int rhs);
        friend values operator - (const values& lhs, float rhs);

    public:
        values& operator = (const values& other);
        // CODE REVIEW: Parameter name 'values' shadows the struct name 'values'
        // This makes the code confusing. Consider renaming to 'value' or 'scalar'.
        values& operator = (int values);

        // CODE REVIEW: Missing implementation for operator+()
        // This is declared but not implemented in int2.cpp, same issue as float2.
        // This will cause linker errors when used.
        values& operator + ();
        values& operator - ();

        values& operator /= (const values& other);
        values& operator /= (int values);
        // CODE REVIEW: Missing implementation for float overload
        // Declared here but not implemented in int2.cpp. Only int versions exist (lines 185-192).
        values& operator /= (float values);

        values& operator *= (const values& other);
        values& operator *= (int values);
        // CODE REVIEW: Missing implementation for float overload
        // Declared here but not implemented in int2.cpp. Only int versions exist (lines 194-201).
        values& operator *= (float values);

        values& operator += (const values& other);
        values& operator += (int values);
        // CODE REVIEW: Missing implementation for float overload
        // Declared here but not implemented in int2.cpp. Only int versions exist (lines 203-210).
        values& operator += (float values);

        values& operator -= (const values& other);
        values& operator -= (int values);
        // CODE REVIEW: Missing implementation for float overload
        // Declared here but not implemented in int2.cpp. Only int versions exist (lines 212-219).
        values& operator -= (float values);
    };

    template <typename T> concept storage_t = eastl::is_same_v<T, values>;

    inline static constexpr values ZERO     { 0, 0 };
    inline static constexpr values ONE      { 1, 1 };
    inline static constexpr values RIGHT    { 1, 0 };
    inline static constexpr values UP       { 0, 1 };

    id_t create(int x = 0, int y = 0, bool useIDPool = true);
    id_t create(const values& vals, bool useIDPool = true);
    bool try_release(id_t id);

    bool is_valid(id_t id);

    bool try_get_values(id_t id, values& vals);

    int& x(id_t id);
    int& y(id_t id);

    const int& const_x(id_t id);
    const int& const_y(id_t id);
};

using int2_v = eloo::int2::values;
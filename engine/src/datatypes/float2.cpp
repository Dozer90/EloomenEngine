#include "maths/math.h"
#include "utility/managed_memory_block.h"

using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 50;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.7f;
    using memblock_t = managed_memory_block<float, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>;
    static memblock_t gMemoryBlockX, gMemoryBlockY;
}

float2::id_t float2::create(float x, float y) {
    id_t id = gMemoryBlockX.push(x);
    ELOO_ASSERT_FATAL(id == gMemoryBlockY.push(y), "ID mismatch between X and Y memory blocks");
    return id;
}

inline float2::id_t float2::create(const values& vals) {
    return create(vals.x(), vals.y());
}

bool float2::release(id_t id) {
    if (!is_valid(id)) {
        return false;
    }
    gMemoryBlockX.remove(id);
    gMemoryBlockY.remove(id);
    return true;
}

inline bool float2::try_get_values(id_t id, float2::values& vals) {
    if (is_valid(id)) {
        vals = {
            gMemoryBlockX.get(id),
            gMemoryBlockY.get(id)
        };
        return true;
    }
    return false;
}

inline float& float2::x(id_t id) { return gMemoryBlockX.get(id); }
inline float& float2::y(id_t id) { return gMemoryBlockY.get(id); }

inline const float& float2::const_x(id_t id) { return gMemoryBlockX.get(id); }
inline const float& float2::const_y(id_t id) { return gMemoryBlockY.get(id); }


///////////////////////////////////////////////////////
// Values container

bool float2::operator != (const float2::values& lhs, const float2::values& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y();
}
bool float2::operator != (const float2::values& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs;
}

bool float2::operator == (const float2::values& lhs, const float2::values& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y();
}
bool float2::operator == (const float2::values& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs;
}

float2::values float2::operator + (const float2::values& lhs) {
    return lhs;
}
float2::values float2::operator - (const float2::values& lhs) {
    return {
        -lhs.x(),
        -lhs.y()
    };
}

float2::values float2::operator / (const float2::values& lhs, const float2::values& rhs) {
    return {
        lhs.x() / rhs.x(),
        lhs.y() / rhs.y()
    };
}
float2::values float2::operator / (const float2::values& lhs, float rhs) {
    return {
        lhs.x() / rhs,
        lhs.y() / rhs
    };
}

float2::values float2::operator * (const float2::values& lhs, const float2::values& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y()
    };
}
float2::values float2::operator * (const float2::values& lhs, float rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs
    };
}

float2::values float2::operator + (const float2::values& lhs, const float2::values& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y()
    };
}
float2::values float2::operator + (const float2::values& lhs, float rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs
    };
}

float2::values float2::operator - (const float2::values& lhs, const float2::values& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y()
    };
}
float2::values float2::operator - (const float2::values& lhs, float rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs
    };
}


///////////////////////////////////////////////////////
// Assignment and manipulation float2::operators

float2::values& float2::values::operator = (const values& other) {
    this->mX = other.mX;
    this->mY = other.mY;
    return *this;
}
float2::values& float2::values::operator = (float val) {
    this->mX = val;
    this->mY = val;
    return *this;
}

float2::values& float2::values::operator - () {
    this->mX = -this->mX;
    this->mY = -this->mY;
    return *this;
}

float2::values& float2::values::operator /= (const values& other) {
    *this = *this / other;
    return *this;
}
float2::values& float2::values::operator /= (float val) {
    *this = *this / val;
    return *this;
}

float2::values& float2::values::operator *= (const values& other) {
    *this = *this * other;
    return *this;
}
float2::values& float2::values::operator *= (float val) {
    *this = *this * val;
    return *this;
}

float2::values& float2::values::operator += (const values& other) {
    *this = *this + other;
    return *this;
}
float2::values& float2::values::operator += (float val) {
    *this = *this + val;
    return *this;
}

float2::values& float2::values::operator -= (const values& other) {
    *this = *this - other;
    return *this;
}
float2::values& float2::values::operator -= (float val) {
    *this = *this - val;
    return *this;
}
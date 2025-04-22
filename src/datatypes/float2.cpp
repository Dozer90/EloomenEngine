#include <maths/math.h>
#include <utility/managed_memory_block.h>

using namespace eloo;

namespace {
    constexpr int gMemoryBlockInitialSize = 50;
    constexpr float gMemoryBlockExpansionScalar = 0.7f;
    using memblock_t = managed_memory_block<float, gMemoryBlockInitialSize, gMemoryBlockExpansionScalar>;
    static memblock_t gMemoryBlockX, gMemoryBlockY;
}

float2::id_t float2::create(float x, float y, bool useIDPool) {
    id_t id = gMemoryBlockX.push(x, useIDPool);
    ASSERT_FATAL(id == gMemoryBlockY.push(y, useIDPool), "ID mismatch between X and Y memory blocks");
    return id;
}

inline float2::id_t float2::create(const values& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), useIDPool);
}

bool float2::try_release(id_t id) {
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

bool operator != (const float2::values& lhs, const float2::values& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y();
}
bool operator != (const float2::values& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs;
}

bool operator == (const float2::values& lhs, const float2::values& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y();
}
bool operator == (const float2::values& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs;
}

float2::values operator - (const float2::values& lhs) {
    return {
        -lhs.x(),
        -lhs.y()
    };
}

float2::values operator / (const float2::values& lhs, const float2::values& rhs) {
    return {
        lhs.x() / rhs.x(),
        lhs.y() / rhs.y()
    };
}
float2::values operator / (const float2::values& lhs, float rhs) {
    return {
        lhs.x() / rhs,
        lhs.y() / rhs
    };
}

float2::values operator * (const float2::values& lhs, const float2::values& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y()
    };
}
float2::values operator * (const float2::values& lhs, float rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs
    };
}

float2::values operator + (const float2::values& lhs, const float2::values& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y()
    };
}
float2::values operator + (const float2::values& lhs, float rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs
    };
}

float2::values operator - (const float2::values& lhs, const float2::values& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y()
    };
}
float2::values operator - (const float2::values& lhs, float rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs
    };
}

float2::values& float2::values::operator = (const values& other) {
    mX = other.x();
    mY = other.y();
    return *this;
}
float2::values& float2::values::operator = (float val) {
    mX = val;
    mY = val;
    return *this;
}

float2::values& float2::values::operator - () {
    mX = -mX;
    mY = -mY;
    return *this;
}

float2::values& float2::values::operator /= (const values& other) {
    mX /= other.x();
    mY /= other.y();
    return *this;
}
float2::values& float2::values::operator /= (float val) {
    mX /= val;
    mY /= val;
    return *this;
}

float2::values& float2::values::operator *= (const values& other) {
    mX *= other.x();
    mY *= other.y();
    return *this;
}
float2::values& float2::values::operator *= (float val) {
    mX *= val;
    mY *= val;
    return *this;
}

float2::values& float2::values::operator += (const values& other) {
    mX += other.x();
    mY += other.y();
    return *this;
}
float2::values& float2::values::operator += (float val) {
    mX += val;
    mY += val;
    return *this;
}

float2::values& float2::values::operator -= (const values& other) {
    mX -= other.x();
    mY -= other.y();
    return *this;
}
float2::values& float2::values::operator -= (float val) {
    mX -= val;
    mY -= val;
    return *this;
}


///////////////////////////////////////////////////////
// Assignment and manipulation operators

float2::values& float2::values::operator = (const values& other) {
    mX = other.x();
    mY = other.y();
    return *this;
}
float2::values& float2::values::operator = (float val) {
    mX = val;
    mY = val;
    return *this;
}

float2::values& float2::values::operator + () {
    return *this;
}
float2::values& float2::values::operator - () {
    mX = -mX;
    mY = -mY;
    return *this;
}

float2::values& float2::values::operator /= (const values& other) {
    mX /= other.x();
    mY /= other.y();
    return *this;
}
float2::values& float2::values::operator /= (float val) {
    mX /= val;
    mY /= val;
    return *this;
}

float2::values& float2::values::operator *= (const values& other) {
    mX *= other.x();
    mY *= other.y();
    return *this;
}
float2::values& float2::values::operator *= (float val) {
    mX *= val;
    mY *= val;
    return *this;
}

float2::values& float2::values::operator += (const values& other) {
    mX += other.x();
    mY += other.y();
    return *this;
}
float2::values& float2::values::operator += (float val) {
    mX += val;
    mY += val;
    return *this;
}

float2::values& float2::values::operator -= (const values& other) {
    mX -= other.x();
    mY -= other.y();
    return *this;
}
float2::values& float2::values::operator -= (float val) {
    mX -= val;
    mY -= val;
    return *this;
}
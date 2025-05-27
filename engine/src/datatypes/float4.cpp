#include "maths/math.h"
#include "utility/managed_memory_block.h"

using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 50;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.7f;
    using memblock_t = managed_memory_block<float, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>;
    static memblock_t gMemoryBlockX, gMemoryBlockY, gMemoryBlockZ, gMemoryBlockW;
}

float4::id_t float4::create(float x, float y, float z, float w, bool useIDPool) {
    id_t id = gMemoryBlockX.push(x, useIDPool);
    ELOO_ASSERT_FATAL(id == gMemoryBlockY.push(y, useIDPool), "ID mismatch between X and Y memory blocks.");
    ELOO_ASSERT_FATAL(id == gMemoryBlockZ.push(z, useIDPool), "ID mismatch between X and Z memory blocks.");
    ELOO_ASSERT_FATAL(id == gMemoryBlockW.push(w, useIDPool), "ID mismatch between X and W memory blocks.");
    return id;
}

float4::id_t float4::create(const values& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), vals.z(), vals.w(), useIDPool);
}

bool float4::try_release(id_t id) {
    if (!is_valid(id)) {
        return false;
    }
    gMemoryBlockX.remove(id);
    gMemoryBlockY.remove(id);
    gMemoryBlockZ.remove(id);
    gMemoryBlockW.remove(id);
    return true;
}

bool float4::try_get_values(id_t id, float4::values& vals) {
    if (is_valid(id)) {
        vals = {
            gMemoryBlockX.get(id),
            gMemoryBlockY.get(id),
            gMemoryBlockZ.get(id),
            gMemoryBlockW.get(id)
        };
        return true;
    }
    return false;
}

float& float4::x(id_t id) { return gMemoryBlockX.get(id); }
float& float4::y(id_t id) { return gMemoryBlockY.get(id); }
float& float4::z(id_t id) { return gMemoryBlockZ.get(id); }
float& float4::w(id_t id) { return gMemoryBlockW.get(id); }

const float& float4::const_x(id_t id) { return gMemoryBlockX.get(id); }
const float& float4::const_y(id_t id) { return gMemoryBlockY.get(id); }
const float& float4::const_z(id_t id) { return gMemoryBlockZ.get(id); }
const float& float4::const_w(id_t id) { return gMemoryBlockW.get(id); }


///////////////////////////////////////////////////////
// Values container

bool float4::operator != (const float4::values& lhs, const float4::values& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z() ||
        lhs.w() != rhs.w();
}
bool float4::operator != (const float4::values& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs ||
        lhs.z() != rhs ||
        lhs.w() != rhs;
}

bool float4::operator == (const float4::values& lhs, const float4::values& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z() &&
        lhs.w() == rhs.w();
}
bool float4::operator == (const float4::values& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs &&
        lhs.z() == rhs &&
        lhs.w() == rhs;
}

float4::values float4::operator + (const float4::values& lhs) {
    return lhs;
}
float4::values float4::operator - (const float4::values& lhs) {
    return {
        -lhs.x(),
        -lhs.y(),
        -lhs.z(),
        -lhs.w()
    };
}

float4::values float4::operator / (const float4::values& lhs, const float4::values& rhs) {
    return {
        lhs.x() / rhs.x(),
        lhs.y() / rhs.y(),
        lhs.z() / rhs.z(),
        lhs.w() / rhs.w()
    };
}
float4::values float4::operator / (const float4::values& lhs, float rhs) {

    return {
        lhs.x() / rhs,
        lhs.y() / rhs,
        lhs.z() / rhs,
        lhs.w() / rhs
    };
}

float4::values float4::operator * (const float4::values& lhs, const float4::values& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y(),
        lhs.z() * rhs.z(),
        lhs.w() * rhs.w()
    };
}
float4::values float4::operator * (const float4::values& lhs, float rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs,
        lhs.z() * rhs,
        lhs.w() * rhs
    };
}

float4::values float4::operator + (const float4::values& lhs, const float4::values& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z(),
        lhs.w() + rhs.w()
    };
}
float4::values float4::operator + (const float4::values& lhs, float rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs,
        lhs.z() + rhs,
        lhs.w() + rhs
    };
}

float4::values float4::operator - (const float4::values& lhs, const float4::values& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z(),
        lhs.w() - rhs.w()
    };
}
float4::values float4::operator - (const float4::values& lhs, float rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs,
        lhs.z() - rhs,
        lhs.w() - rhs
    };
}


///////////////////////////////////////////////////////
// Assignment and manipulation float4::operators

float4::values& float4::values::operator = (const values& other) {
    this->mX = other.mX;
    this->mY = other.mY;
    this->mZ = other.mZ;
    this->mW = other.mW;
    return *this;
}
float4::values& float4::values::operator = (float val) {
    this->mX = val;
    this->mY = val;
    this->mZ = val;
    this->mW = val;
    return *this;
}

float4::values& float4::values::operator - () {
    this->mX = -this->mX;
    this->mY = -this->mY;
    this->mZ = -this->mZ;
    this->mW = -this->mW;
    return *this;
}

float4::values& float4::values::operator /= (const values& other) {
    *this = *this / other;
    return *this;
}
float4::values& float4::values::operator /= (float val) {
    *this = *this / val;
    return *this;
}

float4::values& float4::values::operator *= (const values& other) {
    *this = *this * other;
    return *this;
}
float4::values& float4::values::operator *= (float val) {
    *this = *this * val;
    return *this;
}

float4::values& float4::values::operator += (const values& other) {
    *this = *this + other;
    return *this;
}
float4::values& float4::values::operator += (float val) {
    *this = *this + val;
    return *this;
}

float4::values& float4::values::operator -= (const values& other) {
    *this = *this - other;
    return *this;
}
float4::values& float4::values::operator -= (float val) {
    *this = *this - val;
    return *this;
}
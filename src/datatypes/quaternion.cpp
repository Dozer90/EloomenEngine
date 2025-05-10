#include "maths/math.h"
#include "utility/managed_memory_block.h"

using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 50;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.7f;
    using memblock_t = managed_memory_block<float, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>;
    static memblock_t gMemoryBlockX, gMemoryBlockY, gMemoryBlockZ, gMemoryBlockW;
}

quaternion::id_t quaternion::create(float x, float y, float z, float w, bool useIDPool) {
    id_t id = gMemoryBlockX.push(x, useIDPool);
    ELOO_ASSERT_FATAL(id == gMemoryBlockY.push(y, useIDPool), "ID mismatch between X and Y memory blocks.");
    ELOO_ASSERT_FATAL(id == gMemoryBlockZ.push(z, useIDPool), "ID mismatch between X and Z memory blocks.");
    ELOO_ASSERT_FATAL(id == gMemoryBlockW.push(w, useIDPool), "ID mismatch between X and W memory blocks.");
    return id;
}
quaternion::id_t quaternion::create(const float4::values& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), vals.z(), vals.w(), useIDPool);
}
quaternion::id_t quaternion::create(float eulerX, float eulerY, float eulerZ, bool useIDPool) {
    return create(math::quaternion::create_from_euler(eulerX, eulerY, eulerZ), useIDPool);
}
quaternion::id_t quaternion::create(const float3::values& euler, bool useIDPool) {
    return create(euler.x(), euler.y(), euler.z(), useIDPool);
}
quaternion::id_t quaternion::create(const quaternion::values& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), vals.z(), vals.w(), useIDPool);
}

bool quaternion::try_release(id_t id) {
    if (!is_valid(id)) {
        return false;
    }
    gMemoryBlockX.remove(id);
    gMemoryBlockY.remove(id);
    gMemoryBlockZ.remove(id);
    gMemoryBlockW.remove(id);
    return true;
}

bool quaternion::try_get_values(id_t id, quaternion::values& vals) {
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

float& quaternion::x(id_t id) { return gMemoryBlockX.get(id); }
float& quaternion::y(id_t id) { return gMemoryBlockY.get(id); }
float& quaternion::z(id_t id) { return gMemoryBlockZ.get(id); }
float& quaternion::w(id_t id) { return gMemoryBlockW.get(id); }

const float& quaternion::const_x(id_t id) { return gMemoryBlockX.get(id); }
const float& quaternion::const_y(id_t id) { return gMemoryBlockY.get(id); }
const float& quaternion::const_z(id_t id) { return gMemoryBlockZ.get(id); }
const float& quaternion::const_w(id_t id) { return gMemoryBlockW.get(id); }


///////////////////////////////////////////////////////
// Values container

bool quaternion::operator != (const quaternion::values& lhs, const quaternion::values& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z() ||
        lhs.w() != rhs.w();
}
bool quaternion::operator != (const quaternion::values& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs ||
        lhs.z() != rhs ||
        lhs.w() != rhs;
}

bool quaternion::operator == (const quaternion::values& lhs, const quaternion::values& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z() &&
        lhs.w() == rhs.w();
}
bool quaternion::operator == (const quaternion::values& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs &&
        lhs.z() == rhs &&
        lhs.w() == rhs;
}

quaternion::values quaternion::operator + (const quaternion::values& lhs) {
    return lhs;
}
quaternion::values quaternion::operator - (const quaternion::values& lhs) {
    return {
        -lhs.x(),
        -lhs.y(),
        -lhs.z(),
        -lhs.w()
    };
}

quaternion::values quaternion::operator / (const quaternion::values& lhs, const quaternion::values& rhs) {
    return {
        lhs.x() / rhs.x(),
        lhs.y() / rhs.y(),
        lhs.z() / rhs.z(),
        lhs.w() / rhs.w()
    };
}
quaternion::values quaternion::operator / (const quaternion::values& lhs, float rhs) {

    return {
        lhs.x() / rhs,
        lhs.y() / rhs,
        lhs.z() / rhs,
        lhs.w() / rhs
    };
}

quaternion::values quaternion::operator * (const quaternion::values& lhs, const quaternion::values& rhs) {
    return {
        lhs.mX * rhs.mW + lhs.mY * rhs.mZ - lhs.mZ * rhs.mY + lhs.mW * rhs.mX,
        lhs.mX * rhs.mY - lhs.mY * rhs.mW + lhs.mZ * rhs.mX + lhs.mW * rhs.mZ,
        lhs.mX * rhs.mZ + lhs.mY * rhs.mX - lhs.mZ * rhs.mW + lhs.mW * rhs.mY,
        lhs.mX * rhs.mW - lhs.mY * rhs.mZ + lhs.mZ * rhs.mY + lhs.mW * rhs.mX
    };
}
quaternion::values quaternion::operator * (const quaternion::values& lhs, float rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs,
        lhs.z() * rhs,
        lhs.w() * rhs
    };
}
float3::values quaternion::operator * (const quaternion::values& lhs, const float3::values& rhs) {
    quaternion::values result = { rhs.x(), rhs.y(), rhs.z(), 0.0f };
    const quaternion::values conjugate = { -lhs.x(), -lhs.y(), -lhs.z(), lhs.w() };
    result = lhs * result * conjugate;
    return {
        result.x(),
        result.y(),
        result.z()
    };
}

quaternion::values quaternion::operator + (const quaternion::values& lhs, const quaternion::values& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z(),
        lhs.w() + rhs.w()
    };
}
quaternion::values quaternion::operator + (const quaternion::values& lhs, float rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs,
        lhs.z() + rhs,
        lhs.w() + rhs
    };
}

quaternion::values quaternion::operator - (const quaternion::values& lhs, const quaternion::values& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z(),
        lhs.w() - rhs.w()
    };
}
quaternion::values quaternion::operator - (const quaternion::values& lhs, float rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs,
        lhs.z() - rhs,
        lhs.w() - rhs
    };
}


///////////////////////////////////////////////////////
// Assignment and manipulation quaternion::operators

quaternion::values& quaternion::values::operator = (const values& other) {
    *this = other;
    return *this;
}
quaternion::values& quaternion::values::operator = (float val) {
    *this = val;
    return *this;
}

quaternion::values& quaternion::values::operator + () {
    return *this;
}
quaternion::values& quaternion::values::operator - () {
    *this = -*this;
    return *this;
}

quaternion::values& quaternion::values::operator /= (const values& other) {
    *this = *this - other;
    return *this;
}
quaternion::values& quaternion::values::operator /= (float val) {
    *this = *this - val;
    return *this;
}

quaternion::values& quaternion::values::operator *= (const values& other) {
    *this = *this * other;
    return *this;
}
quaternion::values& quaternion::values::operator *= (float val) {
    *this = *this * val;
    return *this;
}

quaternion::values& quaternion::values::operator += (const values& other) {
    *this = *this + other;
    return *this;
}
quaternion::values& quaternion::values::operator += (float val) {
    *this = *this + val;
    return *this;
}

quaternion::values& quaternion::values::operator -= (const values& other) {
    *this = *this - other;
    return *this;
}
quaternion::values& quaternion::values::operator -= (float val) {
    *this = *this - val;
    return *this;
}
#include <maths/math.h>
#include <utility/managed_memory_block.h>

using namespace eloo;

namespace {
    constexpr int gMemoryBlockInitialSize = 50;
    constexpr float gMemoryBlockExpansionScalar = 0.7f;
    using memblock_t = managed_memory_block<float, gMemoryBlockInitialSize, gMemoryBlockExpansionScalar>;
    static memblock_t gMemoryBlockX, gMemoryBlockY, gMemoryBlockZ;
}

float3::id_t float3::create(float x, float y, float z, bool useIDPool) {
    id_t id = gMemoryBlockX.push(x, useIDPool);
    ASSERT_FATAL(id == gMemoryBlockY.push(y, useIDPool), "ID mismatch between X and Y memory blocks");
    ASSERT_FATAL(id == gMemoryBlockZ.push(z, useIDPool), "ID mismatch between X and Z memory blocks");
    return id;
}

float3::id_t float3::create(const values& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), vals.z(), useIDPool);
}

bool float3::try_release(id_t id) {
    if (!is_valid(id)) {
        return false;
    }
    gMemoryBlockX.remove(id);
    gMemoryBlockY.remove(id);
    gMemoryBlockZ.remove(id);
    return true;
}

bool float3::try_get_values(id_t id, float3::values& vals) {
    if (is_valid(id)) {
        vals = {
            gMemoryBlockX.get(id),
            gMemoryBlockY.get(id),
            gMemoryBlockZ.get(id)
        };
        return true;
    }
    return false;
}

float& float3::x(id_t id) { return gMemoryBlockX.get(id); }
float& float3::y(id_t id) { return gMemoryBlockY.get(id); }
float& float3::z(id_t id) { return gMemoryBlockZ.get(id); }

const float& float3::const_x(id_t id) { return gMemoryBlockX.get(id); }
const float& float3::const_y(id_t id) { return gMemoryBlockY.get(id); }
const float& float3::const_z(id_t id) { return gMemoryBlockZ.get(id); }


/////////////////////////////////////////////////////////////////
// float3::values

bool operator != (const float3::values& lhs, const float3::values& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z();
}
bool operator != (const float3::values& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs ||
        lhs.z() != rhs;
}

bool operator == (const float3::values& lhs, const float3::values& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z();
}
bool operator == (const float3::values& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs &&
        lhs.z() == rhs;
}

float3::values operator - (const float3::values& lhs) {
    return {
        -lhs.x(),
        -lhs.y(),
        -lhs.z()
    };
}

float3::values operator / (const float3::values& lhs, const float3::values& rhs) {
    return {
        lhs.x() / rhs.x(),
        lhs.y() / rhs.y(),
        lhs.z() / rhs.z()
    };
}
float3::values operator / (const float3::values& lhs, float rhs) {
    return {
        lhs.x() / rhs,
        lhs.y() / rhs,
        lhs.z() / rhs
    };
}

float3::values operator * (const float3::values& lhs, const float3::values& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y(),
        lhs.z() * rhs.z()
    };
}
float3::values operator * (const float3::values& lhs, float rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs,
        lhs.z() * rhs
    };
}

float3::values operator + (const float3::values& lhs, const float3::values& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z()
    };
}
float3::values operator + (const float3::values& lhs, float rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs,
        lhs.z() + rhs
    };
}

float3::values operator - (const float3::values& lhs, const float3::values& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z()
    };
}
float3::values operator - (const float3::values& lhs, float rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs,
        lhs.z() - rhs
    };
}

float3::values& float3::values::operator - () {
    mX = -mX;
    mY = -mY;
    mZ = -mZ;
    return *this;
}

float3::values& float3::values::operator /= (const values& other) {
    mX /= other.x();
    mY /= other.y();
    mZ /= other.z();
    return *this;
}
float3::values& float3::values::operator /= (float val) {
    mX /= val;
    mY /= val;
    mZ /= val;
    return *this;
}

float3::values& float3::values::operator *= (const values& other) {
    mX *= other.x();
    mY *= other.y();
    mZ *= other.z();
    return *this;
}
float3::values& float3::values::operator *= (float val) {
    mX *= val;
    mY *= val;
    mZ *= val;
    return *this;
}

float3::values& float3::values::operator += (const values& other) {
    mX += other.x();
    mY += other.y();
    mZ += other.z();
    return *this;
}
float3::values& float3::values::operator += (float val) {
    mX += val;
    mY += val;
    mZ += val;
    return *this;
}

float3::values& float3::values::operator -= (const values& other) {
    mX -= other.x();
    mY -= other.y();
    mZ -= other.z();
    return *this;
}
float3::values& float3::values::operator -= (float val) {
    mX -= val;
    mY -= val;
    mZ -= val;
    return *this;
}


///////////////////////////////////////////////////////
// Assignment and manipulation operators

float3::values& float3::values::operator = (const values& other) {
    mX = other.x();
    mY = other.y();
    mZ = other.z();
    return *this;
}
float3::values& float3::values::operator = (float val) {
    mX = val;
    mY = val;
    mZ = val;
    return *this;
}

float3::values& float3::values::operator + () {
    return *this;
}
float3::values& float3::values::operator - () {
    mX = -mX;
    mY = -mY;
    mZ = -mZ;
    return *this;
}

float3::values& float3::values::operator /= (const values& other) {
    mX /= other.x();
    mY /= other.y();
    mZ /= other.z();
    return *this;
}
float3::values& float3::values::operator /= (float val) {
    mX /= val;
    mY /= val;
    mZ /= val;
    return *this;
}

float3::values& float3::values::operator *= (const values& other) {
    mX *= other.x();
    mY *= other.y();
    mZ *= other.z();
    return *this;
}
float3::values& float3::values::operator *= (float val) {
    mX *= val;
    mY *= val;
    mZ *= val;
    return *this;
}

float3::values& float3::values::operator += (const values& other) {
    mX += other.x();
    mY += other.y();
    mZ += other.z();
    return *this;
}
float3::values& float3::values::operator += (float val) {
    mX += val;
    mY += val;
    mZ += val;
    return *this;
}

float3::values& float3::values::operator -= (const values& other) {
    mX -= other.x();
    mY -= other.y();
    mZ -= other.z();
    return *this;
}
float3::values& float3::values::operator -= (float val) {
    mX -= val;
    mY -= val;
    mZ -= val;
    return *this;
}
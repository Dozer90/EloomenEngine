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

inline float3::id_t float3::create(const value& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), vals.z(), useIDPool);
}

bool float3::try_release(id_t id) {
    return
        gMemoryBlockX.try_remove(id) &&
        gMemoryBlockY.try_remove(id) &&
        gMemoryBlockZ.try_remove(id);
}

inline float3::view float3::get_view(id_t id) {
    return {
        gMemoryBlockX.get(id),
        gMemoryBlockY.get(id),
        gMemoryBlockZ.get(id)
    };
}

inline float& float3::x(id_t id) {
    return gMemoryBlockX.get(id);
}

inline float& float3::y(id_t id) {
    return gMemoryBlockY.get(id);
}

inline float& float3::z(id_t id) {
    return gMemoryBlockZ.get(id);
}

inline const float& float3::const_x(id_t id) {
    return gMemoryBlockX.get(id);
}

inline const float& float3::const_y(id_t id) {
    return gMemoryBlockY.get(id);
}

inline const float& float3::const_z(id_t id) {
    return gMemoryBlockZ.get(id);
}


/////////////////////////////////////////////////////////////////
// float3::value

bool operator != (const float3::value& lhs, const float3::value& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z();
}
bool operator != (const float3::value& lhs, const float3::view& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z();
}
bool operator != (const float3::value& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs ||
        lhs.z() != rhs;
}

bool operator == (const float3::value& lhs, const float3::value& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z();
}
bool operator == (const float3::value& lhs, const float3::view& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z();
}
bool operator == (const float3::value& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs &&
        lhs.z() == rhs;
}

float3::value operator - (const float3::value& lhs) {
    return {
        -lhs.x(),
        -lhs.y(),
        -lhs.z()
    };
}

float3::value operator / (const float3::value& lhs, const float3::value& rhs) {
    return {
        math::is_close_to_zero(rhs.x()) ? 0.0f : lhs.x() / rhs.x(),
        math::is_close_to_zero(rhs.y()) ? 0.0f : lhs.y() / rhs.y(),
        math::is_close_to_zero(rhs.z()) ? 0.0f : lhs.z() / rhs.z()
    };
}
float3::value operator / (const float3::value& lhs, const float3::view& rhs) {
    return {
        math::is_close_to_zero(rhs.x()) ? 0.0f : lhs.x() / rhs.x(),
        math::is_close_to_zero(rhs.y()) ? 0.0f : lhs.y() / rhs.y(),
        math::is_close_to_zero(rhs.z()) ? 0.0f : lhs.z() / rhs.z()
    };
}
float3::value operator / (const float3::value& lhs, float rhs) {
    if (math::is_close_to_zero(rhs)) {
        return { 0.0f, 0.0f, 0.0f };
    }
    return {
        lhs.x() / rhs,
        lhs.y() / rhs,
        lhs.z() / rhs
    };
}

float3::value operator * (const float3::value& lhs, const float3::value& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y(),
        lhs.z() * rhs.z()
    };
}
float3::value operator * (const float3::value& lhs, const float3::view& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y(),
        lhs.z() * rhs.z()
    };
}
float3::value operator * (const float3::value& lhs, float rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs,
        lhs.z() * rhs
    };
}

float3::value operator + (const float3::value& lhs, const float3::value& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z()
    };
}
float3::value operator + (const float3::value& lhs, const float3::view& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z()
    };
}
float3::value operator + (const float3::value& lhs, float rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs,
        lhs.z() + rhs
    };
}

float3::value operator - (const float3::value& lhs, const float3::value& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z()
    };
}
float3::value operator - (const float3::value& lhs, const float3::view& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z()
    };
}
float3::value operator - (const float3::value& lhs, float rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs,
        lhs.z() - rhs
    };
}

float3::value& float3::value::operator - () {
    mX = -mX;
    mY = -mY;
    mZ = -mZ;
    return *this;
}

float3::value& float3::value::operator /= (const view& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    mZ = math::is_close_to_zero(other.z()) ? 0.0f : mZ / other.z();
    return *this;
}
float3::value& float3::value::operator /= (const value& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    mZ = math::is_close_to_zero(other.z()) ? 0.0f : mZ / other.z();
    return *this;
}
float3::value& float3::value::operator /= (float value) {
    if (math::is_close_to_zero(value)) {
        mX = 0.0f;
        mY = 0.0f;
        mZ = 0.0f;
    }
    else {
        mX /= value;
        mY /= value;
        mZ /= value;
    }
    return *this;
}

float3::value& float3::value::operator *= (const view& other) {
    mX *= other.x();
    mY *= other.y();
    mZ *= other.z();
    return *this;
}
float3::value& float3::value::operator *= (const value& other) {
    mX *= other.x();
    mY *= other.y();
    mZ *= other.z();
    return *this;
}
float3::value& float3::value::operator *= (float value) {
    mX *= value;
    mY *= value;
    mZ *= value;
    return *this;
}

float3::value& float3::value::operator += (const view& other) {
    mX += other.x();
    mY += other.y();
    mZ += other.z();
    return *this;
}
float3::value& float3::value::operator += (const value& other) {
    mX += other.x();
    mY += other.y();
    mZ += other.z();
    return *this;
}
float3::value& float3::value::operator += (float value) {
    mX += value;
    mY += value;
    mZ += value;
    return *this;
}

float3::value& float3::value::operator -= (const view& other) {
    mX -= other.x();
    mY -= other.y();
    mZ -= other.z();
    return *this;
}
float3::value& float3::value::operator -= (const value& other) {
    mX -= other.x();
    mY -= other.y();
    mZ -= other.z();
    return *this;
}
float3::value& float3::value::operator -= (float value) {
    mX -= value;
    mY -= value;
    mZ -= value;
    return *this;
}


/////////////////////////////////////////////////////////////////
// float3::view

bool operator != (const float3::view& lhs, const float3::view& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z();
}
bool operator != (const float3::view& lhs, const float3::value& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z();
}
bool operator != (const float3::view& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs ||
        lhs.z() != rhs;
}

bool operator == (const float3::view& lhs, const float3::view& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z();
}
bool operator == (const float3::view& lhs, const float3::value& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z();
}
bool operator == (const float3::view& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs &&
        lhs.z() == rhs;
}

float3::view& float3::view::operator - () {
    mX = -mX;
    mY = -mY;
    mZ = -mZ;
    return *this;
}

float3::view& float3::view::operator /= (const view& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    mZ = math::is_close_to_zero(other.z()) ? 0.0f : mZ / other.z();
    return *this;
}
float3::view& float3::view::operator /= (const value& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    mZ = math::is_close_to_zero(other.z()) ? 0.0f : mZ / other.z();
    return *this;
}
float3::view& float3::view::operator /= (float value) {
    if (math::is_close_to_zero(value)) {
        mX = 0.0f;
        mY = 0.0f;
        mZ = 0.0f;
    }
    else {
        mX /= value;
        mY /= value;
        mZ /= value;
    }
    return *this;
}

float3::view& float3::view::operator *= (const view& other) {
    mX *= other.x();
    mY *= other.y();
    mZ *= other.z();
    return *this;
}
float3::view& float3::view::operator *= (const value& other) {
    mX *= other.x();
    mY *= other.y();
    mZ *= other.z();
    return *this;
}
float3::view& float3::view::operator *= (float value) {
    mX *= value;
    mY *= value;
    mZ *= value;
    return *this;
}

float3::view& float3::view::operator += (const view& other) {
    mX += other.x();
    mY += other.y();
    mZ += other.z();
    return *this;
}
float3::view& float3::view::operator += (const value& other) {
    mX += other.x();
    mY += other.y();
    mZ += other.z();
    return *this;
}
float3::view& float3::view::operator += (float value) {
    mX += value;
    mY += value;
    mZ += value;
    return *this;
}

float3::view& float3::view::operator -= (const view& other) {
    mX -= other.x();
    mY -= other.y();
    mZ -= other.z();
    return *this;
}
float3::view& float3::view::operator -= (const value& other) {
    mX -= other.x();
    mY -= other.y();
    mZ -= other.z();
    return *this;
}
float3::view& float3::view::operator -= (float value) {
    mX -= value;
    mY -= value;
    mZ -= value;
    return *this;
}
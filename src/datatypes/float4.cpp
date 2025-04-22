#include <maths/math.h>
#include <utility/managed_memory_block.h>

using namespace eloo;

namespace {
    constexpr int gMemoryBlockInitialSize = 50;
    constexpr float gMemoryBlockExpansionScalar = 0.7f;
    using memblock_t = managed_memory_block<float, gMemoryBlockInitialSize, gMemoryBlockExpansionScalar>;
    static memblock_t gMemoryBlockX, gMemoryBlockY, gMemoryBlockZ, gMemoryBlockW;
}

float4::id_t float4::create(float x, float y, float z, float w, bool useIDPool) {
    id_t id = gMemoryBlockX.push(x, useIDPool);
    ASSERT_FATAL(id == gMemoryBlockY.push(y, useIDPool), "ID mismatch between X and Y memory blocks");
    ASSERT_FATAL(id == gMemoryBlockZ.push(z, useIDPool), "ID mismatch between X and Z memory blocks");
    ASSERT_FATAL(id == gMemoryBlockW.push(w, useIDPool), "ID mismatch between X and W memory blocks");
    return id;
}

inline float4::id_t float4::create(const value& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), vals.z(), vals.w(), useIDPool);
}

bool float4::try_release(id_t id) {
    return
        gMemoryBlockX.try_remove(id) &&
        gMemoryBlockY.try_remove(id) &&
        gMemoryBlockZ.try_remove(id) &&
        gMemoryBlockW.try_remove(id);
}

inline float4::view float4::get_view(id_t id) {
    return {
        gMemoryBlockX.get(id),
        gMemoryBlockY.get(id),
        gMemoryBlockZ.get(id),
        gMemoryBlockW.get(id)
    };
}

inline float& float4::x(id_t id) {
    return gMemoryBlockX.get(id);
}
inline float& float4::y(id_t id) {
    return gMemoryBlockY.get(id);
}
inline float& float4::z(id_t id) {
    return gMemoryBlockZ.get(id);
}
inline float& float4::w(id_t id) {
    return gMemoryBlockW.get(id);
}

inline const float& float4::const_x(id_t id) {
    return gMemoryBlockX.get(id);
}
inline const float& float4::const_y(id_t id) {
    return gMemoryBlockY.get(id);
}
inline const float& float4::const_z(id_t id) {
    return gMemoryBlockZ.get(id);
}
inline const float& float4::const_w(id_t id) {
    return gMemoryBlockW.get(id);
}


/////////////////////////////////////////////////////////////////
// float4::value

bool operator != (const float4::value& lhs, const float4::value& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z() ||
        lhs.w() != rhs.w();
}
bool operator != (const float4::value& lhs, const float4::view& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z() ||
        lhs.w() != rhs.w();
}
bool operator != (const float4::value& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs ||
        lhs.z() != rhs ||
        lhs.w() != rhs;
}

bool operator == (const float4::value& lhs, const float4::value& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z() &&
        lhs.w() == rhs.w();
}
bool operator == (const float4::value& lhs, const float4::view& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z() &&
        lhs.w() == rhs.w();
}
bool operator == (const float4::value& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs &&
        lhs.z() == rhs &&
        lhs.w() == rhs;
}

float4::value operator - (const float4::value& lhs) {
    return {
        -lhs.x(),
        -lhs.y(),
        -lhs.z(),
        -lhs.w()
    };
}

float4::value operator / (const float4::value& lhs, const float4::value& rhs) {
    return {
        math::is_close_to_zero(rhs.x()) ? 0.0f : lhs.x() / rhs.x(),
        math::is_close_to_zero(rhs.y()) ? 0.0f : lhs.y() / rhs.y(),
        math::is_close_to_zero(rhs.z()) ? 0.0f : lhs.z() / rhs.z(),
        math::is_close_to_zero(rhs.w()) ? 0.0f : lhs.w() / rhs.w()
    };
}
float4::value operator / (const float4::value& lhs, const float4::view& rhs) {
    return {
        math::is_close_to_zero(rhs.x()) ? 0.0f : lhs.x() / rhs.x(),
        math::is_close_to_zero(rhs.y()) ? 0.0f : lhs.y() / rhs.y(),
        math::is_close_to_zero(rhs.z()) ? 0.0f : lhs.z() / rhs.z(),
        math::is_close_to_zero(rhs.w()) ? 0.0f : lhs.w() / rhs.w()
    };
}
float4::value operator / (const float4::value& lhs, float rhs) {
    if (math::is_close_to_zero(rhs)) {
        return { 0.0f, 0.0f, 0.0f, 0.0f };
    }
    return {
        lhs.x() / rhs,
        lhs.y() / rhs,
        lhs.z() / rhs,
        lhs.w() / rhs
    };
}

float4::value operator * (const float4::value& lhs, const float4::value& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y(),
        lhs.z() * rhs.z(),
        lhs.w() * rhs.w()
    };
}
float4::value operator * (const float4::value& lhs, const float4::view& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y(),
        lhs.z() * rhs.z(),
        lhs.w() * rhs.w()
    };
}
float4::value operator * (const float4::value& lhs, float rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs,
        lhs.z() * rhs,
        lhs.w() * rhs
    };
}

float4::value operator + (const float4::value& lhs, const float4::value& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z(),
        lhs.w() + rhs.w()
    };
}
float4::value operator + (const float4::value& lhs, const float4::view& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z(),
        lhs.w() + rhs.w()
    };
}
float4::value operator + (const float4::value& lhs, float rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs,
        lhs.z() + rhs,
        lhs.w() + rhs
    };
}

float4::value operator - (const float4::value& lhs, const float4::value& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z(),
        lhs.w() - rhs.w()
    };
}
float4::value operator - (const float4::value& lhs, const float4::view& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z(),
        lhs.w() - rhs.w()
    };
}
float4::value operator - (const float4::value& lhs, float rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs,
        lhs.z() - rhs,
        lhs.w() - rhs
    };
}


/////////////////////////////////////////////////////////////////
// float4::view

bool operator != (const float4::view& lhs, const float4::view& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z() ||
        lhs.w() != rhs.w();
}
bool operator != (const float4::value& lhs, const float4::view& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z() ||
        lhs.w() != rhs.w();
}
bool operator != (const float4::value& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs ||
        lhs.z() != rhs ||
        lhs.w() != rhs;
}

bool operator == (const float4::view& lhs, const float4::view& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z() &&
        lhs.w() == rhs.w();
}
bool operator == (const float4::view& lhs, const float4::value& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z() &&
        lhs.w() == rhs.w();
}
bool operator == (const float4::view& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs &&
        lhs.z() == rhs &&
        lhs.w() == rhs;
}

float4::view& float4::view::operator - () {
    mX = -mX;
    mY = -mY;
    mZ = -mZ;
    mW = -mW;
    return *this;
}

float4::view& float4::view::operator /= (const view& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    mZ = math::is_close_to_zero(other.z()) ? 0.0f : mZ / other.z();
    mW = math::is_close_to_zero(other.w()) ? 0.0f : mW / other.w();
    return *this;
}
float4::view& float4::view::operator /= (const value& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    mZ = math::is_close_to_zero(other.z()) ? 0.0f : mZ / other.z();
    mW = math::is_close_to_zero(other.w()) ? 0.0f : mW / other.w();
    return *this;
}
float4::view& float4::view::operator /= (float value) {
    if (math::is_close_to_zero(value)) {
        mX = 0.0f;
        mY = 0.0f;
        mZ = 0.0f;
        mW = 0.0f;
    } else {
        mX /= value;
        mY /= value;
        mZ /= value;
        mW /= value;
    }
    return *this;
}

float4::view& float4::view::operator *= (const view& other) {
    mX *= other.x();
    mY *= other.y();
    mZ *= other.z();
    mW *= other.w();
    return *this;
}
float4::view& float4::view::operator *= (const value& other) {
    mX *= other.x();
    mY *= other.y();
    mZ *= other.z();
    mW *= other.w();
    return *this;
}
float4::view& float4::view::operator *= (float value) {
    mX *= value;
    mY *= value;
    mZ *= value;
    mW *= value;
    return *this;
}

float4::view& float4::view::operator += (const view& other) {
    mX += other.x();
    mY += other.y();
    mZ += other.z();
    mW += other.w();
    return *this;
}
float4::view& float4::view::operator += (const value& other) {
    mX += other.x();
    mY += other.y();
    mZ += other.z();
    mW += other.w();
    return *this;
}
float4::view& float4::view::operator += (float value) {
    mX += value;
    mY += value;
    mZ += value;
    mW += value;
    return *this;
}

float4::view& float4::view::operator -= (const view& other) {
    mX -= other.x();
    mY -= other.y();
    mZ -= other.z();
    mW -= other.w();
    return *this;
}
float4::view& float4::view::operator -= (const value& other) {
    mX -= other.x();
    mY -= other.y();
    mZ -= other.z();
    mW -= other.w();
    return *this;
}
float4::view& float4::view::operator -= (float value) {
    mX -= value;
    mY -= value;
    mZ -= value;
    mW -= value;
    return *this;
}
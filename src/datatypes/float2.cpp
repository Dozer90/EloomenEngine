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

inline float2::id_t float2::create(const value& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), useIDPool);
}

bool float2::try_release(id_t id) {
    return
        gMemoryBlockX.try_remove(id) &&
        gMemoryBlockY.try_remove(id);
}

inline float2::view float2::get_view(id_t id) {
    return {
        gMemoryBlockX.get(id),
        gMemoryBlockY.get(id)
    };
}

inline float& float2::x(id_t id) {
    return gMemoryBlockX.get(id);
}

inline float& float2::y(id_t id) {
    return gMemoryBlockY.get(id);
}

inline const float& float2::const_x(id_t id) {
    return gMemoryBlockX.get(id);
}

inline const float& float2::const_y(id_t id) {
    return gMemoryBlockY.get(id);
}


/////////////////////////////////////////////////////////////////
// float2::value

bool operator != (const float2::value& lhs, const float2::value& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y();
}
bool operator != (const float2::value& lhs, const float2::view& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y();
}
bool operator != (const float2::value& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs;
}

bool operator == (const float2::value& lhs, const float2::value& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y();
}
bool operator == (const float2::value& lhs, const float2::view& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y();
}
bool operator == (const float2::value& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs;
}

float2::value operator - (const float2::value& lhs) {
    return {
        -lhs.x(),
        -lhs.y()
    };
}

float2::value operator / (const float2::value& lhs, const float2::value& rhs) {
    return {
        math::is_close_to_zero(rhs.x()) ? 0.0f : lhs.x() / rhs.x(),
        math::is_close_to_zero(rhs.y()) ? 0.0f : lhs.y() / rhs.y()
    };
}
float2::value operator / (const float2::value& lhs, const float2::view& rhs) {
    return {
        math::is_close_to_zero(rhs.x()) ? 0.0f : lhs.x() / rhs.x(),
        math::is_close_to_zero(rhs.y()) ? 0.0f : lhs.y() / rhs.y()
    };
}
float2::value operator / (const float2::value& lhs, float rhs) {
    if (math::is_close_to_zero(rhs)) {
        return { 0.0f, 0.0f };
    }
    return {
        lhs.x() / rhs,
        lhs.y() / rhs
    };
}

float2::value operator * (const float2::value& lhs, const float2::value& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y()
    };
}
float2::value operator * (const float2::value& lhs, const float2::view& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y()
    };
}
float2::value operator * (const float2::value& lhs, float rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs
    };
}

float2::value operator + (const float2::value& lhs, const float2::value& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y()
    };
}
float2::value operator + (const float2::value& lhs, const float2::view& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y()
    };
}
float2::value operator + (const float2::value& lhs, float rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs
    };
}

float2::value operator - (const float2::value& lhs, const float2::value& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y()
    };
}
float2::value operator - (const float2::value& lhs, const float2::view& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y()
    };
}
float2::value operator - (const float2::value& lhs, float rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs
    };
}

float2::value& float2::value::operator = (const value& other) {
    mX = other.x();
    mY = other.y();
    return *this;
}
float2::value& float2::value::operator = (const view& other) {
    mX = other.x();
    mY = other.y();
    return *this;
}
float2::value& float2::value::operator = (float value) {
    mX = value;
    mY = value;
    return *this;
}

float2::value& float2::value::operator - () {
    mX = -mX;
    mY = -mY;
    return *this;
}

float2::value& float2::value::operator /= (const view& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    return *this;
}
float2::value& float2::value::operator /= (const value& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    return *this;
}
float2::value& float2::value::operator /= (float value) {
    if (math::is_close_to_zero(value)) {
        mX = 0.0f;
        mY = 0.0f;
    } else {
        mX /= value;
        mY /= value;
    }
    return *this;
}

float2::value& float2::value::operator *= (const view& other) {
    mX *= other.x();
    mY *= other.y();
    return *this;
}
float2::value& float2::value::operator *= (const value& other) {
    mX *= other.x();
    mY *= other.y();
    return *this;
}
float2::value& float2::value::operator *= (float value) {
    mX *= value;
    mY *= value;
    return *this;
}

float2::value& float2::value::operator += (const view& other) {
    mX += other.x();
    mY += other.y();
    return *this;
}
float2::value& float2::value::operator += (const value& other) {
    mX += other.x();
    mY += other.y();
    return *this;
}
float2::value& float2::value::operator += (float value) {
    mX += value;
    mY += value;
    return *this;
}

float2::value& float2::value::operator -= (const view& other) {
    mX -= other.x();
    mY -= other.y();
    return *this;
}
float2::value& float2::value::operator -= (const value& other) {
    mX -= other.x();
    mY -= other.y();
    return *this;
}
float2::value& float2::value::operator -= (float value) {
    mX -= value;
    mY -= value;
    return *this;
}


/////////////////////////////////////////////////////////////////
// float2::view

bool operator != (const float2::view& lhs, const float2::view& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y();
}
bool operator != (const float2::view& lhs, const float2::value& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y();
}
bool operator != (const float2::view& lhs, float rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs;
}

bool operator == (const float2::view& lhs, const float2::view& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y();
}
bool operator == (const float2::view& lhs, const float2::value& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y();
}
bool operator == (const float2::view& lhs, float rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs;
}

float2::view& float2::view::operator = (const view& other) {
    mX = other.x();
    mY = other.y();
    return *this;
}
float2::view& float2::view::operator = (const value& other) {
    mX = other.x();
    mY = other.y();
    return *this;
}
float2::view& float2::view::operator = (float value) {
    mX = value;
    mY = value;
    return *this;
}

float2::view& float2::view::operator - () {
    mX = -mX;
    mY = -mY;
    return *this;
}

float2::view& float2::view::operator /= (const view& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    return *this;
}
float2::view& float2::view::operator /= (const value& other) {
    mX = math::is_close_to_zero(other.x()) ? 0.0f : mX / other.x();
    mY = math::is_close_to_zero(other.y()) ? 0.0f : mY / other.y();
    return *this;
}
float2::view& float2::view::operator /= (float value) {
    if (math::is_close_to_zero(value)) {
        mX = 0.0f;
        mY = 0.0f;
    }
    else {
        mX /= value;
        mY /= value;
    }
    return *this;
}

float2::view& float2::view::operator *= (const view& other) {
    mX *= other.x();
    mY *= other.y();
    return *this;
}
float2::view& float2::view::operator *= (const value& other) {
    mX *= other.x();
    mY *= other.y();
    return *this;
}
float2::view& float2::view::operator *= (float value) {
    mX *= value;
    mY *= value;
    return *this;
}

float2::view& float2::view::operator += (const view& other) {
    mX += other.x();
    mY += other.y();
    return *this;
}
float2::view& float2::view::operator += (const value& other) {
    mX += other.x();
    mY += other.y();
    return *this;
}
float2::view& float2::view::operator += (float value) {
    mX += value;
    mY += value;
    return *this;
}

float2::view& float2::view::operator -= (const view& other) {
    mX -= other.x();
    mY -= other.y();
    return *this;
}
float2::view& float2::view::operator -= (const value& other) {
    mX -= other.x();
    mY -= other.y();
    return *this;
}
float2::view& float2::view::operator -= (float value) {
    mX -= value;
    mY -= value;
    return *this;
}
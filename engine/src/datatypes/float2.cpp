#include "maths/math.h"
#include "memory.h"

#include <stdexcept>

using namespace sge;

namespace {
    dense_storage<float2::values> gStorage;

    float2::values &require_value(float2::id_t id) {
        auto *value = gStorage.try_get(id);

        if (!value) {
            throw std::out_of_range("Invalid float2 handle");
        }

        return *value;
    }
}

float2::id_t float2::create(float x, float y) {
    return gStorage.create(values{ x, y });
}

bool float2::release(id_t id) {
    return gStorage.remove(id);
}

bool float2::try_get_values(id_t id, float &x, float &y) {
    const auto *value = gStorage.try_get(id);

    if (!value) {
        return false;
    }

    x = value->x();
    y = value->y();
    return true;
}

float &float2::x(id_t id) {
    return require_value(id).x();
}

float &float2::y(id_t id) {
    return require_value(id).y();
}

const float &float2::const_x(id_t id) {
    return require_value(id).x();
}

const float &float2::const_y(id_t id) {
    return require_value(id).y();
}


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

float2::values& float2::values::operator = (const values& other) SGE_NOEXCEPT {
    this->mX = other.mX;
    this->mY = other.mY;
    return *this;
}
float2::values& float2::values::operator = (float val) SGE_NOEXCEPT {
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

bool float2::is_valid(id_t id) {
    return gStorage.is_valid(id);
}

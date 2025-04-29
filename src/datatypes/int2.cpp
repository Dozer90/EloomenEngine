#include <datatype/int2.h>

#include <maths/math.h>

#include <utility/managed_memory_block.h>

using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 50;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.7f;
    using memblock_t = managed_memory_block<int, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>;
    static memblock_t gMemoryBlockX, gMemoryBlockY;
}

int2::id_t int2::create(int x, int y, bool useIDPool) {
    id_t id = gMemoryBlockX.push(x, useIDPool);
    ELOO_ASSERT_FATAL(id == gMemoryBlockY.push(y, useIDPool), "ID mismatch between X and Y memory blocks");
    return id;
}

inline int2::id_t int2::create(const values& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), useIDPool);
}

bool int2::try_release(id_t id) {
    if (!is_valid(id)) {
        return false;
    }
    gMemoryBlockX.remove(id);
    gMemoryBlockY.remove(id);
    return true;
}

inline bool int2::try_get_values(id_t id, int2::values& vals) {
    if (is_valid(id)) {
        vals = {
            gMemoryBlockX.get(id),
            gMemoryBlockY.get(id)
        };
        return true;
    }
    return false;
}

inline int& int2::x(id_t id) { return gMemoryBlockX.get(id); }
inline int& int2::y(id_t id) { return gMemoryBlockY.get(id); }

inline const int& int2::const_x(id_t id) { return gMemoryBlockX.get(id); }
inline const int& int2::const_y(id_t id) { return gMemoryBlockY.get(id); }


///////////////////////////////////////////////////////
// Values container

bool int2::operator != (const int2::values& lhs, const int2::values& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y();
}
bool int2::operator != (const int2::values& lhs, int rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs;
}

bool int2::operator == (const int2::values& lhs, const int2::values& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y();
}
bool int2::operator == (const int2::values& lhs, int rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs;
}

int2::values int2::operator + (const int2::values& lhs) {
    return lhs;
}
int2::values int2::operator - (const int2::values& lhs) {
    return {
        -lhs.x(),
        -lhs.y()
    };
}

int2::values int2::operator / (const int2::values& lhs, const int2::values& rhs) {
    return {
        lhs.x() / rhs.x(),
        lhs.y() / rhs.y()
    };
}
int2::values int2::operator / (const int2::values& lhs, int rhs) {
    return {
        lhs.x() / rhs,
        lhs.y() / rhs
    };
}
int2::values int2::operator / (const int2::values& lhs, float rhs) {
    return {
        static_cast<int>(lhs.x() / rhs),
        static_cast<int>(lhs.y() / rhs)
    };
}

int2::values int2::operator * (const int2::values& lhs, const int2::values& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y()
    };
}
int2::values int2::operator * (const int2::values& lhs, int rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs
    };
}
int2::values int2::operator * (const int2::values& lhs, float rhs) {
    return {
        static_cast<int>(lhs.x() * rhs),
        static_cast<int>(lhs.y() * rhs)
    };
}

int2::values int2::operator + (const int2::values& lhs, const int2::values& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y()
    };
}
int2::values int2::operator + (const int2::values& lhs, int rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs
    };
}
int2::values int2::operator + (const int2::values& lhs, float rhs) {
    return {
        static_cast<int>(lhs.x() + rhs),
        static_cast<int>(lhs.y() + rhs)
    };
}

int2::values int2::operator - (const int2::values& lhs, const int2::values& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y()
    };
}
int2::values int2::operator - (const int2::values& lhs, int rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs
    };
}
int2::values int2::operator - (const int2::values& lhs, float rhs) {
    return {
        static_cast<int>(lhs.x() - rhs),
        static_cast<int>(lhs.y() - rhs)
    };
}


///////////////////////////////////////////////////////
// Assignment and manipulation int2::operators

int2::values& int2::values::operator = (const values& other) {
    *this = other;
    return *this;
}
int2::values& int2::values::operator = (int val) {
    *this = val;
    return *this;
}

int2::values& int2::values::operator - () {
    *this = -*this;
    return *this;
}

int2::values& int2::values::operator /= (const values& other) {
    *this = *this / other;
    return *this;
}
int2::values& int2::values::operator /= (int val) {
    *this = *this / val;
    return *this;
}

int2::values& int2::values::operator *= (const values& other) {
    *this = *this * other;
    return *this;
}
int2::values& int2::values::operator *= (int val) {
    *this = *this * val;
    return *this;
}

int2::values& int2::values::operator += (const values& other) {
    *this = *this + other;
    return *this;
}
int2::values& int2::values::operator += (int val) {
    *this = *this + val;
    return *this;
}

int2::values& int2::values::operator -= (const values& other) {
    *this = *this - other;
    return *this;
}
int2::values& int2::values::operator -= (int val) {
    *this = *this - val;
    return *this;
}
#include <datatype/int3.h>

#include <maths/math.h>

#include <utility/managed_memory_block.h>

using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 50;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.7f;
    using memblock_t = managed_memory_block<int, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>;
    static memblock_t gMemoryBlockX, gMemoryBlockY, gMemoryBlockZ;
}

int3::id_t int3::create(int x, int y, int z, bool useIDPool) {
    id_t id = gMemoryBlockX.push(x, useIDPool);
    ELOO_ASSERT_FATAL(id == gMemoryBlockY.push(y, useIDPool), "ID mismatch between X and Y memory blocks");
    ELOO_ASSERT_FATAL(id == gMemoryBlockZ.push(z, useIDPool), "ID mismatch between X and Z memory blocks");
    return id;
}

int3::id_t int3::create(const values& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), vals.z(), useIDPool);
}

bool int3::try_release(id_t id) {
    if (!is_valid(id)) {
        return false;
    }
    gMemoryBlockX.remove(id);
    gMemoryBlockY.remove(id);
    gMemoryBlockZ.remove(id);
    return true;
}

bool int3::try_get_values(id_t id, int3::values& vals) {
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

int& int3::x(id_t id) { return gMemoryBlockX.get(id); }
int& int3::y(id_t id) { return gMemoryBlockY.get(id); }
int& int3::z(id_t id) { return gMemoryBlockZ.get(id); }

const int& int3::const_x(id_t id) { return gMemoryBlockX.get(id); }
const int& int3::const_y(id_t id) { return gMemoryBlockY.get(id); }
const int& int3::const_z(id_t id) { return gMemoryBlockZ.get(id); }


/////////////////////////////////////////////////////////////////
// int3::values

bool int3::operator != (const int3::values& lhs, const int3::values& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z();
}
bool int3::operator != (const int3::values& lhs, int rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs ||
        lhs.z() != rhs;
}

bool int3::operator == (const int3::values& lhs, const int3::values& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z();
}
bool int3::operator == (const int3::values& lhs, int rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs &&
        lhs.z() == rhs;
}

int3::values int3::operator + (const int3::values& lhs) {
    return lhs;
}
int3::values int3::operator - (const int3::values& lhs) {
    return {
        -lhs.x(),
        -lhs.y(),
        -lhs.z()
    };
}

int3::values int3::operator / (const int3::values& lhs, const int3::values& rhs) {
    return {
        lhs.x() / rhs.x(),
        lhs.y() / rhs.y(),
        lhs.z() / rhs.z()
    };
}
int3::values int3::operator / (const int3::values& lhs, int rhs) {
    return {
        lhs.x() / rhs,
        lhs.y() / rhs,
        lhs.z() / rhs
    };
}

int3::values int3::operator * (const int3::values& lhs, const int3::values& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y(),
        lhs.z() * rhs.z()
    };
}
int3::values int3::operator * (const int3::values& lhs, int rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs,
        lhs.z() * rhs
    };
}

int3::values int3::operator + (const int3::values& lhs, const int3::values& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z()
    };
}
int3::values int3::operator + (const int3::values& lhs, int rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs,
        lhs.z() + rhs
    };
}

int3::values int3::operator - (const int3::values& lhs, const int3::values& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z()
    };
}
int3::values int3::operator - (const int3::values& lhs, int rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs,
        lhs.z() - rhs
    };
}


///////////////////////////////////////////////////////
// Assignment and manipulation int3::operators

int3::values& int3::values::operator = (const values& other) {
    *this = other;
    return *this;
}
int3::values& int3::values::operator = (int val) {
    *this = val;
    return *this;
}

int3::values& int3::values::operator - () {
    *this = -*this;
    return *this;
}

int3::values& int3::values::operator /= (const values& other) {
    *this = *this / other;
    return *this;
}
int3::values& int3::values::operator /= (int val) {
    *this = *this / val;
    return *this;
}

int3::values& int3::values::operator *= (const values& other) {
    *this = *this * other;
    return *this;
}
int3::values& int3::values::operator *= (int val) {
    *this = *this * val;
    return *this;
}

int3::values& int3::values::operator += (const values& other) {
    *this = *this + other;
    return *this;
}
int3::values& int3::values::operator += (int val) {
    *this = *this + val;
    return *this;
}

int3::values& int3::values::operator -= (const values& other) {
    *this = *this - other;
    return *this;
}
int3::values& int3::values::operator -= (int val) {
    *this = *this - val;
    return *this;
}
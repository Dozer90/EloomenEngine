#include <datatype/int4.h>

#include <maths/math.h>

#include <utility/managed_memory_block.h>

using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 50;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.7f;
    using memblock_t = managed_memory_block<int, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION>;
    static memblock_t gMemoryBlockX, gMemoryBlockY, gMemoryBlockZ, gMemoryBlockW;
}

int4::id_t int4::create(int x, int y, int z, int w, bool useIDPool) {
    id_t id = gMemoryBlockX.push(x, useIDPool);
    ELOO_ASSERT_FATAL(id == gMemoryBlockY.push(y, useIDPool), "ID mismatch between X and Y memory blocks.");
    ELOO_ASSERT_FATAL(id == gMemoryBlockZ.push(z, useIDPool), "ID mismatch between X and Z memory blocks.");
    ELOO_ASSERT_FATAL(id == gMemoryBlockW.push(w, useIDPool), "ID mismatch between X and W memory blocks.");
    return id;
}

int4::id_t int4::create(const values& vals, bool useIDPool) {
    return create(vals.x(), vals.y(), vals.z(), vals.w(), useIDPool);
}

bool int4::try_release(id_t id) {
    if (!is_valid(id)) {
        return false;
    }
    gMemoryBlockX.remove(id);
    gMemoryBlockY.remove(id);
    gMemoryBlockZ.remove(id);
    gMemoryBlockW.remove(id);
    return true;
}

bool int4::try_get_values(id_t id, int4::values& vals) {
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

int& int4::x(id_t id) { return gMemoryBlockX.get(id); }
int& int4::y(id_t id) { return gMemoryBlockY.get(id); }
int& int4::z(id_t id) { return gMemoryBlockZ.get(id); }
int& int4::w(id_t id) { return gMemoryBlockW.get(id); }

const int& int4::const_x(id_t id) { return gMemoryBlockX.get(id); }
const int& int4::const_y(id_t id) { return gMemoryBlockY.get(id); }
const int& int4::const_z(id_t id) { return gMemoryBlockZ.get(id); }
const int& int4::const_w(id_t id) { return gMemoryBlockW.get(id); }


///////////////////////////////////////////////////////
// Values container

bool int4::operator != (const int4::values& lhs, const int4::values& rhs) {
    return
        lhs.x() != rhs.x() ||
        lhs.y() != rhs.y() ||
        lhs.z() != rhs.z() ||
        lhs.w() != rhs.w();
}
bool int4::operator != (const int4::values& lhs, int rhs) {
    return
        lhs.x() != rhs ||
        lhs.y() != rhs ||
        lhs.z() != rhs ||
        lhs.w() != rhs;
}

bool int4::operator == (const int4::values& lhs, const int4::values& rhs) {
    return
        lhs.x() == rhs.x() &&
        lhs.y() == rhs.y() &&
        lhs.z() == rhs.z() &&
        lhs.w() == rhs.w();
}
bool int4::operator == (const int4::values& lhs, int rhs) {
    return
        lhs.x() == rhs &&
        lhs.y() == rhs &&
        lhs.z() == rhs &&
        lhs.w() == rhs;
}

int4::values int4::operator + (const int4::values& lhs) {
    return lhs;
}
int4::values int4::operator - (const int4::values& lhs) {
    return {
        -lhs.x(),
        -lhs.y(),
        -lhs.z(),
        -lhs.w()
    };
}

int4::values int4::operator / (const int4::values& lhs, const int4::values& rhs) {
    return {
        lhs.x() / rhs.x(),
        lhs.y() / rhs.y(),
        lhs.z() / rhs.z(),
        lhs.w() / rhs.w()
    };
}
int4::values int4::operator / (const int4::values& lhs, int rhs) {

    return {
        lhs.x() / rhs,
        lhs.y() / rhs,
        lhs.z() / rhs,
        lhs.w() / rhs
    };
}

int4::values int4::operator * (const int4::values& lhs, const int4::values& rhs) {
    return {
        lhs.x() * rhs.x(),
        lhs.y() * rhs.y(),
        lhs.z() * rhs.z(),
        lhs.w() * rhs.w()
    };
}
int4::values int4::operator * (const int4::values& lhs, int rhs) {
    return {
        lhs.x() * rhs,
        lhs.y() * rhs,
        lhs.z() * rhs,
        lhs.w() * rhs
    };
}

int4::values int4::operator + (const int4::values& lhs, const int4::values& rhs) {
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z(),
        lhs.w() + rhs.w()
    };
}
int4::values int4::operator + (const int4::values& lhs, int rhs) {
    return {
        lhs.x() + rhs,
        lhs.y() + rhs,
        lhs.z() + rhs,
        lhs.w() + rhs
    };
}

int4::values int4::operator - (const int4::values& lhs, const int4::values& rhs) {
    return {
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z(),
        lhs.w() - rhs.w()
    };
}
int4::values int4::operator - (const int4::values& lhs, int rhs) {
    return {
        lhs.x() - rhs,
        lhs.y() - rhs,
        lhs.z() - rhs,
        lhs.w() - rhs
    };
}


///////////////////////////////////////////////////////
// Assignment and manipulation int4::operators

int4::values& int4::values::operator = (const values& other) {
    *this = other;
    return *this;
}
int4::values& int4::values::operator = (int val) {
    *this = val;
    return *this;
}

int4::values& int4::values::operator - () {
    *this = -*this;
    return *this;
}

int4::values& int4::values::operator /= (const values& other) {
    *this = *this / other;
    return *this;
}
int4::values& int4::values::operator /= (int val) {
    *this = *this / val;
    return *this;
}

int4::values& int4::values::operator *= (const values& other) {
    *this = *this * other;
    return *this;
}
int4::values& int4::values::operator *= (int val) {
    *this = *this * val;
    return *this;
}

int4::values& int4::values::operator += (const values& other) {
    *this = *this + other;
    return *this;
}
int4::values& int4::values::operator += (int val) {
    *this = *this + val;
    return *this;
}

int4::values& int4::values::operator -= (const values& other) {
    *this = *this - other;
    return *this;
}
int4::values& int4::values::operator -= (int val) {
    *this = *this - val;
    return *this;
}
#pragma once

#include "LayerMasks.h"
#include "Vector3.h"

namespace eloo::Math {

class Ray {
public:
    struct HitInfo {
        bool hit = false;
        Math::Vector3 position;
    };

public:
    Ray() {}
    Ray(const Math::Vector3& position, const Math::Vector3& direction, float length);

    inline void setPosition(const Math::Vector3& position) { mPosition = position; }

    void setDirection(float degrees);
    inline void setDirection(const Math::Vector3& direction) { mDirection = direction; }

    inline void setLength(float length) { mLength = length; }

    inline const Math::Vector3& getPosition() const { return mPosition; }
    inline const Math::Vector3& getDirection() const { return mDirection; }
    inline const float getLength() const { return mLength; }

    HitInfo cast(LayerMask mask) const;
    HitInfo navmeshCast() const;

private:
    Math::Vector3 mPosition;
    Math::Vector3 mDirection;
    float mLength;
};
}; // namespace Math
#include "TankBot.h"

#include "Maths.h"

using namespace eloo;

void TankBot::setLeftTreadForce(double speed) {
    mTreadVelocity.x = math::clamp(speed, -gTreadMaxSpeedPerSecond, gTreadMaxSpeedPerSecond);
}

void TankBot::setRightTreadForce(double speed) {
    mTreadVelocity.y = math::clamp(speed, -gTreadMaxSpeedPerSecond, gTreadMaxSpeedPerSecond);
}

void TankBot::rotateTurret(double angleDegrees) {
    mTurretRotationSpeed = angleDegrees;
}

void TankBot::update(const double deltaTime) {
    // Calculate the new forward velocity
    const double torque = (mTreadVelocity.y - mTreadVelocity.x) / gTankTreadSeperation;
    mTankRotation = math::mod(mTankRotation + torque * deltaTime, math::Tau);
    mTankForward.x = math::cos(math::toRadians(mTankRotation));
    mTankForward.y = math::sin(math::toRadians(mTankRotation));

    // Get the movement speed
    mMovementSpeed = (mTreadVelocity.x + mTreadVelocity.y) / 2.0f;

    // Move the tank
    mTankPosition += mTankForward * mMovementSpeed;

    // Rotate the turret
    mTurretRotation = std::fmod(mTurretRotation + mTurretRotationSpeed * deltaTime, math::Tau);
    mTankForward.x = math::cos(math::toRadians(mTankRotation + mTurretRotation));
    mTankForward.y = math::sin(math::toRadians(mTankRotation + mTurretRotation));

    // Check for other tanks
    eastl::vector<TankBot*> foundBots;
    //for ()
}


//	void Shoot() const;
//
//	void TakeDamage(int damage);
//
//private:
//	int mHealth = 100;
//	sf::Vector2f mPosition;
//	sf::Vector2f mTreadVelocity;
//	float mTurretDirection = 0.0f;
//	float mShotCooldown = 0.0f;
//};
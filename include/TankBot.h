#pragma once

#include "Objects/GameObject.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <SFML/System.hpp>

namespace TankWarz
{
constexpr double gTreadMaxSpeedPerSecond = 5.0;
constexpr double gTurrentMaxTurnAngleDegreesPerSecond = 30.0;
constexpr double gShotCooldownSeconds = 3.0;
constexpr double gTankTreadSeperation = 10.0;

constexpr double gSensorRange = 50.0;

class TankBot : public IGameObject
{
public:
	// Return the name of the bot
	virtual const eastl::string& getBotName() = 0;

	// Return the name of the bot creator
	virtual const eastl::string& getCreatorName() = 0;

	// This is where the meat of the code will go
	virtual void update(const double deltaTime) override;

protected:
	// Set the speed of the treads to move the tank
	void setLeftTreadForce(double speed);
	void setRightTreadForce(double speed);

	void rotateTurret(double angleDegrees);

	const sf::Vector2<double> getPosition() const { return mTankPosition; }
	const sf::Vector2<double> getForwardVector() const { return mTankForward; }
	const sf::Vector2<double> getTurretForwardVector() const { return mTurretForward; }

	void shoot() const;

	void takeDamage(int damage, const sf::Vector2f& directionToImpact);

private:
	int mHealth = 100;
	sf::Vector2<double> mPosition;
	sf::Vector2<double> mTreadVelocity;
	sf::Vector2<double> mTankForward;
	sf::Vector2<double> mTurretForward;
	sf::Vector2<double> mTankPosition;
	double mMovementSpeed = 0.0;
	double mTankRotation = 0.0;
	double mTurretRotation = 0.0;
	double mTurretRotationSpeed = 0.0;
	double mShotCooldown = 0.0;
};
}
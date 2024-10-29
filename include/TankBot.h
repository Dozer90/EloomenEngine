#pragma once

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <SFML/System.hpp>

constexpr double gTreadMaxSpeedPerSecond = 5.0;
constexpr double gTurrentMaxTurnAngleDegreesPerSecond = 30.0;
constexpr double gShotCooldownSeconds = 3.0;
constexpr double gTankTreadSeperation = 10.0;

constexpr double gSensorRange = 50.0;

class TankBot
{
public:
	// Return the name of the bot
	virtual const eastl::string& GetBotName() = 0;

	// Return the name of the bot creator
	virtual const eastl::string& GetCreatorName() = 0;

	// This is where the meat of the code will go
	virtual void LogicUpdate(const float deltaTime, eastl::vector<const TankBot&> detectedEnemies) = 0;

public:
	// Set the speed of the treads to move the tank
	void SetLeftTreadForce(double speed);
	void SetRightTreadForce(double speed);

	void RotateTurret(double angleDegrees);

	const sf::Vector2<double> GetPosition() const { return mTankPosition; }
	const sf::Vector2<double> GetForwardVector() const { return mTankForward; }
	const sf::Vector2<double> GetTurretForwardVector() const { return mTurretForward; }

	void Shoot() const;

	void TakeDamage(int damage, const sf::Vector2f& directionToImpact);

	void Update(const double deltaTime);

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
#pragma once

#include "Objects/GameObject.h"
#include "Objects/ObjectPool.h"

#include <SFML/Graphics/Color.hpp>

#include <EASTL/vector.h>

namespace eloo::Particles
{

using ParticleSystemID = uint8_t;
using PropertyCurveID = uint8_t;

class PropertyCurve
{
public:
	PropertyCurve(float initialStrength, uint8_t pointCount)
	{
		time.reserve(pointCount);
		strength.reserve(pointCount);
		ease.reserve(pointCount - 1);

		time.push_back(0.0f);
		strength.push_back(initialStrength);
	}

	PropertyCurve& addPoint(float time, float strength, EaseType ease)
	{
		ASSERT(time > *(this->time.rbegin()));
		this->time.push_back(time);
		this->strength.push_back(strength);
		this->ease.push_back(ease);
		return *this;
	}

	PropertyCurve& scaleTime(float scale)
	{
		for (float& timeVal : time)
		{
			timeVal *= scale;
		}
		return *this;
	}

	PropertyCurve& normaliseTime()
	{
		return scaleTime(1.0f / *time.rbegin());
	}

	PropertyCurve& scaleStrength(float scale)
	{
		for (float& strengthVal : strength)
		{
			strengthVal *= scale;
		}
		return *this;
	}

	PropertyCurve& normaliseStrength()
	{
		return scaleStrength(1.0f / *strength.rbegin();
	}

	getValueAt(float t) const
	{
		if (t <= *time.begin()) return *strength.begin();
		if (t >= *time.rbegin()) return *strength.rbegin();

	}

private:
	eastl::vector<float> time;
	eastl::vector<float> strength;
	eastl::vector<EaseType> ease;
};

struct ParticleInfo
{
	double lifetimeMin, lifetimeMax;
	double sizeMin, sizeMax;
	double rotation;
	sf::Color beginColor, endColor;
};

class ParticleSystem
{
private:
	class Particle : public eloo::GameObject
	{
	public:
		Particle();

	public:
		virtual void update(const double deltaTime) override final;
		virtual void draw(sf::Window& window) override final;

	protected:
		virtual void onInit() override final;
		virtual void onCleanup() override final;
	};

public:
	ParticleSystem(const size_t maxParticleCount);

private:
	const size_t mMaxParticles;
	eloo::ObjectPool<Particle> mParticlePool;
};

}
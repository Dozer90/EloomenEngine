#pragma once

#include "Objects/GameObject.h"
#include "Objects/ObjectPool.h"

namespace eloo::Particles
{

using ParticleSystemID = uint8_t;

class ParticleSystem
{
private:
	class Particle : public GameObject
	{
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
	ObjectPool<Particle> mParticlePool;
};

}
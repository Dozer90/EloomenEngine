#pragma once

#include "Generic/Generic.h"
#include "Generic/TypeAliases.h"

#include <SFML/System/Vector2.hpp>

#include <EASTL/array.h>
#include <EASTL/vector.h>
#include <EASTL/unique_ptr.h>

#include <box2d/box2d.h>

#if defined(IMGUI_IS_ENABLED)
#include <imgui.h>
#endif

namespace eloo::Physics
{
constexpr uint8_t ChunkSize = 255;
class PhysicsSystem
{
private:
	using ChunkIndex = uint8_t;

	struct Chunk
	{
		Chunk(uint8_t allocSize);
		const uint8_t size;
		DataIndex indicesInUse = 0;
		DataIndex firstUnusedIndex = 0;
		bool isFull = false;

		eastl::unique_ptr<bool[]> inUse;
		eastl::unique_ptr<double[]> xPosition, yPosition;
		eastl::unique_ptr<double[]> xVelocity, yVelocity;
		eastl::unique_ptr<double[]> xAcceleration, yAcceleration;
		eastl::unique_ptr<double[]> xAngularVelocity, yAngularVelocity;
		eastl::unique_ptr<double[]> xForce, yForce;
		eastl::unique_ptr<double[]> mass;
	};

	struct ChunkInfoForID
	{
		ChunkIndex chunkIndex;
		DataIndex dataIndex;
	};

public:
	PhysicsSystem(uint8_t chunkAllocSize, uint8_t initialChunkReserveCount);

	/// <summary>
	/// Get a new ID for a game object to simulate physics
	/// </summary>
	/// <returns>ID required to set/get values from the system</returns>
	GameObjectID requestID();

	/// <summary>
	/// Frees up an active ID so another object can use it
	/// </summary>
	/// <param name="id">The ID to retire</param>
	void retireID(GameObjectID id);

	void update(const double deltaTime);

	sf::Vector2<double> getPosition(GameObjectID id) const;
	sf::Vector2<double> getVelocity(GameObjectID id) const;
	sf::Vector2<double> getAcceleration(GameObjectID id) const;
	double getMass(GameObjectID id) const;

	void setPosition(GameObjectID id, const sf::Vector2<double>& position);
	void setVelocity(GameObjectID id, const sf::Vector2<double>& velocity);
	void setAcceleration(GameObjectID id, const sf::Vector2<double>& acceleration);
	void setMass(GameObjectID id, double mass);

	void applyForce(GameObjectID id, const sf::Vector2<double>& force);
	void clearForce(GameObjectID id);
	void applyImpulse(GameObjectID id, const sf::Vector2<double>& impulse);
	void applyDamping(GameObjectID id, double dampingStrength);

	double getKineticEnergy(GameObjectID id) const;
	sf::Vector2<double> getMomentum(GameObjectID id) const;

private:
	inline ChunkIndex getChunkIndex(GameObjectID id) const
	{
		return static_cast<ChunkIndex>(id / ChunkSize);
	}
	inline DataIndex getDataIndex(GameObjectID id) const
	{
		return static_cast<DataIndex>(id % ChunkSize);
	}

	inline const Chunk& getChunk(GameObjectID id) const { return mChunks[getChunkIndex(id)]; }
	inline Chunk& getChunk(GameObjectID id) { return mChunks[getChunkIndex(id)]; }

	void updatePositions(const double deltaTime);
	void updateVelocities(const double deltaTime);
	void updateAccelerations(const double deltaTime);

private:
	const uint8_t mChunkSize;
	eastl::vector<Chunk> mChunks;
	uint64_t mFirstUnusedID = 0;
};
}
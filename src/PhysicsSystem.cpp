#include "Physics/PhysicsSystem.h"

namespace eloo::Physics
{

PhysicsSystem::PhysicsSystem(uint8_t chunkAllocSize, uint8_t initialChunkReserveCount)
	: mChunkSize(chunkAllocSize)
{
	mChunks.reserve(initialChunkReserveCount);
	mChunks.emplace_back(mChunkSize);
}

GameObjectID PhysicsSystem::requestID()
{
	const GameObjectID id = mFirstUnusedID;
	ChunkIndex chunkIndex = getChunkIndex(id);
	DataIndex dataIndex = getDataIndex(id);
	Chunk& chunk = mChunks[chunkIndex];

	// Set this index to in use and see if it's now full
	chunk.inUse[dataIndex] = true;
	chunk.isFull = ++chunk.indicesInUse == ChunkSize;

	// If this chunk still has space, we need to find the next available index
	if (!chunk.isFull) [[likely]]
	{
		while (++dataIndex < ChunkSize)
		{
			if (chunk.inUse[dataIndex])
			{
				continue;
			}

			// We've found an unused index
			chunk.firstUnusedIndex = dataIndex;
			mFirstUnusedID = chunkIndex * ChunkSize + dataIndex;
			return id;
		}
	}

	// This chunk is full, check for a free space in any others we have
	// allocated after this one
	chunk.firstUnusedIndex = ChunkSize;
	while (++chunkIndex < mChunks.size())
	{
		if (mChunks[chunkIndex].isFull)
		{
			continue;
		}

		// Found a chunk with an unused index
		mFirstUnusedID = chunkIndex * ChunkSize + mChunks[chunkIndex].firstUnusedIndex;
		return id;
	}

	// We have filled up all allocated chunks, we need a new one
	mChunks.emplace_back();
	mFirstUnusedID = chunkIndex * ChunkSize;
	return id;
}

void PhysicsSystem::retireID(GameObjectID id)
{
	const ChunkIndex chunkIndex = getChunkIndex(id);
	const DataIndex dataIndex = getDataIndex(id);
	Chunk& chunk = mChunks[chunkIndex];

	// Clear the data for this ID
	chunk.inUse[dataIndex] = false;
	chunk.xPosition[dataIndex] = chunk.yPosition[dataIndex] = 0.0;
	chunk.xVelocity[dataIndex] = chunk.yVelocity[dataIndex] = 0.0;
	chunk.xAcceleration[dataIndex] = chunk.yAcceleration[dataIndex] = 0.0;
	chunk.xAngularVelocity[dataIndex] = chunk.yAngularVelocity[dataIndex] = 0.0;
	chunk.xForce[dataIndex] = chunk.yForce[dataIndex] = 0.0;
	chunk.mass[dataIndex] = 0.0;

	chunk.isFull = false;
	--chunk.indicesInUse;

	// Set the new first unused vars (if required)
	if (chunk.firstUnusedIndex > dataIndex)
	{
		chunk.firstUnusedIndex = dataIndex;
	}
	const GameObjectID id = chunkIndex * ChunkSize + dataIndex;
	if (mFirstUnusedID > id)
	{
		mFirstUnusedID = id;
	}
}

sf::Vector2<double> PhysicsSystem::getPosition(GameObjectID id) const
{
	const DataIndex index = getDataIndex(id);
	const Chunk& chunk = getChunk(id);
	return sf::Vector2<double>{
		chunk.xPosition[index],
		chunk.yPosition[index]
	};
}

sf::Vector2<double> PhysicsSystem::getVelocity(GameObjectID id) const
{
	const DataIndex index = getDataIndex(id);
	const Chunk& chunk = getChunk(id);
	return sf::Vector2<double>{
		chunk.xVelocity[index],
		chunk.yVelocity[index]
	};
}

sf::Vector2<double> PhysicsSystem::getAcceleration(GameObjectID id) const
{
	const DataIndex index = getDataIndex(id);
	const Chunk& chunk = getChunk(id);
	return sf::Vector2<double>{
		chunk.xAcceleration[index],
		chunk.yAcceleration[index]
	};
}

double PhysicsSystem::getMass(GameObjectID id) const
{
	const DataIndex index = getDataIndex(id);
	const Chunk& chunk = getChunk(id);
	return chunk.mass[index];
}

void PhysicsSystem::setPosition(GameObjectID id, const sf::Vector2<double>& position)
{
	const DataIndex index = getDataIndex(id);
	Chunk& chunk = getChunk(id);
	chunk.xPosition[index] = position.x;
	chunk.yPosition[index] = position.y;
}

void PhysicsSystem::setVelocity(GameObjectID id, const sf::Vector2<double>& velocity)
{
	const DataIndex index = getDataIndex(id);
	Chunk& chunk = getChunk(id);
	chunk.xVelocity[index] = velocity.x;
	chunk.yVelocity[index] = velocity.y;
}

void PhysicsSystem::setAcceleration(GameObjectID id, const sf::Vector2<double>& acceleration)
{
	const DataIndex index = getDataIndex(id);
	Chunk& chunk = getChunk(id);
	chunk.xAcceleration[index] = acceleration.x;
	chunk.yAcceleration[index] = acceleration.y;
}

void PhysicsSystem::setMass(GameObjectID id, double mass)
{
	const DataIndex index = getDataIndex(id);
	Chunk& chunk = getChunk(id);
	chunk.mass[index] = mass;
}

void PhysicsSystem::applyForce(GameObjectID id, const sf::Vector2<double>& force)
{
	const DataIndex index = getDataIndex(id);
	Chunk& chunk = getChunk(id);
	chunk.xForce[index] += force.x;
	chunk.yForce[index] += force.y;
}

void PhysicsSystem::clearForce(GameObjectID id)
{
	const DataIndex index = getDataIndex(id);
	Chunk& chunk = getChunk(id);
	chunk.xForce[index] = 0.0;
	chunk.yForce[index] = 0.0;
}

void PhysicsSystem::applyImpulse(GameObjectID id, const sf::Vector2<double>& impulse)
{
	const DataIndex index = getDataIndex(id);
	Chunk& chunk = getChunk(id);
	chunk.xVelocity[index] += impulse.x / chunk.mass[index];
	chunk.yVelocity[index] += impulse.y / chunk.mass[index];
}

void PhysicsSystem::applyDamping(GameObjectID id, double dampingStrength)
{
	const DataIndex index = getDataIndex(id);
	Chunk& chunk = getChunk(id);
	chunk.xVelocity[index] *= dampingStrength;
	chunk.yVelocity[index] *= dampingStrength;
}

double PhysicsSystem::getKineticEnergy(GameObjectID id) const
{
	const DataIndex index = getDataIndex(id);
	const Chunk& chunk = getChunk(id);
	const double xVelocity = chunk.xVelocity[index];
	const double yVelocity = chunk.yVelocity[index];
	double velocitySqrMagnitude = xVelocity * xVelocity + yVelocity * yVelocity;
	return 0.5 * chunk.mass[index] * velocitySqrMagnitude;
}

sf::Vector2<double> PhysicsSystem::getMomentum(GameObjectID id) const
{
	const DataIndex index = getDataIndex(id);
	const Chunk& chunk = getChunk(id);
	return sf::Vector2<double>{
		chunk.xVelocity[index] * chunk.mass[index],
		chunk.yVelocity[index] * chunk.mass[index]
	};
}

}
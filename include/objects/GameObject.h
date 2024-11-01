#pragma once

#include "Generic/TypeAliases.h"

#include <SFML/Window.hpp>

namespace TankWarz
{
class IGameObject
{
private:
	IGameObject(GameObjectID id);

public:
	/// <summary>
	/// Initilize the object. This will be called when the object is first
	/// created OR recycled from the managing object pool
	/// </summary>
	virtual void init() = 0;

	/// <summary>
	/// Perform any cleanup required when the object is no longer in use
	/// </summary>
	virtual void cleanup() = 0;

	/// <summary>
	/// Update the game object
	/// </summary>
	/// <param name="deltaTime">Time between this frame and the last</param>
	virtual void update(const double deltaTime) = 0;

	/// <summary>
	/// Draw something to the current window
	/// </summary>
	/// <param name="window">The current window being rendered</param>
	virtual void draw(sf::Window& window) {};

public:
	/// <summary>
	/// Get the game object's unique ID
	/// </summary>
	inline GameObjectID getID() const { return mGameObjectID; }

	/// <summary>
	/// Called when the object is pulled from the pool. Used to reset the object
	/// so it can be used fresh
	/// </summary>
	void onAcquire(GameObjectID newID)
	{
		mGameObjectID = newID;
		init();
	}

private:
	GameObjectID mGameObjectID;
};
}
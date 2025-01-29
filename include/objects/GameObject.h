#pragma once

#include "Generic/TypeAliases.h"

#include <SFML/Window.hpp>

#include <EASTL/unique_ptr.h>

namespace eloo {
class GameObject {
private:
    virtual ~GameObject();

public:
    /// <summary>
    /// Called when the object is first created. It will also call onInit
    /// to allow derived classes to initilize anything they need to
    /// </summary>
    void init(GameObjectID id);

    /// <summary>
    /// Called when the object is deleted. It will also call onCleanup
    /// to allow derived classes to clean up anything they need to
    /// </summary>
    void cleanup();

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

    /// <summary>
    /// Get the game object's unique ID
    /// </summary>
    inline GameObjectID getID() const { return mGameObjectID; }

protected:
    /// <summary>
    /// Called when init() is fired. Override this to add any initial setup
    /// for your game object
    /// </summary>
    virtual void onInit() {}

    /// <summary>
    /// Called when cleanup() is fired. Override this to add any additional
    /// clean up for your game object
    /// </summary>
    virtual void onCleanup() {}

private:
    bool mIsInitilized = false;
    GameObjectID mGameObjectID = 0;
};

using GameObjectPtr = eastl::unique_ptr<GameObject>;
}
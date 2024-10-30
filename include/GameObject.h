#pragma once

#include <SFML/Window.hpp>

namespace GameObjects
{
	using GameObjectID = uint64_t;

	class IGameObject
	{
	protected:
		IGameObject();
		~IGameObject();

	public:
		/// <summary>
		/// Process an event and return if it is consumed
		/// </summary>
		/// <param name="event">Event triggered</param>
		/// <returns>True if the event was consumed</returns>
		virtual bool onEvent(const sf::Event& event) {};

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
		/// Get the game objects unique ID
		/// </summary>
		inline GameObjectID getID() const { return mGameObjectID; }

	private:
		const GameObjectID mGameObjectID;
	};
}
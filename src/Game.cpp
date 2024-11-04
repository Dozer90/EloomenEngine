#include "Game.h"

#include "Events/EventSystem.h"
#include "Scenes/SceneManager.h"
#include "TankBot.h"

#include <SFML/Graphics.hpp>
#include <EASTL/chrono.h>
#include <EASTL/vector.h>

using namespace eloo;

Events::EventSystem Game::gEventSystem;
Scenes::SceneManager Game::gSceneManager;

Game::Game()
	: mObjectPool(1000)
{}

void Game::deleteGameObject(GameObjectID id)
{
	if (mGameObjects.empty())
	{
		return;
	}

	constexpr auto condition = [&](const GameObjectPtr& obj, GameObjectID id)
	{
		return obj->getID() < id;
	};
	GameObjectPtr* found = eastl::lower_bound(mGameObjects.begin(), mGameObjects.end(), id, condition);
	if (found == mGameObjects.end() || (*found)->getID() != id)
	{
		return;
	}

	(*found)->cleanup();
	mGameObjects.erase(found);
}

int Game::run()
{
	auto window = sf::RenderWindow({ 1920u, 1080u }, "Playside EloomenateEngine!");
	window.setFramerateLimit(144);

	eastl::chrono::high_resolution_clock clock;

	while (window.isOpen())
	{
		for (auto event = sf::Event(); window.pollEvent(event);)
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		window.display();
	}
}
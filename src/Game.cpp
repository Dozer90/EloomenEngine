#include "Game.h"

#include "Objects/ObjectManager.h"
#include "Scenes/SceneManager.h"
#include "TankBot.h"

#include <SFML/Graphics.hpp>
#include <EASTL/chrono.h>
#include <EASTL/vector.h>

using namespace ENGINE_NAMESPACE;

ObjectManager Game::gObjectManager;
Scenes::SceneManager Game::gSceneManager;

Game::Game()
{
	gObjectManager.registerObjectType<TankBot>();

	// Do initilization stuff here
	Scenes::SceneID mainMenuSceneID = gSceneManager.registerScene<Scenes::MainMenuScene>();
}

int Game::run()
{
	auto window = sf::RenderWindow({ 1920u, 1080u }, "Playside TankWarz!");
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
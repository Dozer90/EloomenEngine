#include "Game.h"

#include "SceneManager.h"

#include <SFML/Graphics.hpp>
#include <EASTL/chrono.h>
#include <EASTL/vector.h>

Scenes::SceneManager Game::gSceneManager;

int Game()
{
	// Do initilization stuff here
	Scenes::SceneID mainMenuSceneID = gSceneManager.CreateNewScene();
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
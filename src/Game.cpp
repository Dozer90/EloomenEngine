#include <Game.h>
#include <SFML/Graphics.hpp>
#include <EASTL/vector.h>

int Game::run()
{
	auto window = sf::RenderWindow({ 1920u, 1080u }, "CMake SFML Project");
	window.setFramerateLimit(144);

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
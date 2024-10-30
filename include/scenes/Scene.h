#pragma once

#include <EASTL/functional.h>
#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace ENGINE_NAMESPACE::Scenes
{
	using SceneID = uint16_t;

	using TransitionFunc = eastl::function<SceneID()>;

	class Scene
	{
	public:
		Scene();

		virtual void update(const double deltaTime) = 0;

		virtual void render() = 0;

	private:
		eastl::string mName;
		eastl::vector<TransitionFunc> mTransitionConditions;
	};



	class MainMenuScene : public Scene
	{
	public:
	};
}
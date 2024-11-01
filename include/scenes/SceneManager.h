#pragma once

#include "Scene.h"

#include <EASTL/set.h>
#include <EASTL/unique_ptr.h>

namespace TankWarz::Scenes
{
class SceneManager
{
public:
	SceneManager();

	template <typename TScene>
	SceneID registerScene()
	{
		static_assert(eastl::is_base_of<Scene, TScene>::value, "TScene must derive from Scene");
		if (mScenes.insert(eastl::make_unique<TScene>()).second)
		{
			static SceneID uniqueSceneID = 0;
			return uniqueSceneID++;
		}
	}

private:
	eastl::set<eastl::unique_ptr<Scene>> mScenes;
};
}
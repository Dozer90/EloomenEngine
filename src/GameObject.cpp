#include "GameObject.h"

namespace GameObjects
{
static GameObjectID gNextGameObjectID = 0;

IGameObject::IGameObject()
	: mGameObjectID(gNextGameObjectID++)
{
}

IGameObject::~IGameObject()
{

}
}
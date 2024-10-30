#include "GameObject.h"

using namespace ENGINE_NAMESPACE;

static GameObjectID gNextGameObjectID = 0;

IGameObject::IGameObject()
	: mGameObjectID(gNextGameObjectID++)
{
}

IGameObject::~IGameObject()
{

}
#include "Objects/GameObject.h"

using namespace eloo;

void GameObject::init(GameObjectID id) {
    if (mIsInitilized) {
        return;
    }
    mGameObjectID = id;
    onInit();
    mIsInitilized = true;
}

void GameObject::cleanup() {
    if (!mIsInitilized) {
        return;
    }
    onCleanup();
    mGameObjectID = 0;
    mIsInitilized = false;
}
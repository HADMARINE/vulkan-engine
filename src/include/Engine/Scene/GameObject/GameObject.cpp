//
// Created by EunwooSong on 2020-11-05.
//

#include "GameObject.h"
using namespace SharkEngine::Core;

void GameObject::Init() {
    Entity::Init();

    transform = AddComponent<Transform>();
}


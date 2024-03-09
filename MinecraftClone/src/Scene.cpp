#include "Precompiled.h"

#include "Scene.h"

Scene::Scene(Camera* playerCamera)
    : mPlayer(playerCamera)
{
}

void Scene::Update(const float dt)
{
    mPlayer.Update(mWorld, dt);
    mWorld.Update();
}

void Scene::Render()
{
    mWorld.Render();
}

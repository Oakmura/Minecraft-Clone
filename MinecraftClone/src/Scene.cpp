#include "Precompiled.h"

#include "Scene.h"
#include "Managers/GraphicsResourceManager.h"

Scene::Scene(World* world)
    : mWorld(world)
{
}

void Scene::Update(const float dt)
{
    mWorld->Update();
}

void Scene::Render()
{
    mWorld->Render();
}

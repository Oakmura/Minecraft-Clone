#include "Precompiled.h"

#include "Scene.h"

Scene::Scene(GraphicsResourceManager& GRM, HWND windowHandle, Camera* playerCamera)
    : mWorld(GRM)
    , mPlayer(playerCamera)
{
}

void Scene::Update(const float dt)
{
    mPlayer.Update(mWorld, dt);
    mWorld.Update();
}

void Scene::Render(GraphicsResourceManager& GRM)
{
    mWorld.Render(GRM);
}

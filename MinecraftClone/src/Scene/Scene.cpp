#include "Precompiled.h"

#include "Scene/Scene.h"
#include "Core/GraphicsEngine.h"

Scene::Scene(World* world, Water* water, Clouds* clouds)
    : mWorld(world)
    , mWater(water)
    , mClouds(clouds)
{
}

void Scene::Update(const SimpleMath::Vector3& cameraPosition, const BlockHandler& blockHandler, const float dt)
{
    mWorld->Update(cameraPosition);
    mBlockMarker.Update(blockHandler);
    mClouds->Update(dt);
}

void Scene::Render(const BlockHandler& blockHandler)
{
    mWorld->Render();

    mClouds->Render();
    mWater->Render();

    mBlockMarker.Render(blockHandler);
}

#include "Precompiled.h"

#include "Scene.h"
#include "Managers/GraphicsResourceManager.h"

Scene::Scene(World* world, Water* water, Clouds* clouds)
    : mWorld(world)
    , mWater(water)
    , mClouds(clouds)
{
}

void Scene::Update(const SimpleMath::Vector3& cameraPosition, const VoxelHandler& voxelHandler, const float dt)
{
    mWorld->Update(cameraPosition);
    mVoxelMarker.Update(voxelHandler);
    mClouds->Update(dt);
}

void Scene::Render(const VoxelHandler& voxelHandler)
{
    mWorld->Render();

    mClouds->Render();
    mWater->Render();

    mVoxelMarker.Render(voxelHandler);
}

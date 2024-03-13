#include "Precompiled.h"

#include "Scene.h"
#include "Managers/GraphicsResourceManager.h"

Scene::Scene(World* world, Water* water)
    : mWorld(world)
    , mWater(water)
{
}

void Scene::Update(const SimpleMath::Vector3& cameraPosition, const VoxelHandler& voxelHandler, const float dt)
{
    mWorld->Update(cameraPosition);
    mVoxelMarker.Update(voxelHandler);
}

void Scene::Render(const VoxelHandler& voxelHandler)
{
    mWorld->Render();

    mWater->Render();

    mVoxelMarker.Render(voxelHandler);
}

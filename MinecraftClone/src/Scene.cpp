#include "Precompiled.h"

#include "Scene.h"
#include "Managers/GraphicsResourceManager.h"

Scene::Scene(World* world)
    : mWorld(world)
{
}

void Scene::Update(const VoxelHandler& voxelHandler, const float dt)
{
    mWorld->Update();
    mVoxelMarker.Update(voxelHandler);
}

void Scene::Render(const VoxelHandler& voxelHandler)
{
    mWorld->Render();
    mVoxelMarker.Render(voxelHandler);
}

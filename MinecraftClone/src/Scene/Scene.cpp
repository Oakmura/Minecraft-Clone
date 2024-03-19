#include "Precompiled.h"

#include "Scene/Scene.h"
#include "Utils/Hasher.h"
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
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    GraphicsPsoLibrary& psoLibrary = ge.GetGraphicsPsoLibrary();

    mWorld->Render();

    psoLibrary.Get(Hasher::Hash("bothSolidAlpha")).SetPipelineState(); // render without cull face
    mClouds->Render();
    mWater->Render();

    psoLibrary.Get(Hasher::Hash("defaultSolidAlpha")).SetPipelineState();
    mBlockMarker.Render(blockHandler);
}

#include "Precompiled.h"

#include "Scene/Scene.h"
#include "Utils/Hasher.h"
#include "Core/GraphicsEngine.h"

Scene::Scene(World* world, Water* water, Clouds* clouds, Player* player)
    : mWorld(world)
    , mWater(water)
    , mClouds(clouds)
    , mPlayer(player)
{
    mGlobalCB.GetCPU().CameraPosition = SimpleMath::Vector3();
    mGlobalCB.GetCPU().WaterLine = 5.6f;
    mGlobalCB.GetCPU().BackgroundColor = { 0.58f, 0.83f, 0.99f };
    mGlobalCB.GetCPU().FogStrength = 1.0f;
    D3D11Utils::CreateConstantBuffer(GraphicsEngine::GetInstance().GetDevice(), mGlobalCB.GetCPU(), &mGlobalCB.GetGPU());
}

void Scene::Update(const float dt)
{
    mPlayer->Update(*mWorld, dt);

    mGlobalCB.GetCPU().CameraPosition = mPlayer->GetPosition();
    D3D11Utils::UpdateBuffer(GraphicsEngine::GetInstance().GetDeviceContext(), mGlobalCB.GetCPU(), mGlobalCB.GetGPU());

    mClouds->Update(dt);
}

void Scene::Render()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    GraphicsPsoLibrary& psoLibrary = ge.GetGraphicsPsoLibrary();

    mGlobalCB.UseOn(eShader::Pixel, 0);

    mWorld->Render();

    psoLibrary.Get(Hasher::Hash("bothSolidAlpha")).SetPipelineState(); // render without cull face
    mClouds->Render();
    mWater->Render();

    psoLibrary.Get(Hasher::Hash("defaultSolidAlpha")).SetPipelineState();
    mPlayer->Render();
}

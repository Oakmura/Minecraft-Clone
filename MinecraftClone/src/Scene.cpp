#include "Precompiled.h"

#include "Scene.h"

Scene::Scene(GraphicsResourceManager& GRM)
    : mWorld(GRM)
{
}

void Scene::Render(GraphicsResourceManager& GRM)
{
    mWorld.Render(GRM);
}

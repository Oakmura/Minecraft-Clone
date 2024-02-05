#pragma once

#include "World.h"
#include "Managers/GraphicsResourceManager.h"

class Scene final
{
    friend class Renderer;
    
public:
    Scene(GraphicsResourceManager& GRM);
    ~Scene() = default;

    void Render(GraphicsResourceManager& GRM);

private:
    World mWorld;
};


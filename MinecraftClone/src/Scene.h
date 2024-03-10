#pragma once

#include "World.h"
#include "VoxelMarker.h"

class Scene final
{    
public:
    Scene(World* world);
    ~Scene() = default;

    void Update(const float dt);
    void Render();

private:
    World* mWorld;
    VoxelMarker mVoxelMarker;
};


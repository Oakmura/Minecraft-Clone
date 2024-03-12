#pragma once

#include "World.h"
#include "VoxelMarker.h"

class Scene final
{    
public:
    Scene(World* world);
    ~Scene() = default;

    void Update(const VoxelHandler& voxelHandler, const float dt);
    void Render(const VoxelHandler& voxelHandler);

private:
    World* mWorld;
    VoxelMarker mVoxelMarker;
};


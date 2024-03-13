#pragma once

#include "World.h"
#include "Water.h"
#include "VoxelMarker.h"

class Scene final
{    
public:
    Scene(World* world, Water* water);
    ~Scene() = default;

    void Update(const SimpleMath::Vector3& cameraPosition, const VoxelHandler& voxelHandler, const float dt);
    void Render(const VoxelHandler& voxelHandler);

private:
    World* mWorld;
    Water* mWater;
    VoxelMarker mVoxelMarker;
};


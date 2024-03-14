#pragma once

#include "World.h"
#include "Water.h"
#include "Clouds.h"
#include "VoxelMarker.h"

class Scene final
{    
public:
    Scene(World* world, Water* water, Clouds* clouds);
    ~Scene() = default;

    void Update(const SimpleMath::Vector3& cameraPosition, const VoxelHandler& voxelHandler, const float dt);
    void Render(const VoxelHandler& voxelHandler);

private:
    World* mWorld;
    Water* mWater;
    Clouds* mClouds;
    VoxelMarker mVoxelMarker;
};


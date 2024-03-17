#pragma once

#include "World.h"
#include "Water.h"
#include "Clouds.h"
#include "BlockMarker.h"

class Scene final
{    
public:
    Scene(World* world, Water* water, Clouds* clouds);
    ~Scene() = default;
    Scene(const Scene& rhs) = delete;
    Scene& operator=(const Scene& rhs) = delete;

    void Update(const SimpleMath::Vector3& cameraPosition, const BlockHandler& blockHandler, const float dt);
    void Render(const BlockHandler& blockHandler);

private:
    World* mWorld;
    Water* mWater;
    Clouds* mClouds;
    BlockMarker mBlockMarker;
};
#pragma once

#include "Player.h"
#include "World.h"
#include "VoxelMarker.h"
#include "Managers/GraphicsResourceManager.h"

class Scene final
{    
public:
    Scene(Camera* playerCamera);
    ~Scene() = default;

    void Update(const float dt);
    void Render();

    inline Player& GetPlayer() { return mPlayer; }

private:
    Player mPlayer;
    World mWorld;
    VoxelMarker mVoxelMarker;
};


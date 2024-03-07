#pragma once

#include "Player.h"
#include "World.h"
#include "VoxelMarker.h"
#include "Managers/GraphicsResourceManager.h"

class Scene final
{
    friend class Renderer;
    
public:
    Scene(GraphicsResourceManager& GRM, HWND windowHandle, Camera* playerCamera);
    ~Scene() = default;

    void Update(const float dt);
    void Render(GraphicsResourceManager& GRM);

    inline Player& GetPlayer() { return mPlayer; }

private:
    Player mPlayer;
    World mWorld;
    VoxelMarker mVoxelMarker;
};


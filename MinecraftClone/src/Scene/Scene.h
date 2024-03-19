#pragma once

#include "World.h"
#include "Water.h"
#include "Clouds.h"
#include "Player.h"

class Scene final
{
    friend class ImGuiUI;
public:
    Scene(World* world, Water* water, Clouds* clouds, Player* player);
    ~Scene() = default;
    Scene(const Scene& rhs) = delete;
    Scene& operator=(const Scene& rhs) = delete;

    void Update(const float dt);
    void Render();

private:
    World* mWorld;
    Water* mWater;
    Clouds* mClouds;
    Player* mPlayer;

    ConstantBuffer<GlobalCB> mGlobalCB;
};
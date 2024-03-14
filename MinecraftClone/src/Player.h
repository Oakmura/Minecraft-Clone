#pragma once

#include "Camera.h"
#include "VoxelHandler.h"
#include "World.h"

class Player final
{
    friend class ImGuiUI;
public:
    Player(Camera* playerCamera);

    void HandleInput();
    void Update(World& world, float dt);

    inline const SimpleMath::Vector3& GetPosition() const { return mPlayerCamera->GetEyePos(); }
    inline const SimpleMath::Vector3& GetForward() const { return mPlayerCamera->GetForward(); }
    inline const SimpleMath::Vector3& GetUp() const { return mPlayerCamera->GetUp(); }
    inline const SimpleMath::Vector3& GetRight() const { return mPlayerCamera->GetRight(); }

    inline SimpleMath::Matrix GetViewMatrix() const { return mPlayerCamera->GetViewMatrix(); }
    inline SimpleMath::Matrix GetProjMatrix() const { return mPlayerCamera->GetProjMatrix(); }

    inline const VoxelHandler& GetVoxelHandler() const { return mVoxelHandler; }

private:
    static constexpr float MOUSE_SENSITIVITY = 0.003f;
    static constexpr float PLAYER_SPEED = 15.f;

    Camera* mPlayerCamera;
    VoxelHandler mVoxelHandler;
};
#pragma once

#include "Settings.h"
#include "Camera.h"
#include "BlockHandler.h"
#include "World.h"

class Player final
{
    friend class ImGuiUI;
public:
    Player(Camera* playerCamera);

    void HandleInput();
    void Update(World& world, const float dt);

    inline const SimpleMath::Vector3& GetPosition() const { return mPlayerCamera->GetEyePos(); }
    inline const SimpleMath::Vector3& GetForward() const { return mPlayerCamera->GetForward(); }
    inline const SimpleMath::Vector3& GetUp() const { return mPlayerCamera->GetUp(); }
    inline const SimpleMath::Vector3& GetRight() const { return mPlayerCamera->GetRight(); }

    inline SimpleMath::Matrix GetViewMatrix() const { return mPlayerCamera->GetViewMatrix(); }
    inline SimpleMath::Matrix GetProjMatrix() const { return mPlayerCamera->GetProjMatrix(); }

    inline const BlockHandler& GetBlockHandler() const { return mBlockHandler; }

private:
    Camera* mPlayerCamera;
    BlockHandler mBlockHandler;

    float mVelocityY;
};
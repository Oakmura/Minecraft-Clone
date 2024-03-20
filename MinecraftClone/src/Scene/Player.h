#pragma once

#include "Settings.h"
#include "Camera.h"
#include "BlockHandler.h"
#include "BlockMarker.h"
#include "Scene/World.h"

class Player final
{
    friend class ImGuiUI;
public:
    Player() = delete;
    Player(Camera* playerCamera);
    ~Player() = default;
    Player(const Player& rhs) = delete;
    Player& operator=(const Player& rhs) = delete;

    void HandleInput();
    void Update(World& world, const float dt);
    void Render();

    inline const SimpleMath::Vector3& GetPosition() const { return mPlayerCamera->GetEyePos(); }
    inline const SimpleMath::Vector3& GetForward() const { return mPlayerCamera->GetForward(); }
    inline const SimpleMath::Vector3& GetUp() const { return mPlayerCamera->GetUp(); }
    inline const SimpleMath::Vector3& GetRight() const { return mPlayerCamera->GetRight(); }

    inline SimpleMath::Matrix GetViewMatrix() const { return mPlayerCamera->GetViewMatrix(); }
    inline SimpleMath::Matrix GetProjMatrix() const { return mPlayerCamera->GetProjMatrix(); }

private:
    void handleCollision();

private:
    Camera* mPlayerCamera;
    BlockHandler mBlockHandler;
    BlockMarker mBlockMarker;

    float mVelocityY;
};
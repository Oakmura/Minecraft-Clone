#pragma once

#include "Camera.h"
#include "VoxelHandler.h"
#include "World.h"

enum eMouseButtonType
{
    LEFT,
    RIGHT,
};

class Player final
{
    friend class UserInterface;
public:
    Player(Camera* playerCamera);

    void Update(World& world, float dt);

    void OnMouseButtonDown(eMouseButtonType mouseButtonType);
    void OnMouseMove(const int mouseX, const int mouseY);
    void OnKeyboardPress(const int keyCode);
    void OnKeyboardRelease(const int keyCode);

    inline const SimpleMath::Vector3& GetPosition() const { return mPlayerCamera->GetEyePos(); }
    inline const SimpleMath::Vector3& GetForward() const { return mPlayerCamera->GetForward(); }
    inline const SimpleMath::Vector3& GetUp() const { return mPlayerCamera->GetUp(); }
    inline const SimpleMath::Vector3& GetRight() const { return mPlayerCamera->GetRight(); }

    inline SimpleMath::Matrix GetViewMatrix() const { return mPlayerCamera->GetViewMatrix(); }
    inline SimpleMath::Matrix GetProjMatrix() const { return mPlayerCamera->GetProjMatrix(); }

private:
    static constexpr float MOUSE_SENSITIVITY = 0.003f;
    static constexpr float PLAYER_SPEED = 15.f;

    Camera* mPlayerCamera;
    VoxelHandler mVoxelHandler;

    bool mbLockMouseRotation = true;
    bool mKeyboardState[256] = { 0, };
};
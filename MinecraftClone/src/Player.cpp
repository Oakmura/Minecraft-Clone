#include "Precompiled.h"

#include "Player.h"
#include "Managers/InputManager.h"

Player::Player(Camera* playerCamera)
    : mPlayerCamera(playerCamera)
{
}

void Player::HandleInput()
{
    InputManager& inputManager = InputManager::GetInstance();

    if (inputManager.IsPressed(eInputButton::MOUSE_L))
    {
        mVoxelHandler.SetVoxel();
    }
    else if (inputManager.IsPressed(eInputButton::MOUSE_R))
    {
        mVoxelHandler.SwitchInteractionMode();
    }

    if (inputManager.IsPressed(eInputButton::F))
    {
        inputManager.ToggleInputLock();

        ::ShowCursor(inputManager.IsInputLock());
    }
}

void Player::Update(World& world, float dt)
{
    InputManager& inputManager = InputManager::GetInstance();
    if (inputManager.IsInputLock())
    {
        return;
    }

    const IntVector2D& mouseRelativeChange = inputManager.GetMouseRelativeChange();
    mPlayerCamera->RotateYaw(mouseRelativeChange.mX * MOUSE_SENSITIVITY);
    mPlayerCamera->RotatePitch(mouseRelativeChange.mY * MOUSE_SENSITIVITY);

    const float yaw = mPlayerCamera->GetYawInRadian();
    const float pitch = mPlayerCamera->GetPitchInRadian();

    const float sinY = sin(yaw);
    const float cosY = cos(yaw);
    const float sinP = sin(pitch);
    const float cosP = cos(pitch);

    SimpleMath::Vector3& forward = mPlayerCamera->GetForward();
    SimpleMath::Vector3& right = mPlayerCamera->GetRight();
    SimpleMath::Vector3& up = mPlayerCamera->GetUp();

    forward = SimpleMath::Vector3(sinY * cosP, -sinP, cosY * cosP);
    right = SimpleMath::Vector3(cosY, 0.f, -sinY);
    up = SimpleMath::Vector3(sinY * sinP, cosP, cosY * sinP);

    const float deltaPosition = PLAYER_SPEED * dt;
    SimpleMath::Vector3& position = mPlayerCamera->GetEyePos();

    if (inputManager.IsPressed(eInputButton::W) || inputManager.IsPressing(eInputButton::W))
    {
        position += forward * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::S) || inputManager.IsPressing(eInputButton::S))
    {
        position -= forward * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::D) || inputManager.IsPressing(eInputButton::D))
    {
        position += right * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::A) || inputManager.IsPressing(eInputButton::A))
    {
        position -= right * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::E) || inputManager.IsPressing(eInputButton::E))
    {
        position += up * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::Q) || inputManager.IsPressing(eInputButton::Q))
    {
        position -= up * deltaPosition;
    }

    mVoxelHandler.Update(*this); // #TODO move to difference place?
}
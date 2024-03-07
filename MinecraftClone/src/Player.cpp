#include "Precompiled.h"

#include "Player.h"

Player::Player(Camera* playerCamera)
    : mPlayerCamera(playerCamera)
{
}

void Player::Update(World& world, float dt)
{
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

    SimpleMath::Vector3& position = mPlayerCamera->GetEyePos();
    if (mKeyboardState['W'])
    {
        position += forward * PLAYER_SPEED * dt;
    }

    if (mKeyboardState['S'])
    {
        position -= forward * PLAYER_SPEED * dt;
    }

    if (mKeyboardState['D'])
    {
        position += right * PLAYER_SPEED * dt;
    }

    if (mKeyboardState['A'])
    {
        position -= right * PLAYER_SPEED * dt;
    }

    if (mKeyboardState['E'])
    {
        position += up * PLAYER_SPEED * dt;
    }

    if (mKeyboardState['Q'])
    {
        position -= up * PLAYER_SPEED * dt;
    }

    mVoxelHandler.Update(*this);
}

void Player::OnMouseButtonDown(eMouseButtonType mouseButtonType)
{
    if (mouseButtonType == eMouseButtonType::LEFT)
    {
        mVoxelHandler.SetVoxel();
    }
    else
    {
        mVoxelHandler.ToggleInteractionMode();
    }
}

void Player::OnMouseMove(const int mouseX, const int mouseY)
{
    static int sPreviousMouseX = mouseX;
    static int sPreviousMouseY = mouseY;

    if (mbLockMouseRotation)
    {
        sPreviousMouseX = mPlayerCamera->GetRelativeScreenCenterX();
        sPreviousMouseY = mPlayerCamera->GetRelativeScreenCenterY();

        return;
    }

    int deltaX = mouseX - sPreviousMouseX;
    int deltaY = mouseY - sPreviousMouseY;
    
    mPlayerCamera->RotateYaw(deltaX * MOUSE_SENSITIVITY);
    mPlayerCamera->RotatePitch(deltaY * MOUSE_SENSITIVITY);

    int maxRadius = mPlayerCamera->GetScreenHeight() / 3;
    IntVector2D center = IntVector2D(mPlayerCamera->GetRelativeScreenCenterX(), mPlayerCamera->GetRelativeScreenCenterY());
    IntVector2D distanceFromCenter = IntVector2D(sPreviousMouseX, sPreviousMouseY) - center;
    float len = sqrt(static_cast<float>(distanceFromCenter.mX * distanceFromCenter.mX + distanceFromCenter.mY * distanceFromCenter.mY));
    if (len > maxRadius)
    {
        POINT pt = { center.mX, center.mY };
        ClientToScreen(GetActiveWindow(), &pt);
        SetCursorPos(pt.x, pt.y);

        sPreviousMouseX = center.mX;
        sPreviousMouseY = center.mY;
    }
    else
    {
        sPreviousMouseX = mouseX;
        sPreviousMouseY = mouseY;
    }
}

void Player::OnKeyboardPress(const int keyCode)
{
    ASSERT(keyCode >= 0 && keyCode < 256, "Invalid key code");

    mKeyboardState[keyCode] = true;

    if (mKeyboardState['F'])
    {
        mbLockMouseRotation = !mbLockMouseRotation;
        mKeyboardState['F'] = false;

        ::ShowCursor(mbLockMouseRotation);
    }
}

void Player::OnKeyboardRelease(const int keyCode)
{
    ASSERT(keyCode >= 0 && keyCode < 256, "Invalid key code");

    mKeyboardState[keyCode] = false;
}
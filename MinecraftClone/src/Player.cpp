#include "Precompiled.h"

#include "Player.h"

void Player::Update(float dt)
{
    const float yaw = mPlayerCamera.GetYawInRadian();
    const float pitch = mPlayerCamera.GetPitchInRadian();

    const float sinY = sin(yaw); 
    const float cosY = cos(yaw);
    const float sinP = sin(pitch); 
    const float cosP = cos(pitch);

    Vector3& forward = mPlayerCamera.GetForward();
    Vector3& right = mPlayerCamera.GetRight();
    Vector3& up = mPlayerCamera.GetUp();

    forward = Vector3(sinY * cosP, -sinP, cosY * cosP);
    right = Vector3(cosY, 0.f, -sinY);
    up = Vector3(sinY * sinP, cosP, cosY * sinP);

    Vector3& position = mPlayerCamera.GetEyePos();
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
}

void Player::OnMouseMove(const int mouseX, const int mouseY)
{
    if (mPreviousMouseX == -1 || mPreviousMouseY == -1 || mbFirstPointView)
    {
        mPreviousMouseX = mouseX;
        mPreviousMouseY = mouseY;

        return;
    }

    int deltaX = mouseX - mPreviousMouseX;
    int deltaY = mouseY - mPreviousMouseY;
    
    mPlayerCamera.RotateYaw(deltaX * MOUSE_SENSITIVITY);
    mPlayerCamera.RotatePitch(deltaY * MOUSE_SENSITIVITY);

    mPreviousMouseX = mouseX;
    mPreviousMouseY = mouseY;
}

void Player::OnKeyboardPress(const int keyCode)
{
    ASSERT(keyCode >= 0 && keyCode < 256, "Invalid key code");

    mKeyboardState[keyCode] = true;

    if (mKeyboardState['F'])
    {
        mbFirstPointView = !mbFirstPointView;
        mKeyboardState['F'] = false;
    }
}

void Player::OnKeyboardRelease(const int keyCode)
{
    ASSERT(keyCode >= 0 && keyCode < 256, "Invalid key code");

    mKeyboardState[keyCode] = false;
}
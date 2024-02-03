#pragma once

#include "Camera.h"

class Player final
{
    friend class UserInterface;
public:
    Player(Camera* playerCamera) : mPlayerCamera(playerCamera) { }

    void Update(float dt);

    void OnMouseMove(const int mouseX, const int mouseY);
    void OnKeyboardPress(const int keyCode);
    void OnKeyboardRelease(const int keyCode);

    inline Matrix GetViewMatrix() const { return mPlayerCamera->GetViewMatrix(); }
    inline Matrix GetProjMatrix() const { return mPlayerCamera->GetProjMatrix(); }

private:
    static constexpr float MOUSE_SENSITIVITY = 0.003f;
    static constexpr float PLAYER_SPEED = 15.f;

    Camera* mPlayerCamera;

    bool mbLockMouseRotation = true;
    bool mKeyboardState[256] = { 0, };
};
#pragma once

#include "Camera.h"

class Player final
{
    friend class UserInterface;
public:
    void Update(float dt);

    void OnMouseMove(const int mouseX, const int mouseY);
    void OnKeyboardPress(const int keyCode);
    void OnKeyboardRelease(const int keyCode);

    inline Matrix GetViewMatrix() const { return mPlayerCamera.GetViewMatrix(); }
    inline Matrix GetProjMatrix() const { return mPlayerCamera.GetProjMatrix(); }

private:
    static constexpr float MOUSE_SENSITIVITY = 0.002f;
    static constexpr float PLAYER_SPEED = 3.f;

    Camera mPlayerCamera;

    int mPreviousMouseX = -1;
    int mPreviousMouseY = -1;

    bool mbFirstPointView = true;
    bool mKeyboardState[256] = { 0, };
};
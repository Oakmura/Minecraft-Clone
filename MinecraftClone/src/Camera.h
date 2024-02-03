#pragma once

class Camera final
{
    friend class UserInterface;
public:
    Camera(HWND windowHandle);

public:
    Matrix GetViewMatrix() const;
    Matrix GetProjMatrix() const;

    inline Vector3& GetEyePos() { return mPos; }
    inline Vector3& GetForward() { return mForward; }
    inline Vector3& GetUp() { return mUp; }
    inline Vector3& GetRight() { return mRight; }
    inline float GetYawInRadian() { return mYawInRadian; }
    inline float GetPitchInRadian() { return mPitchInRadian; }

    inline int GetScreenWidth() { return mScreenWidth; }
    inline int GetScreenHeight() { return mScreenHeight; }
    inline int GetScreenOffsetX() { return mWindowRect.left; }
    inline int GetScreenOffsetY() { return mWindowRect.top; }

    inline int GetAbsoluteScreenCenterX() { return mScreenAbsoluteCenterX; }
    inline int GetAbsoluteScreenCenterY() { return mScreenAbsoluteCenterY; }
    inline int GetRelativeScreenCenterX() { return mScreenWidth >> 1; }
    inline int GetRelativeScreenCenterY() { return mScreenHeight >> 1; }

    inline void SetEyePos(const Vector3& pos) { mPos = pos; }
    inline void SetViewportSize(const IntVector2D& screenSize) { mScreenWidth = screenSize.mX; mScreenHeight = screenSize.mY; };

    void RotateYaw(const float deltaYaw);
    void RotatePitch(const float deltaPitch);

private:
    enum { PITCH_MAX = 87 };

    RECT mWindowRect;
    HWND mWindowHandle;
    int mScreenAbsoluteCenterX;
    int mScreenAbsoluteCenterY;

    Vector3 mPos{ 0.f, 0.f, -3.f };
    Vector3 mUp{ 0.f, 1.f, 0.f };
    Vector3 mRight{ 1.f, 0.f, 0.f };
    Vector3 mForward{ 0.f, 0.f, 1.f };
    
    float mYawInRadian = 0.f;
    float mPitchInRadian = 0.f;

    float mFOV = 45.f;
    float mNearZ = 0.01f;
    float mFarZ = 100.f;

    int mScreenWidth = 1280;
    int mScreenHeight = 720;
};
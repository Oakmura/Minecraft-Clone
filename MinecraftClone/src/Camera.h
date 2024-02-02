#pragma once

class Camera final
{
    friend class UserInterface;
public:
    Camera() = default;
    Camera(Vector3 pos) : mPos(pos) {}
    Camera(Vector3 pos, float yawInRadian, float pitchInRadian) : mPos(pos), mYawInRadian(yawInRadian), mPitchInRadian(pitchInRadian) {}

public:
    inline void SetEyePos(const Vector3& pos) { mPos = pos; }
    inline void SetViewportSize(const IntVector2D& screenSize) { mWidth = screenSize.mX; mHeight = screenSize.mY; };
    
    Matrix GetViewMatrix() const;
    Matrix GetProjMatrix() const;

private:
    Vector3 mPos{ 0.f, 0.f, -1.f };
    Vector3 mUp{ 0.f, 1.f, 0.f };
    Vector3 mRight{ 1.f, 0.f, 0.f };
    Vector3 mForward{ 0.f, 0.f, 1.f };
    
    float mYawInRadian = 0.f;
    float mPitchInRadian = 0.f;

    float mFOV = 45.f;
    float mNearZ = 0.01f;
    float mFarZ = 100.f;

    int mWidth = 1280;
    int mHeight = 720;
};
#pragma once

class Camera final
{
public:
    Camera() = default;
    Camera(Vector3 pos) : mPos(pos) {}
    Camera(Vector3 pos, float yaw, float pitch) : mPos(pos), mYaw(yaw), mPitch(pitch) {}

public:
    inline void SetEyePos(const Vector3& pos) { mPos = pos; }
    inline void SetViewportSize(const IntVector2D& screenSize) { mWidth = screenSize.mX; mHeight = screenSize.mY; };

private:
    Vector3 mPos{ 0.f, 0.f, 0.f };
    Vector3 mUp{ 0.f, 1.f, 0.f };
    Vector3 mRight{ 1.f, 0.f, 0.f };
    Vector3 mForward{ 0.f, 0.f, 1.f };
    
    float mYaw = 0.f;
    float mPitch = 0.f;

    float mFOV = 45.f;
    float mNearZ = 0.01f;
    float mFarZ = 100.f;

    int mWidth = 1280;
    int mHeight = 720;
};
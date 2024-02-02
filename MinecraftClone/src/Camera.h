#pragma once

class Camera final
{
    friend class UserInterface;
public:
    Camera() = default;
    Camera(Vector3 pos) : mPos(pos) {}
    Camera(Vector3 pos, const float yawInRadian, const float pitchInRadian) : mPos(pos), mYawInRadian(yawInRadian), mPitchInRadian(pitchInRadian) {}

public:
    Matrix GetViewMatrix() const;
    Matrix GetProjMatrix() const;

    inline Vector3& GetEyePos() { return mPos; }
    inline Vector3& GetForward() { return mForward; }
    inline Vector3& GetUp() { return mUp; }
    inline Vector3& GetRight() { return mRight; }

    inline float GetYawInRadian() { return mYawInRadian; }
    inline float GetPitchInRadian() { return mPitchInRadian; }

    inline void SetEyePos(const Vector3& pos) { mPos = pos; }
    inline void SetViewportSize(const IntVector2D& screenSize) { mWidth = screenSize.mX; mHeight = screenSize.mY; };

    void RotateYaw(const float deltaYaw);
    void RotatePitch(const float deltaPitch);

private:
    void updateOrientation();

private:
    enum { PITCH_MAX = 89 };

    Vector3 mPos{ 0.f, 0.f, -3.f };
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
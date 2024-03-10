#pragma once

class Camera final
{
    friend class UserInterface;
public:
    Camera() = default;

public:
    SimpleMath::Matrix GetViewMatrix() const;
    SimpleMath::Matrix GetProjMatrix() const;

    inline const SimpleMath::Vector3& GetEyePos() const { return mPos; }
    inline const SimpleMath::Vector3& GetForward() const { return mForward; }
    inline const SimpleMath::Vector3& GetUp() const { return mUp; }
    inline const SimpleMath::Vector3& GetRight() const { return mRight; }

    inline SimpleMath::Vector3& GetEyePos() { return mPos; }
    inline SimpleMath::Vector3& GetForward() { return mForward; }
    inline SimpleMath::Vector3& GetUp() { return mUp; }
    inline SimpleMath::Vector3& GetRight() { return mRight; }

    inline float GetYawInRadian() { return mYawInRadian; }
    inline float GetPitchInRadian() { return mPitchInRadian; }

    inline int GetScreenWidth() { return mScreenSize.mX; }
    inline int GetScreenHeight() { return mScreenSize.mY; }
    inline const IntVector2D& GetScreenSize() const { return mScreenSize; }

    inline void SetEyePos(const SimpleMath::Vector3& pos) { mPos = pos; }
    inline void SetViewportSize(const IntVector2D& screenSize) { mScreenSize = screenSize; };

    void RotateYaw(const float deltaYaw);
    void RotatePitch(const float deltaPitch);

private:
    enum { PITCH_MAX = 87 };

    SimpleMath::Vector3 mPos{ 6.1f, 94.f, 76.f };
    SimpleMath::Vector3 mUp{ 0.f, 1.f, 0.f };
    SimpleMath::Vector3 mRight{ 1.f, 0.f, 0.f };
    SimpleMath::Vector3 mForward{ 0.f, 0.f, 1.f };
    
    float mYawInRadian = -5.3f;
    float mPitchInRadian = 0.45f;

    float mFOV = 45.f;
    float mNearZ = 0.01f;
    float mFarZ = 2000.f;

    IntVector2D mScreenSize = { 1280, 720 };
};
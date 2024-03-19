#pragma once

#include "Settings.h"

class Camera final
{
    friend class ImGuiUI;
public:
    Camera() = default;
    ~Camera() = default;
    Camera(const Camera& rhs) = delete;
    Camera& operator=(const Camera& rhs) = delete;

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
    inline float GetFovXInRadian() { return XMConvertToRadians(mFovX); }
    inline float GetFovYInRadian() { return XMConvertToRadians(mFovY); }

    inline int GetScreenWidth() { return mScreenSize.mX; }
    inline int GetScreenHeight() { return mScreenSize.mY; }
    inline const IntVector2D& GetScreenSize() const { return mScreenSize; }

    inline void SetEyePos(const SimpleMath::Vector3& pos) { mPos = pos; }
    inline void SetViewportSize(const IntVector2D& screenSize) { mScreenSize = screenSize; };

    inline float GetNearZ() const { return mNearZ; }
    inline float GetFarZ() const { return mFarZ; }

    void RotateYaw(const float deltaYaw);
    void RotatePitch(const float deltaPitch);

private:
    enum { PITCH_MAX = 87 };

    SimpleMath::Vector3 mPos = def::g_PLAYER_POS;
    SimpleMath::Vector3 mUp{ 0.f, 1.f, 0.f };
    SimpleMath::Vector3 mRight{ 1.f, 0.f, 0.f };
    SimpleMath::Vector3 mForward{ 0.f, 0.f, 1.f };
    
    float mYawInRadian = -5.3f;
    float mPitchInRadian = 0.45f;

    float mFovX = 80.0f;
    float mFovY = 50.0f;
    float mNearZ = def::g_NEAR_Z;
    float mFarZ = def::g_FAR_Z;

    IntVector2D mScreenSize = def::g_SCREEN_SIZE;
};
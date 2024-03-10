#include "Precompiled.h"

#include "Camera.h"
#include "Managers/WindowManager.h"

SimpleMath::Matrix Camera::GetViewMatrix() const
{
    return SimpleMath::Matrix::CreateTranslation(-mPos) * SimpleMath::Matrix::CreateRotationY(-mYawInRadian) * SimpleMath::Matrix::CreateRotationX(-mPitchInRadian);
}

SimpleMath::Matrix Camera::GetProjMatrix() const
{
    return XMMatrixPerspectiveFovLH(XMConvertToRadians(mFOV), mScreenSize.mX / static_cast<float>(mScreenSize.mY), mNearZ, mFarZ);
}

void Camera::RotateYaw(const float deltaYaw)
{
    mYawInRadian += deltaYaw;
}

void Camera::RotatePitch(const float deltaPitch)
{
    mPitchInRadian += deltaPitch;
    mPitchInRadian = std::clamp(mPitchInRadian, XMConvertToRadians(-PITCH_MAX), XMConvertToRadians(PITCH_MAX));
}
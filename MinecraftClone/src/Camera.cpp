#include "Precompiled.h"

#include "Camera.h"

Camera::Camera(HWND windowHandle)
    : mWindowHandle(windowHandle)
{
    ::GetWindowRect(windowHandle, &mWindowRect);
    ::MapWindowPoints(HWND_DESKTOP, GetParent(windowHandle), (LPPOINT)&mWindowRect, 2);

    mScreenAbsoluteCenterX = (mWindowRect.left + mWindowRect.right) >> 1;
    mScreenAbsoluteCenterY = (mWindowRect.top + mWindowRect.bottom) >> 1;
}

SimpleMath::Matrix Camera::GetViewMatrix() const
{
    return SimpleMath::Matrix::CreateTranslation(-mPos) * SimpleMath::Matrix::CreateRotationY(-mYawInRadian) * SimpleMath::Matrix::CreateRotationX(-mPitchInRadian);
}

SimpleMath::Matrix Camera::GetProjMatrix() const
{
    return XMMatrixPerspectiveFovLH(XMConvertToRadians(mFOV), mScreenWidth / static_cast<float>(mScreenHeight), mNearZ, mFarZ);
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